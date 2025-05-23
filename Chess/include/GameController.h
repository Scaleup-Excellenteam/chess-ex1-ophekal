#pragma once

#include <string>
#include "BoardManager.h"
#include "MoveResult.h"
#include "MovementValidator.h"

class GameController
{
public:
	GameController(const std::string& boardString);
	MoveResult validateMovement(const std::string& response);

private:
	BoardManager m_boardManager;
	bool m_isBlackTurn;
	MovementValidator m_movementValidator;

	void updateIsBlackTurn(bool isBlackTurn);
	bool isCurrentPlayerBlack () const;
	bool isKingInCheck(bool isBlack) const;
	bool isValidSource(Piece* piece) const;
	bool isMyPiece(Piece* piece) const;
	bool canLegallyMove(Piece* piece, const std::string& target);
	bool isSameColorAtTarget(Piece* piece, const Piece* targetPiece) const;
	bool doesMoveCauseSelfCheck(Piece* piece, const std::string& from, const std::string& to);
};