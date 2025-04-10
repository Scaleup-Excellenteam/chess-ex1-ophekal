#include <string>
#include <iostream>
#include "GameController.h"
#include "MoveResult.h"


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
MoveResult GameController::validateMovement(const std::string& response)
{
	std::string from = response.substr(0, 2);
	std::string target = response.substr(2, 2);

	Piece* piece = m_boardManager.getPieceAt(from);
	const Piece* targetPiece = m_boardManager.getPieceAt(target);

	if (!isValidSource(piece)) return MoveResult::NoPieceAtSource;
	if (!isMyPiece(piece)) return MoveResult::OpponentPieceAtSource;
	if (isSameColorAtTarget(piece, targetPiece)) return  MoveResult::PlayerPieceAtTarget;
	if (!canLegallyMove(piece, target)) return  MoveResult::InvalidMoveOrBlocked;
	if (doesMoveCauseSelfCheck(piece, from, target)) return MoveResult::MoveCausesCheck;

	//piece->move(target);
	//m_boardManager.movePiece(from, target);
	m_boardManager.movePiece(piece, target);
	updateIsBlackTurn(!piece->isBlack());

	bool isOpponentInCheck = isKingInCheck(!piece->isBlack());
	return isOpponentInCheck ? MoveResult::ValidMoveCausesCheck : MoveResult::ValidMove;
}


// checks if there's a piece in the specified position
bool GameController::isValidSource(Piece* piece) {
	if (!piece) {
		return false;
	}
	return true;
}


// checking if trying to move opponents player
bool GameController::isMyPiece(Piece* piece) {
	if (isCurrentPlayerBlack() != piece->isBlack()) {
		return false;
	}
	return true;
}


// Check if the piece can move to the target position
bool GameController::canLegallyMove(Piece* piece, const std::string& target) {

	return m_movementValidator.isMoveLegal(piece, target, m_boardManager.getBoard());
	
	/*
	* 	if (!piece->isMoveValid(target, m_boardManager.getBoard())) {
		return false;
	}
	return true;
	*/

}


// Check if the piece is trying to move to a target piece where it's the same color
bool GameController::isSameColorAtTarget(Piece* piece, const Piece* targetPiece) {
	
	if (targetPiece && targetPiece->isBlack() == piece->isBlack()) {
		//m_returnCode = MoveResult::PlayerPieceAtTarget;
		return true;	// same color therefore an illegal move
	}
	return false;
}


// checks if the after moving the player the board is in a state that allows a check against the player
bool GameController::doesMoveCauseSelfCheck(Piece* piece, const std::string& from, const std::string& to) {

	Piece* pieceInTargetPosition = m_boardManager.getPieceAt(to);

	m_boardManager.movePiece(piece, to);
	bool isInSelfCheck = isKingInCheck(piece->isBlack());
	m_boardManager.movePiece(piece, from);
	
	if (pieceInTargetPosition != nullptr) {
		m_boardManager.movePiece(pieceInTargetPosition, to);
	}

	return isInSelfCheck;
}


// checks wheter a specified player's king is under threat
bool GameController::isKingInCheck(bool kingColor) {
	
	std::string kingPosition = m_boardManager.findKingPosition(kingColor);
	
	// null - no king exists in this color
	if (kingPosition == "") {
		std::cout << "no king in this color" << std::endl;
		return false;
	}

	if (m_boardManager.IsIfOpponentPiecesThreatning(kingColor, kingPosition)) {
		return true;
	}
	return false;
}





/*

// returns the return code value
MoveResult GameController::getLastReturnCode() const
{
	return m_returnCode;
}
*/

/*
* // Validates and applies a move command
bool GameController::isMovementValid(const std::string& response)
{
	std::string from = response.substr(0, 2);
	std::string target = response.substr(2, 2);

	Piece* piece = m_boardManager.getPieceAt(from);
	const Piece* targetPiece = m_boardManager.getPieceAt(target);

	if (!isValidSource(piece)) return false;
	if (!isCorrectTurn(piece)) return false;
	if (isSameColorAtTarget(piece, targetPiece)) return false;
	if (!canLegallyMove(piece, target)) return false;
	if (doesMoveCauseSelfCheck(piece, from, target)) return false;

	piece->move(target);
	m_boardManager.movePiece(from, target);
	updateIsBlackTurn(!piece->isBlack());

	bool isOpponentInCheck = isKingInCheck(!piece->isBlack());
	m_returnCode = isOpponentInCheck ? MoveResult::ValidMoveCausesCheck : MoveResult::ValidMove;

	return true;

}
*/

// ---------- old code -----------------
/*
if (!piece) {
		m_returnCode = MoveResult::NoPieceAtSource;
		return false;
	}

	// It’s not the correct player's turn
	if (isCurrentPlayerBlack() != piece->isBlack()) {
		m_returnCode = MoveResult::OpponentPieceAtSource;
		return false;
	}
	// Check if the piece can move to the target position
	if (!piece->isMoveValid(target)) {
		m_returnCode = MoveResult::InvalidMoveOrBlocked;
		return false;
	}
	// Check if the piece is trying to move to a target piece where it's the same color
	if (targetPiece && targetPiece->isBlack() == piece->isBlack()) {
		m_returnCode = MoveResult::PlayerPieceAtTarget;
		return false;
	}
	// Check if the move causes self-check
	if (doesMoveCauseSelfCheck(piece, from, target)) {
		m_returnCode = MoveResult::MoveCausesCheck;
		return false;
	}
*/