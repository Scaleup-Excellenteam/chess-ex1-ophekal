#pragma once
#include "Pieces/Piece.h"

class King : public Piece
{
public:
	King(const std::string& position, bool isBlack);
	std::unique_ptr<Piece> clone() const override;
	int getValue() const override;
	bool isDirectionValid(const std::string& targetPosition) const override;

private:
	static bool m_register; // For registering the King piece in the factory
};