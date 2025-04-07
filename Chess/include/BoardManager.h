#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "Factory/PieceFactory.h"
//#include <Pieces/Piece.h>

class BoardManager
{
public:
	BoardManager(const std::string& boardString);
	virtual ~BoardManager() = default;
	std::string charToPieceName(char symbol) const;
	std::string indexToPosition(int index) const;
	Piece* getPieceAt(const std::string& position) const;
	std::string findKingPosition(bool isBlack) const;
	void movePiece(const std::string& from, const std::string& to);
	bool IsIfOpponentPiecesThreatning(bool isBlack, std::string targetPosition);

	//void setPieceAt(const std::string& pos, Piece* piece);


protected:
	std::unordered_map<std::string, std::unique_ptr<Piece>> m_board;       // The polymorphic mapping of the board

};