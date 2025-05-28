#pragma once

#include <string>
#include "Board/Board.h"
#include "MoveResult.h"
#include "MovementValidator.h"
#include "ProposeMoves/PossibleMoves.h"

class GameController
{
public:
	GameController(const std::string& boardString, int wantedDepth);
	MoveResult validateMovement(const std::string& response);
	PriorityQueue<PossibleMovement> recommendMoves();
	std::string formatRecommendations(const PriorityQueue<PossibleMovement>& moves);


private:
	Board m_board;
	bool m_isBlackTurn;
	MovementValidator m_movementValidator;
	int m_depth;
	PossibleMoves m_recommendMoves;
	
	void updateIsBlackTurn(bool isBlackTurn);
	bool isCurrentPlayerBlack () const;
	bool isKingInCheck(bool isBlack) const;
	bool isValidSource(Piece* piece) const;
	bool isMyPiece(Piece* piece) const;
	bool canLegallyMove(Piece* piece, const std::string& target);
	bool isSameColorAtTarget(Piece* piece, const Piece* targetPiece) const;
	bool doesMoveCauseSelfCheck(Piece* piece, const std::string& from, const std::string& to);
};