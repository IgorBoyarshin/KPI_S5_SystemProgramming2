#ifndef PIECE_H
#define PIECE_H

#include "PieceType.h"


class Piece {
    public:
        PieceType type;
        std::string content;
        Piece(PieceType pieceType, const std::string& content);
};


#endif
