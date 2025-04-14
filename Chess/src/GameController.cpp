#include <string>
#include <iostream>
#include "GameController.h"
#include "MoveResult.h"


// Initializes the game controller with a given board setup string.
// White starts the game by default.
GameController::GameController (const std::string& boardString)
	: m_boardManager(boardString), m_isBlackTurn(false) {}


// Returns true if it's currently black's turn
bool GameController::isCurrentPlayerBlack() const
{
	return m_isBlackTurn;
}


// Updates whose turn it is to play
void GameController::updateIsBlackTurn(bool isBlackTurn) {
	m_isBlackTurn = isBlackTurn;
}


// Validates and executes a move command from the user
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


// Checks if there's a piece in the specified position
bool GameController::isValidSource(Piece* piece) {
	if (!piece) {
		return false;
	}
	return true;
}


// Checks if the piece at the source belongs to the current player
bool GameController::isMyPiece(Piece* piece) {
	if (isCurrentPlayerBlack() != piece->isBlack()) {
		return false;
	}
	return true;
}


// Determines if a move is legal for a piece based on its movement rules and path clearance
bool GameController::canLegallyMove(Piece* piece, const std::string& target) {

	return m_movementValidator.isMoveLegal(piece, target, m_boardManager.getBoard());
}


// Checks if the destination position is occupied by a piece of the same color
bool GameController::isSameColorAtTarget(Piece* piece, const Piece* targetPiece) {
	
	if (targetPiece && targetPiece->isBlack() == piece->isBlack()) {
		return true;	// same color therefore an illegal move
	}
	return false;
}


// Simulates the move and checks if it would leave the player's king in check
bool GameController::doesMoveCauseSelfCheck(Piece* piece, const std::string& from, const std::string& to) {

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


// Checks whether the specified player's king is under threat
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