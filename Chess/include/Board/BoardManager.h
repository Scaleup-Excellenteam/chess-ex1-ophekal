#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "Factory/PieceFactory.h"
#include "Board/Board.h"

class BoardManager
{
public:
	BoardManager(const std::string& boardString);

	Piece* getPieceAt(const std::string& position) const;
	void movePiece(Piece* from, const std::string& to);
	Piece* removePieceAt(const std::string& position);
	void placePiece(Piece* piece, const std::string& position);

	const std::unordered_map<std::string, std::unique_ptr<Piece>>& getBoard() const;

private:
	Board m_board;
};