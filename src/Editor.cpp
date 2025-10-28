#include "Editor.hpp"
#include "Clipboard.hpp"
#include "TextCommands.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

Editor::Editor(const std::string& initialContent)
    : document(initialContent), selectingMode(false), filename(""), targetColumn(0) {}

void Editor::insertText(const std::string& text) {
    // Si du texte est sélectionné, on le supprime d'abord
    if (selection.getStart() != selection.getEnd()) {
        size_t start = selection.getStart();
        size_t end = selection.getEnd();
        auto deleteCmd = std::make_unique<DeleteCommand>(document, selection, start, end - start);
        commandManager.executeCommand(std::move(deleteCmd));
    }

    size_t pos = selection.getStart();
    auto insertCmd = std::make_unique<InsertCommand>(document, selection, pos, text);
    commandManager.executeCommand(std::move(insertCmd));

    selectingMode = false;
}

void Editor::moveCursor(int offset) {
    selectingMode = false;

    int currentPos = static_cast<int>(selection.getCursorPosition());
    int newPos = currentPos + offset;
    
    newPos = std::max(0, newPos);
    newPos = std::min(newPos, static_cast<int>(document.getLength()));

    selection.setStart(static_cast<size_t>(newPos));
    selection.setEnd(static_cast<size_t>(newPos));
}

void Editor::extendSelection(int offset) {

    int currentEnd = static_cast<int>(selection.getCursorPosition());
    int newEnd = currentEnd + offset;
    
    newEnd = std::max(0, newEnd);
    newEnd = std::min(newEnd, static_cast<int>(document.getLength()));
    
    selection.setEnd(static_cast<size_t>(newEnd));
}

void Editor::moveCursorVertical(int lineOffset) {
    selectingMode = false;
    
    std::string content = document.getFullText();
    size_t cursorPos = selection.getCursorPosition();
    
    // début de la ligne = '\n'
    size_t lineStart = cursorPos;
    while (lineStart > 0 && content[lineStart - 1] != '\n') {
        lineStart--;
    }
    
    // colonne de la ligne
    size_t currentColumn = cursorPos - lineStart;
    targetColumn = currentColumn;
    
    size_t newPos = cursorPos;
    
    if (lineOffset < 0) { // -1 = Haut
        if (lineStart > 0) {
            // début de la ligne précédente
            size_t prevLineEnd = lineStart - 1; // avant '\n'
            size_t prevLineStart = prevLineEnd;
            while (prevLineStart > 0 && content[prevLineStart - 1] != '\n') {
                prevLineStart--;
            }
            
            // nouvelle position selon la colonne de la ligne initial
            size_t prevLineLength = prevLineEnd - prevLineStart;
            newPos = prevLineStart + std::min(targetColumn, prevLineLength);
        }
    } else if (lineOffset > 0) { // +1 = Bas

        // même logique
        size_t lineEnd = cursorPos;
        while (lineEnd < content.length() && content[lineEnd] != '\n') {
            lineEnd++;
        }
        
        if (lineEnd < content.length()) {
            size_t nextLineStart = lineEnd + 1;
            size_t nextLineEnd = nextLineStart;
            while (nextLineEnd < content.length() && content[nextLineEnd] != '\n') {
                nextLineEnd++;
            }
            
            size_t nextLineLength = nextLineEnd - nextLineStart;
            newPos = nextLineStart + std::min(targetColumn, nextLineLength);
        }
    }
    
    selection.setStart(newPos);
    selection.setEnd(newPos);
}

void Editor::extendSelectionVertical(int lineOffset) {
    // Même logique que pour moveCursorVertical
    std::string content = document.getFullText();
    size_t cursorPos = selection.getCursorPosition();
    
    size_t lineStart = cursorPos;
    while (lineStart > 0 && content[lineStart - 1] != '\n') {
        lineStart--;
    }
    
    size_t currentColumn = cursorPos - lineStart;
    targetColumn = currentColumn;
    
    size_t newPos = cursorPos;
    
    if (lineOffset < 0) {
        if (lineStart > 0) {
            size_t prevLineEnd = lineStart - 1;
            size_t prevLineStart = prevLineEnd;
            while (prevLineStart > 0 && content[prevLineStart - 1] != '\n') {
                prevLineStart--;
            }
            
            size_t prevLineLength = prevLineEnd - prevLineStart;
            newPos = prevLineStart + std::min(targetColumn, prevLineLength);
        }
    } else if (lineOffset > 0) {
        size_t lineEnd = cursorPos;
        while (lineEnd < content.length() && content[lineEnd] != '\n') {
            lineEnd++;
        }
        
        if (lineEnd < content.length()) {
            size_t nextLineStart = lineEnd + 1;
            size_t nextLineEnd = nextLineStart;
            while (nextLineEnd < content.length() && content[nextLineEnd] != '\n') {
                nextLineEnd++;
            }
            
            size_t nextLineLength = nextLineEnd - nextLineStart;
            newPos = nextLineStart + std::min(targetColumn, nextLineLength);
        }
    }
    
    selection.setEnd(newPos);
}

void Editor::toggleSelectionMode() {
    selectingMode = !selectingMode;
}

void Editor::toggleNormalMode() {
    selectingMode = false;
}

bool Editor::isSelecting() const {
    return selectingMode;
}

void Editor::copy() {
    size_t start = selection.getStart();
    size_t end = selection.getEnd();
    if (start != end) {
        std::string selectedText = document.getText(start, end - start);
        Clipboard::getInstance().setContent(selectedText);
    }
}

void Editor::cut() {
    copy();
    deleteSelection();
}

void Editor::selectAll() {
    selectingMode = true;

    selection.setStart(0);
    selection.setEnd(document.getLength());
}

void Editor::deleteSelection() {
     size_t start = selection.getStart();
     size_t end = selection.getEnd();
     if (start != end) {
        auto cmd = std::make_unique<DeleteCommand>(document, selection, start, end - start);
        commandManager.executeCommand(std::move(cmd));
     }
}

void Editor::deleteBackward() {
    size_t start = selection.getStart();
    size_t end = selection.getEnd();
    
    // supprime selection si elle existe
    if (start != end) {
        deleteSelection();
        selectingMode = false;
    }
    else if (start > 0) {
        auto cmd = std::make_unique<DeleteCommand>(document, selection, start - 1, 1);
        commandManager.executeCommand(std::move(cmd));
        selectingMode = false;
    }
}

void Editor::deleteForward() {
    size_t start = selection.getStart();
    size_t end = selection.getEnd();
    
    // supprime selection si elle existe
    if (start != end) {
        deleteSelection();
        selectingMode = false;
    }
    else if (start < document.getLength()) {
        auto cmd = std::make_unique<DeleteCommand>(document, selection, start, 1);
        commandManager.executeCommand(std::move(cmd));
        selectingMode = false;
    }
}

void Editor::deleteCurrentLine() {
    std::string content = document.getFullText();
    size_t cursorPos = selection.getCursorPosition();
    
    // début de ligne -> dernier '\n'
    size_t lineStart = cursorPos;
    while (lineStart > 0 && content[lineStart - 1] != '\n') {
        lineStart--;
    }
    
    // fin de ligne -> '\n' suivant
    size_t lineEnd = cursorPos;
    while (lineEnd < content.length() && content[lineEnd] != '\n') {
        lineEnd++;
    }

    if (lineEnd < content.length() && content[lineEnd] == '\n') {
        lineEnd++;
    }
    
    // supprime la ligne
    if (lineEnd > lineStart) {
        auto cmd = std::make_unique<DeleteCommand>(document, selection, lineStart, lineEnd - lineStart);
        commandManager.executeCommand(std::move(cmd));
        selectingMode = false;
    }
}

void Editor::paste() {
    const std::string& clipboardContent = Clipboard::getInstance().getContent();
    if (!clipboardContent.empty()) {
        insertText(clipboardContent);
    }
}

std::string Editor::getBufferContent() const {
    return document.getFullText();
}

size_t Editor::getSelectionStart() const {
    return selection.getStart();
}

size_t Editor::getSelectionEnd() const {
    return selection.getEnd();
}

size_t Editor::getCursorPosition() const {
    return selection.getCursorPosition();
}

bool Editor::hasBeenModified() const {
    return commandManager.isModified();
}

bool Editor::loadFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return false;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();
    
    document = PieceTable(content);
    
    selection.setStart(0);
    selection.setEnd(0);
    selectingMode = false;
    
    filename = filepath;
    commandManager.clear();
    commandManager.markAsSaved();
    
    return true;
}

bool Editor::saveFile() {
    if (filename.empty()) {
        return false;
    }
    return saveFileAs(filename);
}

bool Editor::saveFileAs(const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        return false;
    }
    
    file << document.getFullText();
    file.close();
    
    filename = filepath;
    commandManager.clear();
    commandManager.markAsSaved();
    return true;
}

std::string Editor::getFilename() const {
    return filename;
}

bool Editor::hasFilename() const {
    return !filename.empty();
}

void Editor::undo() {
    commandManager.undo();
}

void Editor::redo() {
    commandManager.redo();
}

bool Editor::canUndo() const {
    return commandManager.canUndo();
}

bool Editor::canRedo() const {
    return commandManager.canRedo();
}
