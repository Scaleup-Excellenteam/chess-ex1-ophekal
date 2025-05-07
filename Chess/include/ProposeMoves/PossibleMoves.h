#pragma once

#include <string>
#include <vector>
#include "ProposeMoves/PossibleMovement.h"
#include "MovementValidator.h"
#include "Board/BoardManager.h"
#include "PriorityQueue.h"


class PossibleMoves
{
public:
	PossibleMoves(const std::string& boardString, const bool& blackTurn, const MovementValidator& movementValidator);
	void findPossibleMoves();
	void setCurrentPlayersTurn();



private:
	BoardManager m_boardManager;				// thats how will access board
	bool m_isBlackTurn;							// thats how will know who's turn it is
	MovementValidator m_movementValidator;		// that's how will validate the possible move
	PriorityQueue<PossibleMovement> m_bestMoves;	// that's the pq we will keep the best moves for the player!!
};