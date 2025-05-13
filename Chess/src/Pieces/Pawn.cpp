#include "Pieces/Pawn.h"
#include "Factory/PieceFactory.h"
#include <iostream>


/**
 * Registers the Pawn piece into the PieceFactory. This enables the factory to create a Pawn
 * instance when requested by name.
 *
 * The registration is done statically, so the Pawn piece will be available for creation when
 * the program starts.
 */
bool Pawn::m_register = PieceFactory::registerPiece("Pawn",
	[](const std::string& pos, bool isBlack) {
		return std::make_unique<Pawn>(pos, isBlack);
	});


//----------------------------------------------------------

/**
 * Constructs a Pawn piece with the given position and color.
 *
 * @param position The position of the Pawn on the chessboard.
 * @param isBlack A boolean indicating whether the Pawn is black (true) or white (false).
 */
Pawn::Pawn(const std::string& position, bool isBlack)
	:Piece(position, isBlack, "Pawn") {}



/*
*/
std::unique_ptr<Piece> Pawn::clone() const
{
	return std::make_unique<Pawn>(*this);
}

int Pawn::getValue() const
{
	return 1;
}


/**
 * Checks if the direction from the current position to the target position is valid for a Pawn.
 * The logic for a Pawn’s movement is not yet implemented in this method.
 *
 * @param targetPosition The target position to check (e.g., "A3").
 *
 * @return A boolean value:
 *         - false, as the movement logic for the Pawn is not implemented in this method.
 */
bool Pawn::isDirectionValid(const std::string& targetPosition) const
{
	auto [currentRow, currentCol] = positionToCoords(m_position);
	auto [targetRow, targetCol] = positionToCoords(targetPosition);
	
	// Direction depends on pawn color, white pawns move "up", black pawns move "down"
	int forwardDirection = m_isBlack ? -1 : 1;

	// Check if moving forward
	if (currentCol == targetCol) {

		if (targetRow - currentRow == forwardDirection) {
			return true;
		}
	
		// Check if pawn is in its initial row
		// Black pawns start in row G (index 6), White pawns start in row B (index 1)
		bool isInStartingRow = m_isBlack ? (currentRow == 6) : (currentRow == 1);
	
		if (isInStartingRow && targetRow - currentRow == 2 * forwardDirection) {
			return true;
		}

		return false;
	}

	// Check for diagonal capture (one square diagonally forward)
	if (std::abs(targetCol - currentCol) == 1 && (targetRow - currentRow) == forwardDirection) {
		return true;
	}

	return false;
}

