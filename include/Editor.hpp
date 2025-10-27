#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "PieceTable.hpp"
#include "Selection.hpp"
#include "CommandManager.hpp"
#include <string>

class Editor {
private:
    PieceTable document;
    Selection selection;
    bool selectingMode;
    std::string filename;
    bool modified;
    size_t targetColumn;
    CommandManager commandManager;

public:
    Editor(const std::string& initialContent = "");
    
    bool loadFile(const std::string& filepath);
    bool saveFile();
    bool saveFileAs(const std::string& filepath);
    std::string getFilename() const;
    bool hasFilename() const;
    bool hasBeenModified() const;

    void copy();
    void cut();
    void paste();
    void selectAll();
    void insertText(const std::string& text);
    void deleteSelection();
    void deleteBackward();
    void deleteForward();
    void deleteCurrentLine();

    void moveCursor(int offset);
    void moveCursorVertical(int lineOffset);
    void extendSelection(int offset);
    void extendSelectionVertical(int lineOffset);

    void toggleSelectionMode();
    void toggleNormalMode();
    bool isSelecting() const;

    std::string getBufferContent() const;
    size_t getSelectionStart() const;
    size_t getSelectionEnd() const;
    size_t getCursorPosition() const;

    void undo();
    void redo();
    bool canUndo() const;
    bool canRedo() const;
};

#endif // EDITOR_HPP