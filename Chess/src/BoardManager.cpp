#include "BoardManager.h"
#include "MovementValidator.h"
#include <cctype>
#include <iostream>
#include <memory>


/**
 * Constructs the board from a string representation where each character represents a piece,
 * with '#' representing an empty square.
 *
 * @param boardString A linear string representation of the board (e.g., "rnbqkbnrpp...").
 */
BoardManager::BoardManager(const std::string& boardString){

	for (size_t i = 0; i < boardString.size(); ++i)
	{
		char symbol = boardString[i];
		if (symbol == '#') {
			continue;
		}

		std::string pieceName = charToPieceName(symbol);
		bool isBlack = std::islower(symbol);
		std::string position = indexToPosition(i);

		m_board[position] = PieceFactory::createPiece(pieceName, position, isBlack);
	}
}


 /**
  * Converts a character to the corresponding piece name.
  *
  * @param symbol A character representing the piece.
  * @return A string representing the piece type.
  */
 std::string BoardManager::charToPieceName(char symbol) const {

	switch (std::tolower(symbol))
	{
		case 'r': return "Rook";
		case 'n': return "Knight";
		case 'b': return "Bishop";
		case 'k': return "King";
		case 'q': return "Queen";
		case 'p': return "Pawn";
		default: return "";
	}
}


 /**
  * Converts a linear board index to standard chess position notation.
  *
  * @param index The index in the board string.
  * @return A string representing the position in algebraic notation.
  */
 std::string BoardManager::indexToPosition(int index) const {

	char row = 'a' + index / 8;
	char col = '1' + index % 8;
	return { row, col };
}


 /**
  * Retrieves the piece at a specific board position.
  *
  * @param position The position to check.
  * @return A raw pointer to the piece at that position, or nullptr if empty.
  */
 Piece* BoardManager::getPieceAt(const std::string& position) const {

	auto it = m_board.find(position);
	if (it == m_board.end()) {
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
 std::string BoardManager::findKingPosition(bool isBlack) const
{
	for (const auto& [position, piece] : m_board)
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
 bool BoardManager::IsIfOpponentPiecesThreatning(bool kingColor, std::string targetPosition) {

	MovementValidator validator;

	for (const auto& [position, piece] : m_board) {

		if (piece && piece->isBlack() != kingColor) {
			if (piece->isDirectionValid(targetPosition)) {
				if (validator.isMoveLegal(piece.get(), targetPosition, m_board)) {
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

	std::string from = piece->getPosition();
	m_board[to] = std::move(m_board[from]);
	m_board.erase(from);
	piece->move(to);

}


 /**
  * Removes a piece from the specified position on the board.
  *
  * @param position The position to remove the piece from.
  * @return A raw pointer to the removed piece, or nullptr if no piece was present.
  */
 Piece* BoardManager::removePieceAt(const std::string& position)
{
	auto it = m_board.find(position);
	if (it != m_board.end()) {
		Piece* rawPointer = it->second.release();
		m_board.erase(it);
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
	if (piece) {
		piece->move(position);
		m_board[position] = std::unique_ptr<Piece>(piece);
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
