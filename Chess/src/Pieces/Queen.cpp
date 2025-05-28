#include "Pieces/Queen.h"
#include "Factory/PieceFactory.h"
#include <iostream>


/**
 * Registers the Queen piece into the PieceFactory. This allows the factory to create a Queen
 * instance when requested by name.
 *
 * The registration is done statically, so the Queen piece will be available for creation when
 * the program starts.
 */
bool Queen::m_register = PieceFactory::registerPiece("Queen",
	[](const std::string& pos, bool isBlack) {
		return std::make_unique<Queen>(pos, isBlack);
	});


//----------------------------------------------------------

/**
 * Constructs a Queen piece with the given position and color.
 *
 * @param position The position of the Queen on the chessboard.
 * @param isBlack A boolean indicating whether the Queen is black (true) or white (false).
 */
Queen::Queen(const std::string& position, bool isBlack)
	:Piece(position, isBlack, "Queen") {}



/**
 * Creates a deep copy of the Queen piece.
 *
 * @return A unique pointer to a new Queen instance with the same properties.
 */
std::unique_ptr<Piece> Queen::clone() const
{
	return std::make_unique<Queen>(*this);
}


/**
 * Checks if the direction from the current position to the target position is valid for a Queen.
 * A Queen can move diagonally, vertically, or horizontally.
 *
 * @param targetPosition The target position to check.
 *
 * @return A boolean value:
 *         - true if the move is along a valid direction (diagonal, vertical, or horizontal),
 *         - false otherwise.
 */
bool Queen::isDirectionValid(const std::string& targetPosition) const
{
	auto [currentRow, currentCol] = positionToCoords(m_position);
	auto [targetRow, targetCol] = positionToCoords(targetPosition);
	
	bool isVertical = (currentRow == targetRow);
	bool isHorizontal = (currentCol == targetCol);
	bool isDiagonal = (std::abs(targetRow - currentRow) == std::abs(targetCol - currentCol));

	return isVertical || isHorizontal || isDiagonal;
}