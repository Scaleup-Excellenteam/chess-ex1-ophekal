#include "Pieces/Rook.h"
#include <iostream>


Rook::Rook(const string& position, bool isBlack)
	:Piece(position, isBlack, "Rook") {}


bool Rook::isMoveValid(const string& targetPosition) {
	std::cout << "check here if Rook's movement is valid\n";
	
	// Placeholder for the logic 

	return true;
}
