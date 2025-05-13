#pragma once
#include "Pieces/Piece.h"

class Bishop : public Piece
{
public:
	Bishop(const std::string& position, bool isBlack);
	std::unique_ptr<Piece> clone() const override;
	int getValue() const override;
	bool isDirectionValid (const std::string& targetPosition) const override;

private:
	static bool m_register; // For registering the Bishop piece in the factory
};