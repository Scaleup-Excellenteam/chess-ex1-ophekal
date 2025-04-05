#pragma once
#include "Pieces/Piece.h"

class Pawn : public Piece {
public:
    Pawn(const std::string& position, bool isBlack);
    bool isMoveValid(const std::string& targetPosition) const override;

private:
    static bool m_register; // For registering the Pawn piece in the factory

};