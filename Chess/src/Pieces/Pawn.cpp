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
	return false;
}

