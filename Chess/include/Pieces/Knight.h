#pragma once
#include "Pieces/Piece.h"

class Knight : public Piece {
public:
    Knight(const std::string& position, bool isBlack);
    std::unique_ptr<Piece> clone() const override;
    bool isDirectionValid (const std::string& targetPosition) const override;

private:
    static bool m_register; // For registering the Knight piece in the factory

};