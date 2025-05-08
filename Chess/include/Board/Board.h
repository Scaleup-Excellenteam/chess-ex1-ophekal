#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "Factory/PieceFactory.h"

class Board
{
public:
	Board(const std::string& boardString);

	Piece* getPieceAt(const std::string& position) const;
	std::string findKingPosition(bool isBlack) const;
	void movePiece(Piece* from, const std::string& to);
	Piece* removePieceAt(const std::string& position);
	void placePiece(Piece* piece, const std::string& position);
	const std::unordered_map<std::string, std::unique_ptr<Piece>>& getBoard() const;

private:
	std::unordered_map<std::string, std::unique_ptr<Piece>> m_board;

	std::string indexToPosition(int index) const;
	std::string charToPieceName(char symbol) const;
};