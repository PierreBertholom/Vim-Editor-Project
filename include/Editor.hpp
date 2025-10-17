#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "PieceTable.hpp"
#include "Selection.hpp"
#include <string>

class Editor {
private:
    PieceTable document;
    Selection selection;
    bool selectingMode;

public:
    Editor(const std::string& initialContent = "Hello, world!\n");

    void copy();
    void cut();
    void paste();
    void selectAll();
    void insertText(const std::string& text);
    void deleteSelection();
    void deleteBackward();
    void deleteForward();

    void moveCursor(int offset);
    void extendSelection(int offset);

    void toggleSelectionMode();
    void toggleNormalMode();
    bool isSelecting() const;

    std::string getBufferContent() const;
    size_t getSelectionStart() const;
    size_t getSelectionEnd() const;
    size_t getCursorPosition() const;
};

#endif // EDITOR_HPP