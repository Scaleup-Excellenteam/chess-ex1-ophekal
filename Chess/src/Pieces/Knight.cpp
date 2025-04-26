#include "Pieces/Knight.h"
#include "Factory/PieceFactory.h"
#include <iostream>


/**
 * Registers the Knight piece into the PieceFactory. This allows the factory to create a Knight
 * instance when requested by name.
 *
 * The registration is done statically, so the Knight piece will be available for creation when
 * the program starts.
 */
bool Knight::m_register = PieceFactory::registerPiece("Knight",
	[](const std::string& pos, bool isBlack) {
		return std::make_unique<Knight>(pos, isBlack);
	});


//----------------------------------------------------------

/**
 * Constructs a Knight piece with the given position and color.
 *
 * @param position The position of the Knight on the chessboard.
 * @param isBlack A boolean indicating whether the Knight is black (true) or white (false).
 */
Knight::Knight(const std::string& position, bool isBlack)
	:Piece(position, isBlack, "Knight") {}


/**
 * Checks if the direction from the current position to the target position is valid for a Knight.
 * The logic for a Knight’s movement is not implemented in this method.
 * 
 * @param targetPosition The target position to check.
 *
 * @return A boolean value:
 *         - false, as the movement logic for the Knight is not implemented in this method.
 */
bool Knight::isDirectionValid(const std::string& targetPosition) const
{
	auto [currentRow, currentCol] = positionToCoords(m_position);
	auto [targetRow, targetCol] = positionToCoords(targetPosition);

	return ((std::abs(currentRow - targetRow) == 2 && std::abs(currentCol - targetCol) == 1) ||
		    (std::abs(currentRow - targetRow) == 1 && std::abs(currentCol - targetCol) == 2));

}