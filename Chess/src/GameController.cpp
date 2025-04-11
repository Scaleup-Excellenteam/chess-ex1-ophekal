#include <string>
#include <iostream>
#include "GameController.h"
#include "MoveResult.h"


// Initializes the game controller with the board string and sets white's turn to start
GameController::GameController (const std::string& boardString)
	: m_boardManager(boardString), m_isBlackTurn(false) {}


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
}


// Check if the piece is trying to move to a target piece where it's the same color
bool GameController::isSameColorAtTarget(Piece* piece, const Piece* targetPiece) {
	
	if (targetPiece && targetPiece->isBlack() == piece->isBlack()) {
		return true;	// same color therefore an illegal move
	}
	return false;
}


/*
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
*/


// checks if the after moving the player the board is in a state that allows a check against the player
bool GameController::doesMoveCauseSelfCheck(Piece* piece, const std::string& from, const std::string& to) {

	// Temporarily remove any piece at the target and simulate move

	Piece* capturedPiece = m_boardManager.removePieceAt(to);
	m_boardManager.movePiece(piece, to);

	bool isInSelfCheck = isKingInCheck(piece->isBlack());

	// Undo move
	m_boardManager.movePiece(piece, from);
	if (capturedPiece != nullptr){
		m_boardManager.placePiece(capturedPiece, to); // Restore captured piece
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