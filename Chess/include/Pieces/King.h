#pragma once
#include "Pieces/Piece.h"

class King : public Piece
{
public:
	King(const std::string& position, bool isBlack);
	bool isMoveValid(const std::string& targetPosition, const BoardMap& board) const override;

private:
	static bool m_register; // For registering the King piece in the factory
};