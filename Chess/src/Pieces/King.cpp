#include "Pieces/King.h"
#include "Factory/PieceFactory.h"
#include <iostream>


// Register the King into the factory
bool King::m_register = PieceFactory::registerPiece("King",
	[](const std::string& pos, bool isBlack) {
		return std::make_unique<King>(pos, isBlack);
	});


//----------------------------------------------------------
// King Constructor
King::King(const std::string& position, bool isBlack)
	:Piece(position, isBlack, "King") {}



// Checks if direction is valid (one square in any direction)
bool King::isDirectionValid(const std::string& targetPosition) const
{	
	auto [currentRow, currentCol] = positionToCoords(m_position);
	auto [targetRow, targetCol] = positionToCoords(targetPosition);

	return (((std::abs(currentRow - targetRow) == 1) && (std::abs(currentCol - targetCol) == 1)) ||
			((std::abs(currentRow - targetRow) == 1) && (currentCol == targetCol)) ||
			((currentRow == targetRow) && (std::abs(currentCol - targetCol) == 1)));
}