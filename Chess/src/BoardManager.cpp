#include "BoardManager.h"
#include <cctype>

// BoardManager Constructor
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

// Converts a character symbol to the corresponding piece name for the factory
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

// Converts board string index to chess position
std::string BoardManager::indexToPosition(int index) const {

	char row = 'a' + index / 8;
	char col = '1' + index % 8;
	return { row, col };
}

// Retrieves the piece at the given position on the board
Piece* BoardManager::getPieceAt(const std::string& position) const {

	auto it = m_board.find(position);
	if (it == m_board.end()) {
		return nullptr;			// Means there's no piece at this position
	}
	return it->second.get();	// Returning the raw pointer to the piece in order to update it if needed
}

// Updates the piece location on the board
void BoardManager::movePiece(const std::string& from, const std::string& target) {

	Piece* piece = getPieceAt(from);
	if (piece) {
		piece->move(target);
		m_board[target] = std::move(m_board[from]);
		m_board.erase(from);
	}
}
