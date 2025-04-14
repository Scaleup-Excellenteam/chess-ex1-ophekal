#include "Pieces/Pawn.h"
#include "Factory/PieceFactory.h"
#include <iostream>


// Register the Pawn into the factory
bool Pawn::m_register = PieceFactory::registerPiece("Pawn",
	[](const std::string& pos, bool isBlack) {
		return std::make_unique<Pawn>(pos, isBlack);
	});


//----------------------------------------------------------
// Pawn Constructor
Pawn::Pawn(const std::string& position, bool isBlack)
	:Piece(position, isBlack, "Pawn") {}



bool Pawn::isDirectionValid(const std::string& targetPosition) const
{
	return false;
}

