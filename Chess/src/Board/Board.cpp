#include "Board/Board.h"
#include "MovementValidator.h"
#include "Exceptions/StringFormatException.h"
#include <cctype>
#include <iostream>
#include <memory>


/**
 * Constructs the board from a string representation where each character represents a piece,
 * with '#' representing an empty square.
 *
 * @param boardString A linear string representation of the board (e.g., "rnbqkbnrpp...").
 */
Board::Board(const std::string& boardString) {
	
	// Check if string is the right length (64 squares)
	if (boardString.length() != 64) {
		throw StringFormatException("Expected 64 characters, got " + std::to_string(boardString.length()));
	}

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
 * Copy constructor that creates a deep copy of another board.
 *
 * @param other The board to copy from.
 */
Board::Board(const Board& other){

	for (const auto& [pos, piece] : other.m_board) {
		if (piece) {
			m_board[pos] = piece->clone();
		}
	}
}
	


 /**
  * Converts a character to the corresponding piece name.
  *
  * @param symbol A character representing the piece.
  * @return A string representing the piece type.
  */
 std::string Board::charToPieceName(char symbol) const {

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
 std::string Board::indexToPosition(int index) const {

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
 Piece* Board::getPieceAt(const std::string& position) const {

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
 std::string Board::findKingPosition(bool isBlack) const {
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
  * Moves a piece from its current position to the specified destination.
  *
  * @param piece Pointer to the piece to be moved.
  * @param to The destination position.
  */
 void Board::movePiece(Piece* piece, const std::string& to) {

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
 Piece* Board::removePieceAt(const std::string& position) {
	
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
 void Board::placePiece(Piece* piece, const std::string& position){
	
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
 const std::unordered_map<std::string, std::unique_ptr<Piece>>& Board::getBoard() const {
	return m_board;
}
