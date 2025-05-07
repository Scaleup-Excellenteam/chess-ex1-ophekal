#include "Pieces/Rook.h"
#include "Factory/PieceFactory.h"
#include <iostream>


/**
 * Registers the Rook piece into the PieceFactory. This allows the factory to create a Rook
 * instance when requested by name.
 *
 * The registration is done statically, so the Rook piece will be available for creation when
 * the program starts.
 */
bool Rook::m_register = PieceFactory::registerPiece("Rook",
	[](const std::string& pos, bool isBlack) {
		return std::make_unique<Rook>(pos, isBlack);
	});


//----------------------------------------------------------

/**
 * Constructs a Rook piece with the given position and color.
 *
 * @param position The position of the Rook on the chessboard.
 * @param isBlack A boolean indicating whether the Rook is black (true) or white (false).
 */
Rook::Rook(const std::string& position, bool isBlack)
	:Piece(position, isBlack, "Rook") {}


/*
*/
std::unique_ptr<Piece> Rook::clone() const
{
	return std::make_unique<Rook>(*this);
}


/**
 * Checks if the direction from the current position to the target position is valid for a Rook.
 * A Rook can move either horizontally or vertically any number of squares.
 *
 * @param targetPosition The target position to check.
 *
 * @return A boolean value:
 *         - true if the move is along a horizontal or vertical line,
 *         - false otherwise.
 */
bool Rook::isDirectionValid(const std::string& targetPosition) const
{
	auto [currentRow, currentCol] = positionToCoords(m_position);
	auto [targetRow, targetCol] = positionToCoords(targetPosition);

	return currentRow == targetRow || currentCol == targetCol;
}