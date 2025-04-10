#pragma once
#include "Pieces/Piece.h"

class Bishop : public Piece
{
public:
	Bishop(const std::string& position, bool isBlack);
	bool isDirectionValid (const std::string& targetPosition) const override;

private:
	static bool m_register; // For registering the Bishop piece in the factory
};