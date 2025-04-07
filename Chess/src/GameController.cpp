#include <string>
#include <iostream>
#include "GameController.h"
#include "MoveResult.h"


// Initializes the game controller with the board string and sets white's turn to start
GameController::GameController (const std::string& boardString)
	: m_boardManager(boardString), m_isBlackTurn(false), m_returnCode(MoveResult::ValidMove) {

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
bool GameController::isMovementValid(const std::string& response)
{
	std::string from = response.substr(0, 2);
	std::string target = response.substr(2, 2);

	Piece* piece = m_boardManager.getPieceAt(from);
	const Piece* targetPiece = m_boardManager.getPieceAt(target);

	if (!isValidSource(piece)) return false;
	if (!isCorrectTurn(piece)) return false;
	if (!canLegallyMove(piece, target)) return false;
	if (isSameColorAtTarget(piece, targetPiece)) return false;
	if (doesMoveCauseSelfCheck(piece, from, target)) return false;
	
	piece->move(target);
	m_boardManager.movePiece(from, target);
	updateIsBlackTurn(! piece->isBlack());
	
	bool isOpponentInCheck = isKingInCheck(!piece->isBlack());
	m_returnCode = isOpponentInCheck ? MoveResult::ValidMoveCausesCheck : MoveResult::ValidMove;
	
	return true;
	
}

// checks if there's a piece in the specified position
bool GameController::isValidSource(Piece* piece) {
	if (!piece) {
		m_returnCode = MoveResult::NoPieceAtSource;
		return false;
	}
	return true;
}


// It’s not the correct player's turn
bool GameController::isCorrectTurn(Piece* piece) {
	if (isCurrentPlayerBlack() != piece->isBlack()) {
		m_returnCode = MoveResult::OpponentPieceAtSource;
		return false;
	}
	return true;
}


// Check if the piece can move to the target position
bool GameController::canLegallyMove(Piece* piece, const std::string& target) {
	if (!piece->isMoveValid(target)) {
		m_returnCode = MoveResult::InvalidMoveOrBlocked;
		return false;
	}
	return true;
}


// Check if the piece is trying to move to a target piece where it's the same color
bool GameController::isSameColorAtTarget(Piece* piece, const Piece* targetPiece) {
	if (targetPiece && targetPiece->isBlack() == piece->isBlack()) {
		m_returnCode = MoveResult::PlayerPieceAtTarget;
		return false;
	}
	return true;
}


// checks if the after moving the player the board is in a state that allows a check against the player
bool GameController::doesMoveCauseSelfCheck(Piece* piece, const std::string& from, const std::string& to) {
		
	m_boardManager.movePiece(from, to);
	bool isSelfInCheck = isKingInCheck(piece->isBlack());
	m_boardManager.movePiece(to, from);
	return isSelfInCheck;
}


// checks wheter a specified player's king is under threat
bool GameController::isKingInCheck(bool isBlack) {
	
	std::string kingPosition = m_boardManager.findKingPosition(isBlack);

	if (m_boardManager.IsIfOpponentPiecesThreatning(!isBlack, kingPosition)) {
		return true;
	}
	return false;
}


// returns the return code value
MoveResult GameController::getLastReturnCode() const
{
	return m_returnCode;
}



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