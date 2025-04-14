#include "Pieces/Queen.h"
#include "Factory/PieceFactory.h"
#include <iostream>


// Register the Queen into the factory
bool Queen::m_register = PieceFactory::registerPiece("Queen",
	[](const std::string& pos, bool isBlack) {
		return std::make_unique<Queen>(pos, isBlack);
	});


//----------------------------------------------------------
// Queen Constructor
Queen::Queen(const std::string& position, bool isBlack)
	:Piece(position, isBlack, "Queen") {}


// Checks if direction is valid (diagonal, horizontal, or vertical)
bool Queen::isDirectionValid(const std::string& targetPosition) const
{
	auto [currentRow, currentCol] = positionToCoords(m_position);
	auto [targetRow, targetCol] = positionToCoords(targetPosition);
	
	bool isVertical = (currentRow == targetRow);
	bool isHorizontal = (currentCol == targetCol);
	bool isDiagonal = (std::abs(targetRow - currentRow) == std::abs(targetCol - currentCol));

	return isVertical || isHorizontal || isDiagonal;
}