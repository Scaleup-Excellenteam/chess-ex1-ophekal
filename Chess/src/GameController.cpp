#include <string>
#include "GameController.h"
#include <iostream>

// Initializes the game controller with the board string and sets white's turn to start
GameController::GameController (const std::string& boardString)
	: m_boardManager(boardString), m_isBlackTurn(false) {

	std::cout<<"controller is created"<<std::endl;
}


// Returns true if it's currently black's turn, false otherwise
bool GameController::isCurrentPlayerBlack() const
{
	return m_isBlackTurn;
}

// Sets the current turn to black or white
void GameController::updateIsBlackTurn(bool isBlackTurn) {
	m_isBlackTurn = isBlackTurn;
}

// Validates and applies a move command
// Returns true if the move is valid and executed, false otherwise
bool GameController::isMovemenetValid(const std::string& response)
{
	std::string from = response.substr(0, 2);
	std::string target = response.substr(2, 2);

	Piece* piece = m_boardManager.getPieceAt(from);

	// Invalid if no piece at 'from' or if it’s not the correct player's turn
	if (!piece || isCurrentPlayerBlack() != piece->isBlack()) {
		// TODO: Save a specific error code/message for UI feedback (e.g., "Invalid turn" or "No piece")
		return false;
	}

	// Check if the piece can move to the target position
	if (!piece->isMoveValid(target)) {
		// TODO: Save a specific error code/message for invalid move
		//maybe save response code as a member so it'll be access able through the main!
		return false;
	}
	
	piece->move(target);
	m_boardManager.movePiece(from, target);
			
	// Switch turns
	updateIsBlackTurn(piece->isBlack() ? false : true);
		
	return true;
}
