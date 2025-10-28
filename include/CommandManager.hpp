#ifndef COMMANDMANAGER_HPP
#define COMMANDMANAGER_HPP

#include <stack>
#include <memory>
#include "Command.hpp"

class CommandManager {
private:
    std::stack<std::unique_ptr<Command>> undoStack;
    std::stack<std::unique_ptr<Command>> redoStack;
    size_t saveState;

public:
    CommandManager() : saveState{0} {};
    void executeCommand(std::unique_ptr<Command> command);
    void undo();
    void redo();
    bool canUndo() const;
    bool canRedo() const;
    void clear();

    void markAsSaved();
    bool isModified() const;
};

#endif // COMMANDMANAGER_HPP
