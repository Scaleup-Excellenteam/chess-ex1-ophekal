#pragma once
#include "Pieces/Piece.h"

class Pawn : public Piece {
public:
    Pawn(const std::string& position, bool isBlack);
    std::unique_ptr<Piece> clone() const override;
    bool isDirectionValid(const std::string& targetPosition) const override;

private:
    static bool m_register; // For registering the Pawn piece in the factory

};