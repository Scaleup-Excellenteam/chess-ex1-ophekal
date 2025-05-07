#include "ProposeMoves/PossibleMoves.h"
#include "PriorityQueue.h"


PossibleMoves::PossibleMoves(const std::string& boardString, const bool& blackTurn, const MovementValidator& movementValidator)
    :m_boardManager(boardString), m_isBlackTurn(blackTurn), m_movementValidator(movementValidator) {}


void PossibleMoves::findPossibleMoves() {

	const auto& board = m_boardManager.getBoard();

	for (const auto& [position, piece] : board)
	{
		if (piece && piece->isBlack() == m_isBlackTurn)
		{

			// Loop over all possible board positions
			for (char row = 'a'; row <= 'h'; row++) {
				for (char col = '1'; col <= '8'; col++) {

					std::string targetPos = { row, col };

					if (targetPos == position) {
						continue;
					}

					// check movement validaty
					if (m_movementValidator.isMoveLegal(piece.get(), targetPos, board)) {

						// Create a possible movement
						PossibleMovement movement;
						movement.setFrom(position);
						movement.setDestination(targetPos);
						// i need to change to the real logic behind it- didnt understand it yet
						movement.setScore(1);

						m_bestMoves.push(movement);
					}
				}
			}
		}
	}
}



void PossibleMoves::setCurrentPlayersTurn() {
    m_isBlackTurn = !m_isBlackTurn;
}

