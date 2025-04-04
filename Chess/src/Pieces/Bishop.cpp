#include "Pieces/Bishop.h"
#include "Factory/PieceFactory.h"
#include <iostream>


// Register the Bishop into the factory
bool Bishop::m_register = PieceFactory::registerPiece("Bishop",
	[](const std::string& pos, bool isBlack) {

		std::cout << "Bishop created\n";

		return std::make_unique<Bishop>(pos, isBlack);
	});


//----------------------------------------------------------

Bishop::Bishop(const std::string& position, bool isBlack)
	:Piece(position, isBlack, "Bishop") {}


bool Bishop::isMoveValid(const std::string& targetPosition) const {
	std::cout << "check here if Bishops's movement is valid\n";

	// Placeholder for the logic 

	return true;
}
