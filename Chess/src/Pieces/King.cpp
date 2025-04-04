#include "Pieces/King.h"
#include "Factory/PieceFactory.h"
#include <iostream>


// Register the King into the factory
bool King::m_register = PieceFactory::registerPiece("King",
	[](const std::string& pos, bool isBlack) {

		std::cout << "King created\n";

		return std::make_unique<King>(pos, isBlack);
	});


//----------------------------------------------------------

King::King(const std::string& position, bool isBlack)
	:Piece(position, isBlack, "King") {}


bool King::isMoveValid(const std::string& targetPosition) const {
	std::cout << "check here if King's movement is valid\n";

	// Placeholder for the logic 

	return true;
}
