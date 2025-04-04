#pragma once
#include "Pieces/Piece.h"

class Rook: public Piece
{
public:
	Rook(const std::string& position, bool isBlack);
	bool isMoveValid(const std::string& targetPosition) const override;

private:
	static bool m_register; // For registering the Rook piece in the factory
};