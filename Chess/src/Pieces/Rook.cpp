#include "Pieces/Rook.h"
#include "Factory/PieceFactory.h"
#include <iostream>


// Register the Rook into the factory
bool Rook::m_register = PieceFactory::registerPiece("Rook",
	[](const std::string& pos, bool isBlack) {
		return std::make_unique<Rook>(pos, isBlack);
	});


//----------------------------------------------------------
// Rook Constructor
Rook::Rook(const std::string& position, bool isBlack)
	:Piece(position, isBlack, "Rook") {}


// Checks if direction is valid (horizontal or vertical)
bool Rook::isDirectionValid(const std::string& targetPosition) const
{
	auto [currentRow, currentCol] = positionToCoords(m_position);
	auto [targetRow, targetCol] = positionToCoords(targetPosition);

	return currentRow == targetRow || currentCol == targetCol;
}