#include "Selection.hpp"
#include <algorithm>

Selection::Selection() : start(0), end(0) {}

size_t Selection::getStart() const {
    return std::min(start, end);
}

size_t Selection::getEnd() const {
    return std::max(start, end);
}

size_t Selection::getCursorPosition() const {
    return end;
}

void Selection::setStart(size_t position) {
    start = position;
}

void Selection::setEnd(size_t position) {
    end = position;
}