#ifndef SELECTION_HPP
#define SELECTION_HPP

#include <cstddef>

class Selection {
private:
    size_t start;
    size_t end;

public:
    Selection();
    size_t getStart() const;
    size_t getEnd() const;
    size_t getCursorPosition() const;
    void setStart(size_t position);
    void setEnd(size_t position);
};

#endif // SELECTION_HPP