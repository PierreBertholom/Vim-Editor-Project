#include "PieceTable.hpp"
#include <algorithm>
#include <stdexcept>

PieceTable::PieceTable(const std::string& initialContent) : textLength(initialContent.length()) {
    originalBuffer.insert(initialContent, 0);
    // un seul morceau pour décrire le buffer original
    if (!initialContent.empty()) {
        pieces.emplace_back(&originalBuffer, 0, initialContent.length());
    }
}

PieceTable& PieceTable::operator=(const PieceTable& other) {
    if (this != &other) {
        originalBuffer = other.originalBuffer;
        addBuffer = other.addBuffer;
        textLength = other.textLength;
        
        pieces.clear();
        for (const auto& piece : other.pieces) {
            Buffer* DestSource;
            if (piece.source == &other.originalBuffer) {
                DestSource = &originalBuffer;
            } else {
                DestSource = &addBuffer;
            }
            pieces.emplace_back(DestSource, piece.start, piece.length);
        }
    }
    return *this;
}

size_t PieceTable::getLength() const {
    return textLength;
}

std::string PieceTable::getFullText() const {
    std::string result = "";
    result.reserve(textLength);
    for(const auto& piece : pieces) {
        result.append(piece.source->getText(piece.start, piece.length));
    }
    return result;
}

std::string PieceTable::getText(size_t position, size_t count) const {
    return getFullText().substr(position, count);
}

void PieceTable::insert(const std::string& text, size_t position) {
    if(text.empty()) return;

    // ajoute le nouveau texte au addBuffer et crée un morceau
    size_t addStart = addBuffer.getLength();
    addBuffer.insert(text, addStart);
    Piece newPiece(&addBuffer, addStart, text.length());

    // trouve le morceau de l'insertion
    size_t currentPos = 0;
    for (auto it = pieces.begin(); it != pieces.end(); ++it) {
        if (position >= currentPos && position <= currentPos + it->length) {
            // morceau où on insère
            Piece& currentPiece = *it;

            // séparer en deux
            Piece leftPart(currentPiece.source, currentPiece.start, position - currentPos);
            Piece rightPart(currentPiece.source, currentPiece.start+leftPart.length, currentPiece.length - leftPart.length);

            // remplace le morceau actuel par la partie gauche
            *it = leftPart;

            // insère la nouvelle pièce et la partie droite après
            it = pieces.insert(it+1, newPiece);
            it = pieces.insert(it+1, rightPart);

            // supprime les morceaux de longueur nulle
            pieces.erase(std::remove_if(pieces.begin(), pieces.end(), 
                                      [](const Piece& p){ return p.length == 0; }), 
                        pieces.end());

            textLength += text.length();
            return;
        }
        currentPos += it->length;
    }

    // insertion à la toute fin du texte
    pieces.push_back(newPiece);
    textLength += text.length();
}

// La suppression fonctionne en redécoupant les morceaux affectés
void PieceTable::remove(size_t position, size_t count) {
    if (count == 0 || position >= textLength) return;
    
    count = std::min(count, textLength - position);
    
    size_t endPos = position + count;
    size_t currentPos = 0;
    
    auto it = pieces.begin();
    while(it != pieces.end() && currentPos < endPos) {
        size_t pieceEnd = currentPos + it->length;
        
        // morceau avant la position de suppression
        if (pieceEnd <= position) {
            currentPos = pieceEnd;
            ++it;
            continue;
        }
        
        // morceau après
        if (currentPos >= endPos) {
            break;
        }
        
        // morceau overlap la suppression
        size_t overlapStart = std::max(position, currentPos);
        size_t overlapEnd = std::min(endPos, pieceEnd);
        
        // Cas 1 : morceau est entièrement dans la zone à supprimer
        if (currentPos >= position && pieceEnd <= endPos) {
            it = pieces.erase(it);
            currentPos = pieceEnd;
        }
        // Cas 2 : la suppression est au milieu du morceau (découpage en deux)
        else if (currentPos < position && pieceEnd > endPos) {
            Piece leftPart(it->source, it->start, position - currentPos);
            Piece rightPart(it->source, it->start + (endPos - currentPos), pieceEnd - endPos);
            
            *it = leftPart;
            it = pieces.insert(it + 1, rightPart);
            ++it;
            currentPos = pieceEnd;
        }
        // Cas 3 : la suppression coupe le début du morceau
        else if (currentPos < position && pieceEnd > position) {
            it->length = position - currentPos;
            currentPos = pieceEnd;
            ++it;
        }
        // Cas 4 : la suppression coupe la fin du morceau
        else if (currentPos >= position && currentPos < endPos && pieceEnd > endPos) {
            size_t newStart = it->start + (endPos - currentPos);
            size_t newLength = pieceEnd - endPos;
            it->start = newStart;
            it->length = newLength;
            currentPos = pieceEnd;
            ++it;
        }
        else {
            ++it;
            currentPos = pieceEnd;
        }
    }
    
    textLength -= count;
}
