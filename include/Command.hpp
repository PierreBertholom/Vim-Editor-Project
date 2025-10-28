#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <memory>

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual std::unique_ptr<Command> clone() const = 0;
};

#endif // COMMAND_HPP
