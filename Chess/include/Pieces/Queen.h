#pragma once
#include "Pieces/Piece.h"

class Queen : public Piece
{
public:
	Queen(const std::string& position, bool isBlack);
	std::unique_ptr<Piece> clone() const;
	bool isDirectionValid (const std::string& targetPosition) const override;

private:
	static bool m_register; // For registering the Queen piece in the factory
};