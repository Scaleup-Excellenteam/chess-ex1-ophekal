#include "ProposeMoves/PossibleMoves.h"
#include "PriorityQueue.h"


PossibleMoves::PossibleMoves(const std::string& boardString, const bool& blackTurn, const MovementValidator& movementValidator)
    :m_board(boardString), m_isBlackTurn(blackTurn), m_movementValidator(movementValidator) {}


void PossibleMoves::findPossibleMoves(int numOfTurns) {

	for (const auto& [position, piece] : m_board.getBoard())
	{
		if (piece && piece->isBlack() == m_isBlackTurn)
		{
			checkAllPossibleMovesForPiece(piece, numOfTurns);
		}
	}
}


// given a specific piece, we will simulate all possible moves for him as long as target isnt current position
void PossibleMoves::checkAllPossibleMovesForPiece(const std::unique_ptr<Piece>& piece, int numOfTurns) {

	// Loop over all possible board positions
	for (char row = 'a'; row <= 'h'; row++) {
		for (char col = '1'; col <= '8'; col++) {
			
			// the position we will move the player to and continue the depth simulation from
			std::string targetPos = { row, col };

			if (targetPos == piece->getPosition()) {
				continue;
			}

			// create a board for simulating the move for each piece
			Board simulationBoard(m_board);

			// if the position is not
			// start simulatation according to the users desirable depth
			for (int i = 0; i < numOfTurns; i++) {

				// if move isn't legal no need to move player and no need to simulate more turns! cause the first one isn't legal
				if (!isMoveLegal(piece, targetPos)) {
					break;
				}

				executeTurn(piece, targetPos, simulationBoard);
				switchCurrentPlayersTurn();

			}
		}
	}
}


// this function cheks if the movement is even legal
bool PossibleMoves::isMoveLegal(const std::unique_ptr<Piece>& piece, const std::string targetPos) {

	return m_movementValidator.isMoveLegal(piece.get(), targetPos, m_board.getBoard());
}


// this func is called is the move is legal and does the movement
void PossibleMoves::executeTurn(const std::unique_ptr<Piece>& piece, const std::string& targPos, Board& simulationBoard) {


	simulationBoard.movePiece(piece->getPosition(), targPos);
	
	// Create a possible movement
	PossibleMovement movement;
	movement.setFrom(piece->getPosition());
	movement.setDestination(targPos);

	// i need to change to the real logic behind it- didnt understand it yet
	movement.setScore(1);

	m_bestMoves.push(movement);

}




void PossibleMoves::switchCurrentPlayersTurn() {
    m_isBlackTurn = !m_isBlackTurn;
}


























//-------------------------------------------------------

void PossibleMoves::findPossibleMoves(int numOfTurns) {

	for (const auto& [position, piece] : m_board.getBoard())
	{
		// per each piece create a simulation board
		Board simulatedBoard(m_board);


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

						//m_bestMoves.push(movement);
					}
				}
			}
		}
	}
}



void PossibleMoves::switchCurrentPlayersTurn() {
	m_isBlackTurn = !m_isBlackTurn;
}

