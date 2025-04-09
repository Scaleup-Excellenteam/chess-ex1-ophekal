#pragma once
#include "Pieces/Piece.h"

class Knight : public Piece {
public:
    Knight(const std::string& position, bool isBlack);
    bool isMoveValid(const std::string& targetPosition, const BoardMap& board) const override;

private:
    static bool m_register; // For registering the Knight piece in the factory

};