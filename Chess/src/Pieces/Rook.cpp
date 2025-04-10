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


// Only checks if direction is valid (horizontal or vertical)
bool Rook::isDirectionValid(const std::string& targetPosition) const
{
	auto [currentRow, currentCol] = positionToCoords(m_position);
	auto [targetRow, targetCol] = positionToCoords(targetPosition);

	// checks if the move is neither horizontal nor vertical
	if (currentRow != targetRow && currentCol != targetCol) {
		return false;
	}

	return true;
}


/*

bool Rook::isMoveValid(const std::string& targetPosition, const BoardMap& board) const {
	
	std::cout << "check here if Rook's movement is valid\n";
	
	// checking if there are any pieces in the way
	if (currentRow == targetRow) {
		// since the move is horizontal only the col changes, therefore set the start and end col the piece moved through
		int start = std::min(currentCol, targetCol) +1;
		int end = std::max(currentCol, targetCol);

		for (int col = start; col < end; col++) {
			std::string newPosition = std::string(1, 'a' + currentRow) + std::string(1, '1' + col);
			
			if (board.find(newPosition) != board.end()) {
				// means there's a piece in the way
				return false;
			}
		}
	}
	else {
		// the movement is vertical
		int start = std::min(currentRow, targetRow) + 1;
		int end = std::max(currentRow, targetRow);

		for (int row = start; row < end; row++) {
			std::string positionToCheck = std::string(1, 'a' + row) + std::string(1, '1' + currentCol);

			if (board.find(positionToCheck) != board.end()) {
				// means there's a piece in the way
				return false;
			}
		}
	}

	return true;
}


*/
