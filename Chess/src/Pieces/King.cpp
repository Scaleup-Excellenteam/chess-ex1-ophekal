#include "Pieces/King.h"
#include "Factory/PieceFactory.h"
#include <iostream>


/**
 * Registers the King piece into the PieceFactory. This enables the factory to create a King
 * instance when requested by name.
 *
 * The registration happens statically, ensuring that the King piece is available for creation
 * at the start of the program.
 */
bool King::m_register = PieceFactory::registerPiece("King",
	[](const std::string& pos, bool isBlack) {
		return std::make_unique<King>(pos, isBlack);
	});


//----------------------------------------------------------

/**
 * Constructs a King piece with the given position and color.
 *
 * @param position The position of the King on the chessboard
 * @param isBlack A boolean indicating whether the King is black (true) or white (false).
 */
King::King(const std::string& position, bool isBlack)
	:Piece(position, isBlack, "King") {}


/*
*/
std::unique_ptr<Piece> King::clone() const
{
	return std::make_unique<King>(*this);
}

int King::getValue() const
{
	return 100;
}



/**
 * Checks if the direction from the current position to the target position is valid for a King.
 * A King moves one square in any direction—vertically, horizontally, or diagonally.
 *
 * @param targetPosition The target position to check
 *
 * @return A boolean value:
 *         - true if the movement from the current position to the target position is valid (one square in any direction).
 *         - false if the move is invalid.
 */
bool King::isDirectionValid(const std::string& targetPosition) const
{	
	auto [currentRow, currentCol] = positionToCoords(m_position);
	auto [targetRow, targetCol] = positionToCoords(targetPosition);

	return (((std::abs(currentRow - targetRow) == 1) && (std::abs(currentCol - targetCol) == 1)) ||
			((std::abs(currentRow - targetRow) == 1) && (currentCol == targetCol)) ||
			((currentRow == targetRow) && (std::abs(currentCol - targetCol) == 1)));
}