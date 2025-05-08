#include "Board/BoardManager.h"
#include "MovementValidator.h"
#include <cctype>
#include <iostream>
#include <memory>


/**
 * Constructs the board using the Board constructor
 *
 * @param boardString A linear string representation of the board (e.g., "rnbqkbnrpp...").
 */
BoardManager::BoardManager(const std::string& boardString)
	:m_board(boardString) {}
	

 /**
  * Retrieves the piece at a specific board position.
  *
  * @param position The position to check.
  * @return A raw pointer to the piece at that position, or nullptr if empty.
  */
 Piece* BoardManager::getPieceAt(const std::string& position) const {

	
	auto it = m_board.getPieces().find(position);
	if (it == m_board.getPieces().end()) {
		return nullptr;
	}
	return it->second.get();
}


 /**
  * Finds the position of the King for a specific color.
  *
  * @param isBlack True if searching for the black king, false for white.
  * @return The position of the king, or an empty string if not found.
  */
 std::string BoardManager::findKingPosition(bool isBlack) const {
	for (const auto& [position, piece] : m_board.getPieces())
	{
		if (piece && piece->getName() == "King" && piece->isBlack() == isBlack)
		{
			return position;
		}
	}

	// if the king isn't on the board
	return "";
}


 /**
  * Checks whether any opposing pieces are threatening a given position.
  *
  * @param kingColor The color of the king being threatened (true = black, false = white).
  * @param targetPosition The position to check.
  * @return True if any opponent pieces threaten the king if piece is moved to targetPosition, false otherwise.
  */
 bool BoardManager::IsIfOpponentPiecesThreatning(bool kingColor, std::string targetPosition) const {

	MovementValidator validator;
	std::unordered_map<std::string, std::unique_ptr<Piece>> board = m_board.getPieces();


	for (const auto& [position, piece] : board) {

		if (piece && piece->isBlack() != kingColor) {
			if (piece->isDirectionValid(targetPosition)) {
				if (validator.isMoveLegal(piece.get(), targetPosition, m_board.getPieces())) {
					return true;
				}
			}
		}
	}
	return false;
}


 /**
  * Moves a piece from its current position to the specified destination.
  *
  * @param piece Pointer to the piece to be moved.
  * @param to The destination position.
  */
 void BoardManager::movePiece(Piece* piece, const std::string& to) {

	 std::unordered_map<std::string, std::unique_ptr<Piece>> board = m_board.getPieces();

	std::string from = piece->getPosition();
	board[to] = std::move(board[from]);
	board.erase(from);
	piece->move(to);

}


 /**
  * Removes a piece from the specified position on the board.
  *
  * @param position The position to remove the piece from.
  * @return A raw pointer to the removed piece, or nullptr if no piece was present.
  */
 Piece* BoardManager::removePieceAt(const std::string& position) {

	std::unordered_map<std::string, std::unique_ptr<Piece>> board = m_board.getPieces();

	auto it = board.find(position);
	if (it != board.end()) {
		Piece* rawPointer = it->second.release();
		board.erase(it);
		return rawPointer;
	}
	return nullptr;
}


 /**
  * Places a given piece on the board at the specified position.
  *
  * @param piece Pointer to the piece to place.
  * @param position The position to place the piece at.
  */
 void BoardManager::placePiece(Piece* piece, const std::string& position){
	
	 std::unordered_map<std::string, std::unique_ptr<Piece>> board = m_board.getPieces();

	if (piece) {
		piece->move(position);
		board[position] = std::unique_ptr<Piece>(piece);
	}
}


 /**
 * Returns the current state of the board.
 *
 * @return A constant reference to the board map.
 */
 const std::unordered_map<std::string, std::unique_ptr<Piece>>& BoardManager::getBoard() const {
	return m_board;
}


 /**
 * Simulates the move to check if it would leave the player's own king in check.
 *
 * @param piece Pointer to the piece to move.
 * @param from Original position of the piece.
 * @param to Target position of the piece.
 * @return True if the move would result in a check against the player; otherwise, false.
 */
 bool BoardManager::doesMoveCauseSelfCheck(Piece* piece, const std::string& from, const std::string& to) {

	 Piece* capturedPiece = removePieceAt(to);
	 movePiece(piece, to);

	 bool isInSelfCheck = isKingInCheck(piece->isBlack());

	 // Undo move
	 movePiece(piece, from);
	 if (capturedPiece != nullptr) {
		 placePiece(capturedPiece, to); // Restore captured piece
	 }
	 return isInSelfCheck;
 }



 /**
  * Determines whether the specified player's king is under threat.
  *
  * @param kingColor True for black king, false for white king.
  * @return True if the king is in check; otherwise, false.
  */
 bool BoardManager::isKingInCheck(bool kingColor) const {

	 std::string kingPosition = findKingPosition(kingColor);

	 //add here exception!!!!
	 // null - no king exists in this color
	 if (kingPosition == "") {
		 std::cout << "no king in this color" << std::endl;
		 return false;
	 }

	 return IsIfOpponentPiecesThreatning(kingColor, kingPosition);
 }