#include "BoardManager.h"
#include "MovementValidator.h"
#include <cctype>
#include <iostream>
#include <memory>

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

	std::cout<<"BoardManager: polymorphic board created"<<std::endl;
	for (const auto& pair : m_board) {
		std::cout << pair.first << ": " << pair.second->getName() << (pair.second->isBlack() ? " (Black)" : " (White)") << std::endl;
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

// Converts string board index to chess position
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

std::string BoardManager::findKingPosition(bool isBlack) const
{
	for (const auto& [position, piece] : m_board)
	{
		if (piece && piece->getName() == "King" && piece->isBlack() == isBlack)
		{
			return position;
		}
	}
	return "";
}


// Iterate over the board and check for opponent's pieces
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


// Updates the piece location on the board
void BoardManager::movePiece(Piece* piece, const std::string& to) {

	std::string from = piece->getPosition();
	m_board[to] = std::move(m_board[from]);
	m_board.erase(from);
	piece->move(to);

}

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

void BoardManager::placePiece(Piece* piece, const std::string& position){
	if (piece) {
		piece->move(position);
		m_board[position] = std::unique_ptr<Piece>(piece);
	}
}

// getter for board
const std::unordered_map<std::string, std::unique_ptr<Piece>>& BoardManager::getBoard() const {
	return m_board;
}
