#pragma once

#include "Board/Board.h"
#include "MovementValidator.h"
#include "PriorityQueue.h"
#include "ProposeMoves/PossibleMovement.h"
#include <vector>
#include <string>

class PossibleMoves {
public:

    PossibleMoves(const MovementValidator& movementValidator);
    void findPossibleMoves(int numOfTurns, bool isBlack, const Board& board);
    const PriorityQueue<PossibleMovement>& getBestMoves() const;

	void printBestMoves() const;

private:
    bool m_recommendForBlack = false;	// the color of the player we recommend the moves
	bool m_isBlackTurn = false;			// the color of the current player in the recursion
    MovementValidator m_movementValidator;
    PriorityQueue<PossibleMovement> m_bestMoves;


    // Helper methods for the Min-Max algorithm
    int calculateMoveScore(Board& boardBefore, Board& boardAfter, const std::string& from, const std::string& to);
	int minMax(Board& board, bool isBlackTurn, int depth, int maxDepth);
    int evaluateBoard(const Board& board, bool forBlack) const;
    int getPieceValue(const Piece* piece) const;

    // Utility methods
    std::vector<std::string> allPositionsOnBoard() const;
};


/**
/#pragma once

#include <string>
#include <vector>
#include "ProposeMoves/PossibleMovement.h"
#include "MovementValidator.h"
#include "Board/Board.h"
#include "PriorityQueue.h"


class PossibleMoves
{
public:
	PossibleMoves(const std::string& boardString, const bool& blackTurn, const MovementValidator& movementValidator);
	
	void findPossibleMoves(int numOfTurns);
	void switchCurrentPlayersTurn();
	const PriorityQueue<PossibleMovement>& getBestMoves() const;

	



private:
	
	Board m_board;									
	bool m_isBlackTurn;							
	MovementValidator m_movementValidator;		
	PriorityQueue<PossibleMovement> m_bestMoves;	
	
	int evaluateBoard(const Board& board, bool forBlack);
	int simulate(Board board, bool isBlackTurn, int depth, int maxDepth);
	std::vector<std::string> allPositionsOnBoard() const;
	int calculateMoveScore(Board& boardBefore, Board& boardAfter, const std::string& from, const std::string& to);

	
};
*/