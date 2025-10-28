#ifndef COMMANDMANAGER_HPP
#define COMMANDMANAGER_HPP

#include <stack>
#include <vector>
#include <memory>
#include <functional>
#include "Command.hpp"

class CommandManager {
private:
    std::stack<std::unique_ptr<Command>> undoStack;
    std::stack<std::unique_ptr<Command>> redoStack;
    size_t saveState;
    
    std::vector<std::function<void()>> macroActions;
    bool isRecording;

public:
    CommandManager() : saveState(0), isRecording(false) {}
    
    void executeCommand(std::unique_ptr<Command> command);
    void undo();
    void redo();
    bool canUndo() const;
    bool canRedo() const;
    void clear();
    void markAsSaved();
    bool isModified() const;
    
    // Macro
    void startRecording();
    void stopRecording();
    void playMacro();
    bool recording() const;
    bool hasMacro() const;
    void recordAction(std::function<void()> action);
};

#endif // COMMANDMANAGER_HPP
