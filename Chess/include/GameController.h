#pragma once

#include <string>
#include "BoardManager.h"
#include "MoveResult.h"

class GameController
{
public:
	GameController(const std::string& boardString);
	bool isMovementValid(const std::string& response);
	MoveResult getLastReturnCode() const;

private:
	BoardManager m_boardManager;
	bool m_isBlackTurn;
	MoveResult m_returnCode;
	void updateIsBlackTurn(bool isBlackTurn);
	bool isCurrentPlayerBlack () const;
	bool isKingInCheck(bool isBlack);
	bool isValidSource(Piece* piece);
	bool isCorrectTurn(Piece* piece);
	bool canLegallyMove(Piece* piece, const std::string& target);
	bool isSameColorAtTarget(Piece* piece, const Piece* targetPiece);
	bool doesMoveCauseSelfCheck(Piece* piece, const std::string& from, const std::string& to);






};