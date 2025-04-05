#include "Pieces/Knight.h"
#include "Factory/PieceFactory.h"


// Register the Pawn into the factory
bool Knight::m_register = PieceFactory::registerPiece("Knight",
	[](const std::string& pos, bool isBlack) {

		std::cout << "Knight created\n";

		return std::make_unique<Knight>(pos, isBlack);
	});


//----------------------------------------------------------
Knight::Knight(const std::string& position, bool isBlack)
	:Piece(position, isBlack, "Knight") {}


bool Knight::isMoveValid(const std::string& targetPosition) const {
	return false; // no actual move logic
}
