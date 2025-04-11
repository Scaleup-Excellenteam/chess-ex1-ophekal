#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "Factory/PieceFactory.h"

class BoardManager
{
public:
	BoardManager(const std::string& boardString);
	virtual ~BoardManager() = default;
	std::string charToPieceName(char symbol) const;
	std::string indexToPosition(int index) const;
	Piece* getPieceAt(const std::string& position) const;
	std::string findKingPosition(bool isBlack) const;
	void movePiece(Piece* from, const std::string& to);
	Piece* removePieceAt(const std::string& position);
	void placePiece(Piece* piece, const std::string& position);
	bool IsIfOpponentPiecesThreatning(bool kingColor, std::string targetPosition);
	const std::unordered_map<std::string, std::unique_ptr<Piece>>& getBoard() const;


protected:
	std::unordered_map<std::string, std::unique_ptr<Piece>> m_board;       // The polymorphic mapping of the board

};