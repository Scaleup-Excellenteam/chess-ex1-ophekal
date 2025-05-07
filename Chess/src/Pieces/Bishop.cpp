#include "Pieces/Bishop.h"
#include "Factory/PieceFactory.h"
#include <iostream>


/**
 * Register the Bishop piece into the PieceFactory. This allows the factory to create a Bishop
 * instance when requested by name.
 *
 * The registration is done statically, so the Bishop piece will be available for creation when
 * the program starts.
 */
bool Bishop::m_register = PieceFactory::registerPiece("Bishop",
	[](const std::string& pos, bool isBlack) {
		return std::make_unique<Bishop>(pos, isBlack);
	});


//----------------------------------------------------------

/**
 * Constructs a Bishop piece with the specified position and color.
 *
 * @param position The position of the Bishop on the chessboard.
 * @param isBlack A boolean indicating whether the Bishop is black (true) or white (false).
 */
Bishop::Bishop(const std::string& position, bool isBlack)
	:Piece(position, isBlack, "Bishop") {}


/*
*/
std::unique_ptr<Piece> Bishop::clone() const
{
	return std::make_unique<Bishop>(*this);
}


/**
 * Checks if the direction from the current position to the target position is valid for a Bishop.
 * A Bishop moves diagonally, so the difference in rows and columns must be equal.
 *
 * @param targetPosition The target position to check.
 *
 * @return A boolean value:
 *         - true if the movement from the current position to the target position is valid (diagonal move).
 *         - false if the move is invalid.
 */
bool Bishop::isDirectionValid(const std::string& targetPosition) const
{
	auto [currentRow, currentCol] = positionToCoords(m_position);
	auto [targetRow, targetCol] = positionToCoords(targetPosition);

	return (std::abs(targetRow - currentRow) == std::abs(targetCol - currentCol));
}