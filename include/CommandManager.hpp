#ifndef COMMANDMANAGER_HPP
#define COMMANDMANAGER_HPP

#include <stack>
#include <memory>
#include "Command.hpp"

class CommandManager {
private:
    std::stack<std::unique_ptr<Command>> undoStack;
    std::stack<std::unique_ptr<Command>> redoStack;

public:
    void executeCommand(std::unique_ptr<Command> command);
    void undo();
    void redo();
    bool canUndo() const;
    bool canRedo() const;
    void clear();
};

#endif // COMMANDMANAGER_HPP
