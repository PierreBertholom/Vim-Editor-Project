#ifndef TEXTCOMMANDS_HPP
#define TEXTCOMMANDS_HPP

#include "Command.hpp"
#include "PieceTable.hpp"
#include "Selection.hpp"
#include <string>

// Commande pour insérer du texte
class InsertCommand : public Command {
private:
    PieceTable& document;
    Selection& selection;
    size_t position;
    std::string insertedText;
    size_t oldSelectionStart;
    size_t oldSelectionEnd;

public:
    InsertCommand(PieceTable& doc, Selection& sel, size_t pos, const std::string& text);
    void execute() override;
    void undo() override;
    std::unique_ptr<Command> clone() const override;
};

// Commande pour supprimer du texte
class DeleteCommand : public Command {
private:
    PieceTable& document;
    Selection& selection;
    size_t position;
    size_t length;
    std::string deletedText;
    size_t oldSelectionStart;
    size_t oldSelectionEnd;

public:
    DeleteCommand(PieceTable& doc, Selection& sel, size_t pos, size_t len);
    void execute() override;
    void undo() override;
    std::unique_ptr<Command> clone() const override;
};

#endif // TEXTCOMMANDS_HPP
