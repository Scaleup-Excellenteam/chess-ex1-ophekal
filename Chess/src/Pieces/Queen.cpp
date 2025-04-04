#include "Pieces/Queen.h"
#include "Factory/PieceFactory.h"
#include <iostream>


// Register the Queen into the factory
bool Queen::m_register = PieceFactory::registerPiece("Queen",
	[](const std::string& pos, bool isBlack) {

		std::cout << "Queen created\n";

		return std::make_unique<Queen>(pos, isBlack);
	});


//----------------------------------------------------------

Queen::Queen(const std::string& position, bool isBlack)
	:Piece(position, isBlack, "Queen") {}


bool Queen::isMoveValid(const std::string& targetPosition) const {
	std::cout << "check here if Queen's movement is valid\n";

	// Placeholder for the logic 

	return true;
}
