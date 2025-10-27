#include "TextCommands.hpp"

// InsertCommand
InsertCommand::InsertCommand(PieceTable& doc, Selection& sel, size_t pos, const std::string& text)
    : document(doc), selection(sel), position(pos), insertedText(text),
      oldSelectionStart(sel.getStart()), oldSelectionEnd(sel.getEnd()) {}

void InsertCommand::execute() {
    document.insert(insertedText, position);
    size_t newPos = position + insertedText.length();
    selection.setStart(newPos);
    selection.setEnd(newPos);
}

void InsertCommand::undo() {
    document.remove(position, insertedText.length());
    selection.setStart(oldSelectionStart);
    selection.setEnd(oldSelectionEnd);
}

// DeleteCommand
DeleteCommand::DeleteCommand(PieceTable& doc, Selection& sel, size_t pos, size_t len)
    : document(doc), selection(sel), position(pos), length(len),
      oldSelectionStart(sel.getStart()), oldSelectionEnd(sel.getEnd()) {}

void DeleteCommand::execute() {
    deletedText = document.getText(position, length);
    document.remove(position, length);
    selection.setStart(position);
    selection.setEnd(position);
}

void DeleteCommand::undo() {
    document.insert(deletedText, position);
    selection.setStart(oldSelectionStart);
    selection.setEnd(oldSelectionEnd);
}
