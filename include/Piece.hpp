#ifndef PIECE_HPP
#define PIECE_HPP

#include <cstddef>

class Buffer;

struct Piece {
    const Buffer* source;
    size_t start;
    size_t length;

    Piece(const Buffer* src, size_t st, size_t len) : source(src), start(st), length(len) {}
};

#endif // PIECE_HPP