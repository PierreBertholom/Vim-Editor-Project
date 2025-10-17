#ifndef PIECE_TABLE_HPP
#define PIECE_TABLE_HPP

#include "Buffer.hpp"
#include "Piece.hpp"
#include <string>
#include <vector>

class PieceTable {
private:
    Buffer originalBuffer;
    Buffer addBuffer;
    std::vector<Piece> pieces;
    size_t textLength;

public:
    PieceTable(const std::string& initialContent);

    void insert(const std::string& text, size_t position);
    void remove(size_t position, size_t count);

    std::string getText(size_t position, size_t count) const;
    std::string getFullText() const;
    size_t getLength() const;
};

#endif // PIECE_TABLE_HPP