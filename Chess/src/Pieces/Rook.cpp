#include "Pieces/Rook.h"
#include "Factory/PieceFactory.h"
#include <iostream>


// Register the Rook into the factory
bool Rook::m_register = PieceFactory::registerPiece("Rook",
	[](const std::string& pos, bool isBlack) {

		std::cout << "Rook created\n";

		return std::make_unique<Rook>(pos, isBlack);
	});


//----------------------------------------------------------

Rook::Rook(const std::string& position, bool isBlack)
	:Piece(position, isBlack, "Rook") {}


bool Rook::isMoveValid(const std::string& targetPosition) const {
	std::cout << "check here if Rook's movement is valid\n";
	
	// Placeholder for the logic 

	return true;
}
