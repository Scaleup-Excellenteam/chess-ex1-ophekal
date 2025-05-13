#include "ProposeMoves/PossibleMoves.h"
#include "PriorityQueue.h"
#include <climits>
#include <algorithm>

// Piece values
const int PAWN_VALUE = 100;
const int KNIGHT_VALUE = 320;
const int BISHOP_VALUE = 330;
const int ROOK_VALUE = 500;
const int QUEEN_VALUE = 900;
const int KING_VALUE = 20000;

// Scoring modifiers
const int THREATENED_BY_WEAKER_PENALTY = -200;
const int THREATENS_STRONGER_BONUS = 150;
const int CAPTURE_BONUS_MULTIPLIER = 10; // Bonus multiplier for capturing pieces

/**
 * Constructor for PossibleMoves
 */
PossibleMoves::PossibleMoves(const std::string& boardString, const bool& blackTurn, const MovementValidator& movementValidator)
    : m_board(boardString), m_isBlackTurn(blackTurn), m_movementValidator(movementValidator) {}

/**
 * Gets piece value based on its type
 */
int PossibleMoves::getPieceValue(const Piece* piece) const {
    if (!piece) return 0;

    const std::string& name = piece->getName();
    if (name == "Pawn") return PAWN_VALUE;
    if (name == "Knight") return KNIGHT_VALUE;
    if (name == "Bishop") return BISHOP_VALUE;
    if (name == "Rook") return ROOK_VALUE;
    if (name == "Queen") return QUEEN_VALUE;
    if (name == "King") return KING_VALUE;

    return 0;
}

/**
 * Calculates the score for a specific move by analyzing:
 * 1. Capture value
 * 2. Whether the moved piece is threatened by weaker pieces
 * 3. Whether the moved piece threatens stronger opponent pieces
 */
int PossibleMoves::calculateMoveScore(Board& boardBefore, Board& boardAfter, const std::string& from, const std::string& to) {
    auto movedPiece = boardAfter.getPieceAt(to);
    if (!movedPiece) return 0;

    auto capturedPiece = boardBefore.getPieceAt(to); // If there was a piece in the destination
    int score = 0;

    // Capture value - higher bonus for capturing valuable pieces
    if (capturedPiece && capturedPiece->isBlack() != movedPiece->isBlack()) {
        int captureValue = getPieceValue(capturedPiece);
        score += captureValue * CAPTURE_BONUS_MULTIPLIER;
    }

    // Check if threatened by weaker piece after the move
    for (const auto& [pos, enemyPiece] : boardAfter.getBoard()) {
        if (enemyPiece && enemyPiece->isBlack() != movedPiece->isBlack()) {
            if (m_movementValidator.isMoveLegal(enemyPiece.get(), to, boardAfter.getBoard())) {
                if (getPieceValue(enemyPiece.get()) < getPieceValue(movedPiece)) {
                    score += THREATENED_BY_WEAKER_PENALTY;
                }
            }
        }
    }

    // Check if threatens stronger pieces
    for (const auto& [pos, targetPiece] : boardAfter.getBoard()) {
        if (targetPiece && targetPiece->isBlack() != movedPiece->isBlack()) {
            if (m_movementValidator.isMoveLegal(movedPiece, pos, boardAfter.getBoard())) {
                if (getPieceValue(targetPiece.get()) > getPieceValue(movedPiece)) {
                    score += THREATENS_STRONGER_BONUS;
                }
            }
        }
    }

    return score;
}

/**
 * Find all possible moves for the current player and evaluate them using Min-Max algorithm
 */
void PossibleMoves::findPossibleMoves(int numOfTurns) {
    // Clear the priority queue
    while (!m_bestMoves.getQueue().empty()) {
        m_bestMoves.poll();
    }

    // For each piece of the current player
    for (const auto& [pos, piece] : m_board.getBoard()) {
        if (piece && piece->isBlack() == m_isBlackTurn) {

            // For each possible position on the board
            for (const std::string& target : allPositionsOnBoard()) {
                // Check if the movement is legal for that piece
                if (!m_movementValidator.isMoveLegal(piece.get(), target, m_board.getBoard())) {
                    continue;
                }

                // Clone the board and make the move
                Board clonedBoard(m_board);
                Piece* clonedPiece = clonedBoard.getPieceAt(pos);
                if (!clonedPiece) continue;

                // Calculate immediate move score
                int immediateScore = calculateMoveScore(m_board, clonedBoard, pos, target);

                clonedBoard.movePiece(clonedPiece, target);

                // Get opponent's best response through min-max
                int opponentBestScore = 0;
                if (numOfTurns > 1) {
                    opponentBestScore = minMax(clonedBoard, !m_isBlackTurn, 1, numOfTurns);
                }

                // Final score is our score minus opponent's best score
                int finalScore = immediateScore - opponentBestScore;

                // Create and store the possible movement
                PossibleMovement movement;
                movement.setFrom(pos);
                movement.setDestination(target);
                movement.setScore(finalScore);

                // Push to priority queue (will keep only top 5)
                m_bestMoves.push(movement);
            }
        }
    }
}

/**
 * Evaluates the current board state, returning a score from the perspective of the player
 */
int PossibleMoves::evaluateBoard(const Board& board, bool forBlack) const {
    int score = 0;

    // Material advantage evaluation
    for (const auto& [pos, piece] : board.getBoard()) {
        if (!piece) continue;

        int value = getPieceValue(piece.get());
        if (piece->isBlack() == forBlack) {
            score += value;
        }
        else {
            score -= value;
        }
    }

    return score;
}

/**
 * Naive Min-Max algorithm without pruning
 */
int PossibleMoves::minMax(Board& board, bool isBlackTurn, int depth, int maxDepth) {
    // Terminal condition: maximum depth reached
    if (depth == maxDepth) {
        return evaluateBoard(board, m_isBlackTurn);
    }

    // Initialize the best score depending on whether we're maximizing or minimizing
    int bestScore = (isBlackTurn == m_isBlackTurn) ? INT_MIN : INT_MAX;

    // For each piece of the current player
    for (const auto& [pos, piece] : board.getBoard()) {
        if (!piece || piece->isBlack() != isBlackTurn) continue;

        // For each possible position on the board
        for (const std::string& target : allPositionsOnBoard()) {
            // Check if the movement is legal for that piece
            if (!m_movementValidator.isMoveLegal(piece.get(), target, board.getBoard())) {
                continue;
            }

            // Clone board and make move
            Board clonedBoard(board);
            Piece* clonedPiece = clonedBoard.getPieceAt(pos);
            if (!clonedPiece) continue;

            clonedBoard.movePiece(clonedPiece, target);

            // Recursive evaluation
            int score = minMax(clonedBoard, !isBlackTurn, depth + 1, maxDepth);

            // Update best score (max for current player, min for opponent)
            if (isBlackTurn == m_isBlackTurn) {
                bestScore = std::max(bestScore, score);
            }
            else {
                bestScore = std::min(bestScore, score);
            }
        }
    }

    return bestScore;
}

/**
 * Returns all valid positions on the chess board
 */
std::vector<std::string> PossibleMoves::allPositionsOnBoard() const {
    std::vector<std::string> positions;
    positions.reserve(64); // 8x8 board

    for (char row = 'a'; row <= 'h'; ++row) {
        for (char col = '1'; col <= '8'; ++col) {
            positions.push_back(std::string{ row, col });
        }
    }

    return positions;
}

/**
 * Returns the priority queue containing the best moves
 */
const PriorityQueue<PossibleMovement>& PossibleMoves::getBestMoves() const {
    return m_bestMoves;
}


/**
/

#include "ProposeMoves/PossibleMoves.h"
#include "PriorityQueue.h"


PossibleMoves::PossibleMoves(const std::string& boardString, const bool& blackTurn, const MovementValidator& movementValidator)
    :m_board(boardString), m_isBlackTurn(blackTurn), m_movementValidator(movementValidator) {}


// Scores the move (capture, threat, etc.)
int PossibleMoves::calculateMoveScore(Board& boardBefore, Board& boardAfter, const std::string& from, const std::string& to) {
	
	auto movedPiece = boardAfter.getPieceAt(to);
	auto capturedPiece = boardBefore.getPieceAt(to); // If there was a piece in the destination

	int score = 0;

	// Capture value
	if (capturedPiece && capturedPiece->isBlack() != movedPiece->isBlack()) {
		score += capturedPiece->getValue();
	}

	// check if threatened by weaker piece
	for (const auto& [pos, enemyPiece] : boardAfter.getBoard()) {
		if (enemyPiece && enemyPiece->isBlack() != movedPiece->isBlack()) {
			if (m_movementValidator.isMoveLegal(enemyPiece.get(), to, boardAfter.getBoard())) {
				if (enemyPiece->getValue() < movedPiece->getValue()) {
					score -= 2;
				}
			}
		}
	}

	// check if threatens stronger pieces
	for (const auto& [pos, targetPiece] : boardAfter.getBoard()) {
		if (targetPiece && targetPiece->isBlack() != movedPiece->isBlack()) {
			if (m_movementValidator.isMoveLegal(movedPiece, pos, boardAfter.getBoard())) {
				if (targetPiece->getValue() > movedPiece->getValue()) {
					score += 3;
				}
			}
		}
	}

	return score;
}


void PossibleMoves::findPossibleMoves(int numOfTurns) {

	for (const auto& [pos, piece] : m_board.getBoard()) {
		if (piece && piece->isBlack() == m_isBlackTurn) {
			for (std::string target : allPositionsOnBoard()) {

				// check if the movement is legal for that piece
				if (!m_movementValidator.isMoveLegal(piece.get(), target, m_board.getBoard())) continue;

				Board clonedBoard(m_board);
				clonedBoard.movePiece(clonedBoard.getPieceAt(pos), target);

				int moveScore = calculateMoveScore(m_board, clonedBoard, pos, target, m_isBlackTurn);
				int opponentScore = simulate(clonedBoard, !m_isBlackTurn, 1, numOfTurns);

				int finalScore = moveScore - opponentScore;

				PossibleMovement movement;
				movement.setFrom(pos);
				movement.setDestination(target);
				movement.setScore(finalScore);

				m_bestMoves.push(movement);
			}
		}
	}
}



int PossibleMoves::evaluateBoard(const Board &board, bool forBlack) {

	int score = 0;

	for (const auto& [pos, piece] : board.getBoard()) {
		if (!piece) continue;

		int value = piece->getValue(); 
		score += (piece->isBlack() == forBlack) ? value : -value;
	}
	return score;
}



int PossibleMoves::simulate(Board board, bool isBlackTurn, int depth, int maxDepth)
{
	if (depth == maxDepth) {
		return evaluateBoard(board, isBlackTurn);
	}

	int bestScore = m_isBlackTurn ? INT_MIN : INT_MAX;

	for (const auto& [pos, piece] : m_board.getBoard()) {

		if (piece && piece->isBlack() == m_isBlackTurn) {

			for (std::string target : allPositionsOnBoard()) {

				// check if the movement is legal for that piece
				if (!m_movementValidator.isMoveLegal(piece.get(), target, m_board.getBoard())) continue;

				Board clonedBoard(m_board);
				clonedBoard.movePiece(clonedBoard.getPieceAt(pos), target);

				// recursive call
				int score = simulate(clonedBoard, !m_isBlackTurn, depth + 1, maxDepth);
				bestScore = isBlackTurn ? std::max(bestScore, score) : std::min(bestScore, score);
			}
		}
	}

	return bestScore;
}




std::vector<std::string> PossibleMoves::allPositionsOnBoard() const {

	std::vector<std::string> positions;

	for (char row = 'a'; row <= 'h'; ++row) {
		for (char col = '1'; col <= '8'; ++col) {
			positions.push_back(std::string{ row, col });
		}
	}
	return positions;
}



const PriorityQueue<PossibleMovement>& PossibleMoves::getBestMoves() const {
	return m_bestMoves;
}
*/