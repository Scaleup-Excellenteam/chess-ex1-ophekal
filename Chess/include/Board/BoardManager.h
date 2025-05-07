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
	BoardManager(const BoardManager& other);
	virtual ~BoardManager() = default;
	Piece* getPieceAt(const std::string& position) const;
	std::string findKingPosition(bool isBlack) const;
	void movePiece(Piece* from, const std::string& to);
	Piece* removePieceAt(const std::string& position);
	void placePiece(Piece* piece, const std::string& position);
	bool IsIfOpponentPiecesThreatning(bool kingColor, std::string targetPosition) const;
	const std::unordered_map<std::string, std::unique_ptr<Piece>>& getBoard() const;

private:
	std::unordered_map<std::string, std::unique_ptr<Piece>> m_board;
	Board m_board;
	std::string indexToPosition(int index) const;
	std::string charToPieceName(char symbol) const;
};