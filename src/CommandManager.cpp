#include "CommandManager.hpp"

void CommandManager::executeCommand(std::unique_ptr<Command> command) {
    command->execute();
    undoStack.push(std::move(command));
    // nouvelle action -> nouvelle "branche" -> vider la pile redo
    while (!redoStack.empty()) {
        redoStack.pop();
    }
}

void CommandManager::undo() {
    if (!canUndo()) return;
    
    auto command = std::move(undoStack.top());
    undoStack.pop();
    command->undo();
    redoStack.push(std::move(command));
}

void CommandManager::redo() {
    if (!canRedo()) return;
    
    auto command = std::move(redoStack.top());
    redoStack.pop();
    command->execute();
    undoStack.push(std::move(command));
}

bool CommandManager::canUndo() const {
    return !undoStack.empty();
}

bool CommandManager::canRedo() const {
    return !redoStack.empty();
}

void CommandManager::clear() {
    while (!undoStack.empty()) undoStack.pop();
    while (!redoStack.empty()) redoStack.pop();
    saveState = 0;
}

void CommandManager::markAsSaved() {
    saveState = undoStack.size();
}

bool CommandManager::isModified() const {
    return undoStack.size() != saveState;
}