#pragma once
#include "Pieces/Piece.h"

class Queen : public Piece
{
public:
	Queen(const std::string& position, bool isBlack);
	bool isDirectionValid (const std::string& targetPosition) const override;

private:
	static bool m_register; // For registering the Queen piece in the factory
};