#include <string>
#include <iostream>
#include <sstream>
#include "GameController.h"
#include "MoveResult.h"


/**
 * Constructs a new GameController with the given board layout string.
 * White starts the game by default (m_isBlackTurn = false).
 *
 * @param boardString A linear board representation used to initialize the game.
 */
GameController::GameController (const std::string& boardString, int wantedDepth)
	: m_board(boardString), m_isBlackTurn(false), m_depth(wantedDepth), m_recommendMoves(m_movementValidator) {}


/**
 * Checks if it's currently black's turn.
 *
 * @return True if it's black's turn; otherwise, false.
 */
bool GameController::isCurrentPlayerBlack() const
{
	return m_isBlackTurn;
}


/**
 * Sets the current turn to black or white.
 *
 * @param isBlackTurn True if it's black's turn, false for white.
 */
void GameController::updateIsBlackTurn(bool isBlackTurn) {
	m_isBlackTurn = isBlackTurn;
}


/**
 * Validates and performs a move based on the user's input command.
 *
 * @param response A string containing a move (e.g., "e2e4").
 * @return A MoveResult enum value indicating the outcome of the move.
 */
MoveResult GameController::validateMovement(const std::string& response)
{
	std::string from = response.substr(0, 2);
	std::string target = response.substr(2, 2);

	Piece* piece = m_board.getPieceAt(from);
	const Piece* targetPiece = m_board.getPieceAt(target);

	if (!isValidSource(piece)) return MoveResult::NoPieceAtSource;
	if (!isMyPiece(piece)) return MoveResult::OpponentPieceAtSource;
	if (isSameColorAtTarget(piece, targetPiece)) return  MoveResult::PlayerPieceAtTarget;
	if (!canLegallyMove(piece, target)) return  MoveResult::InvalidMoveOrBlocked;
	if (doesMoveCauseSelfCheck(piece, from, target)) return MoveResult::MoveCausesCheck;

	m_board.movePiece(piece, target);
	updateIsBlackTurn(!piece->isBlack());

	bool isOpponentInCheck = isKingInCheck(!piece->isBlack());
	return isOpponentInCheck ? MoveResult::ValidMoveCausesCheck : MoveResult::ValidMove;
}


/**
 * Determines if a source square has a piece.
 *
 * @param piece Pointer to the piece at the source position.
 * @return True if a piece exists; otherwise, false.
 */
bool GameController::isValidSource(Piece* piece) const {
	if (!piece) {
		return false;
	}
	return true;
}


/**
 * Checks whether the piece belongs to the current player.
 *
 * @param piece Pointer to the piece being moved.
 * @return True if the piece belongs to the current player; otherwise, false.
 */
bool GameController::isMyPiece(Piece* piece) const {
	if (isCurrentPlayerBlack() != piece->isBlack()) {
		return false;
	}
	return true;
}


/**
 * Validates if the move is legal for the given piece type and path.
 *
 * @param piece Pointer to the piece being moved.
 * @param target The destination square.
 * @return True if the move is valid; otherwise, false.
 */
bool GameController::canLegallyMove(Piece* piece, const std::string& target) {

	return m_movementValidator.isMoveLegal(piece, target, m_board.getBoard());
}


/**
 * Determines if a piece at the destination square is the same color as the moving piece.
 *
 * @param piece Pointer to the piece being moved.
 * @param targetPiece Pointer to the piece at the destination square.
 * @return True if both pieces are of the same color; otherwise, false.
 */
bool GameController::isSameColorAtTarget(Piece* piece, const Piece* targetPiece) const {
	
	return (targetPiece && targetPiece->isBlack() == piece->isBlack());
}


/**
 * Simulates the move to check if it would leave the player's own king in check.
 *
 * @param piece Pointer to the piece to move.
 * @param from Original position of the piece.
 * @param to Target position of the piece.
 * @return True if the move would result in a check against the player; otherwise, false.
 */
bool GameController::doesMoveCauseSelfCheck(Piece* piece, const std::string& from, const std::string& to) {

	Piece* capturedPiece = m_board.removePieceAt(to);
	m_board.movePiece(piece, to);

	bool isInSelfCheck = isKingInCheck(piece->isBlack());

	// Undo move
	m_board.movePiece(piece, from);
	if (capturedPiece != nullptr){
		m_board.placePiece(capturedPiece, to); // Restore captured piece
	}
	return isInSelfCheck;
}


/**
 * Checks if the specified king is currently in check.
 *
 * @param kingColor True if checking the black king, false for white king.
 * @return True if the king is in check; otherwise, false.
 */
bool GameController::isKingInCheck(bool kingColor) const {
	
	std::string kingPosition = m_board.findKingPosition(kingColor);
	return m_movementValidator.isKingInCheck(kingColor, kingPosition, m_board.getBoard());
}


/**
 * Generates and returns recommended moves for the current player.
 *
 * @return A priority queue containing the best possible moves.
 */
PriorityQueue<PossibleMovement> GameController::recommendMoves() {
	
	m_recommendMoves.findPossibleMoves(m_depth, m_isBlackTurn, m_board);
	return m_recommendMoves.getBestMoves();
}


/**
 * Formats the recommended moves into a readable string format.
 *
 * @param moves The priority queue of moves to format.
 * @return A formatted string representation of the moves.
 */
std::string GameController::formatRecommendations(const PriorityQueue<PossibleMovement>& moves) {
	
	std::ostringstream out;
	out << moves;
	return out.str();
}
