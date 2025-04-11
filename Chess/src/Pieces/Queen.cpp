#include "Pieces/Queen.h"
#include "Factory/PieceFactory.h"
#include <iostream>


// Register the Queen into the factory
bool Queen::m_register = PieceFactory::registerPiece("Queen",
	[](const std::string& pos, bool isBlack) {
		return std::make_unique<Queen>(pos, isBlack);
	});


//----------------------------------------------------------
Queen::Queen(const std::string& position, bool isBlack)
	:Piece(position, isBlack, "Queen") {}


// Only checks if direction is valid (diagonal, horizontal, or vertical)
bool Queen::isDirectionValid(const std::string& targetPosition) const
{
	return false;
}