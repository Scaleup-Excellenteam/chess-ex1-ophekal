#include "Pieces/Bishop.h"
#include "Factory/PieceFactory.h"
#include <iostream>


// Register the Bishop into the factory
bool Bishop::m_register = PieceFactory::registerPiece("Bishop",
	[](const std::string& pos, bool isBlack) {
		return std::make_unique<Bishop>(pos, isBlack);
	});


//----------------------------------------------------------
// Bishop Constructor
Bishop::Bishop(const std::string& position, bool isBlack)
	:Piece(position, isBlack, "Bishop") {}


// Checks if direction is valid (diagonal)
bool Bishop::isDirectionValid(const std::string& targetPosition) const
{
	auto [currentRow, currentCol] = positionToCoords(m_position);
	auto [targetRow, targetCol] = positionToCoords(targetPosition);

	return (std::abs(targetRow - currentRow) == std::abs(targetCol - currentCol));
}