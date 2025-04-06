#include "Pieces/Pawn.h"
#include "Factory/PieceFactory.h"
#include <iostream>


// Register the Pawn into the factory
bool Pawn::m_register = PieceFactory::registerPiece("Pawn",
	[](const std::string& pos, bool isBlack) {

		std::cout << "Pawn created\n";

		return std::make_unique<Pawn>(pos, isBlack);
	});


//----------------------------------------------------------
Pawn::Pawn(const std::string& position, bool isBlack)
	:Piece(position, isBlack, "Pawn") {}


bool Pawn::isMoveValid(const std::string& targetPosition) const {
	return false; // no actual move logic
}
