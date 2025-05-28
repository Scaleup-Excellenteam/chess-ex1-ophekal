#include "ProposeMoves/PossibleMoves.h"
#include "PriorityQueue.h"
#include <climits>
#include <algorithm>

const int PAWN_VALUE = 100;
const int KNIGHT_VALUE = 320;
const int BISHOP_VALUE = 330;
const int ROOK_VALUE = 500;
const int QUEEN_VALUE = 900;
const int KING_VALUE = 20000;

const int THREATENED_BY_WEAKER_PENALTY = -100;
const int THREATENED_BY_STRONGER_PENALTY = -200;
const int THREATENS_STRONGER_BONUS = 150;
const int CAPTURE_BONUS_MULTIPLIER = 10;



/**
 * Constructs a PossibleMoves object with a movement validator.
 *
 * @param movementValidator The validator used to check move legality.
 */
PossibleMoves::PossibleMoves(const MovementValidator& movementValidator)
    : m_movementValidator(movementValidator) {}


/**
 * Returns the point value of a chess piece.
 *
 * @param piece Pointer to the piece to evaluate.
 * @return The point value of the piece, or 0 if piece is null or unknown.
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
 * Calculates the score for a specific move by evaluating captures, threats, and tactical benefits.
 *
 * @param boardBefore The board state before the move.
 * @param boardAfter The board state after the move.
 * @param from The starting position of the move.
 * @param to The destination position of the move.
 * @return The calculated score for the move.
 */
int PossibleMoves::calculateMoveScore(Board& boardBefore, Board& boardAfter, const std::string& from, const std::string& to) {
       
    auto movedPiece = boardAfter.getPieceAt(to);
    if (!movedPiece) {
        return 0;
    }

    int score = 0;

    auto capturedPiece = boardBefore.getPieceAt(to);
    if (capturedPiece && capturedPiece->isBlack() != movedPiece->isBlack()) {
        int captureValue = getPieceValue(capturedPiece);
        score += captureValue * CAPTURE_BONUS_MULTIPLIER;
    }

    for (const auto& [pos, enemyPiece] : boardAfter.getBoard()) {
        if (!enemyPiece || enemyPiece->isBlack() == movedPiece->isBlack()) continue;

        // Check if this enemy piece can move to the new location of our moved piece
        if (m_movementValidator.isMoveLegal(enemyPiece.get(), to, boardAfter.getBoard())) {
            int threatValue = getPieceValue(enemyPiece.get());
            int myValue = getPieceValue(movedPiece);

            if (threatValue < myValue) {
                score += THREATENED_BY_WEAKER_PENALTY;
            }
            else {
                score += THREATENED_BY_STRONGER_PENALTY;
            }
        }
    }

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
 * Finds and evaluates all possible moves for a given color using minimax algorithm.
 *
 * @param depth The search depth for the minimax algorithm.
 * @param isBlack True if finding moves for black pieces, false for white.
 * @param board The current board state to analyze.
 */
void PossibleMoves::findPossibleMoves(int depth, bool isBlack, const Board& board ) {


    // Clear previous best moves
    while (!m_bestMoves.getQueue().empty()) {
        m_bestMoves.poll();
    }

    m_recommendForBlack = isBlack;
    m_isBlackTurn = isBlack;


    for (const auto& [pos, piece] : board.getBoard()) {
        if (piece && piece->isBlack() == m_recommendForBlack) {
            for (const std::string& target : allPositionsOnBoard()) {
                if (target == pos) continue;

                auto targetPiece = board.getPieceAt(target);
                if (targetPiece && (targetPiece->isBlack() == m_isBlackTurn)) continue;

                if (!m_movementValidator.isMoveLegal(piece.get(), target, board.getBoard())) continue;

                // Create a copy of the board to simulate the move
                Board clonedBoard(board);
                Piece* clonedPiece = clonedBoard.getPieceAt(pos);
                if (!clonedPiece) continue;
                Board beforeBoard(clonedBoard);
                clonedBoard.movePiece(clonedPiece, target);
                
                // Calculate immediate score for this move
                int immediateScore = calculateMoveScore(beforeBoard, clonedBoard, pos, target);

                // Calculate future score through minMax algorithm
                int futureScore = 0;
                if (depth > 0) {
                    futureScore = minMax(clonedBoard, !m_isBlackTurn, 1, depth);
                }

                // Final score is immediate + future
                int finalScore = immediateScore + futureScore;

                // Create movement object and add to priority queue
                PossibleMovement movement;
                movement.setFrom(pos);
                movement.setDestination(target);
                movement.setScore(finalScore);
                m_bestMoves.push(movement);
            }
        }
    }
}


/**
 * Implements the minimax algorithm to evaluate future move consequences.
 *
 * @param board The board state to evaluate.
 * @param isBlackTurn True if it's black's turn, false for white.
 * @param depth Current depth in the search tree.
 * @param maxDepth Maximum depth to search.
 * @return The best score achievable from this position.
 */
int PossibleMoves::minMax(Board& board, bool isBlackTurn, int depth, int maxDepth) {

    if (depth > maxDepth) {
        return 0;
    }

    int bestScore = (isBlackTurn == m_recommendForBlack) ? INT_MIN : INT_MAX;

    for (const auto& [pos, piece] : board.getBoard()) {
        if (piece && piece->isBlack() == isBlackTurn) {
            for (const std::string& target : allPositionsOnBoard()) {
                if (target == pos) continue;

                auto targetPiece = board.getPieceAt(target);
                if (targetPiece && (targetPiece->isBlack() == isBlackTurn)) continue;

                if (!m_movementValidator.isMoveLegal(piece.get(), target, board.getBoard())) continue;

                // Simulate move
                Board clonedBoard(board);
                Piece* clonedPiece = clonedBoard.getPieceAt(pos);
                if (!clonedPiece) continue;

                Board beforeBoard(clonedBoard);
                clonedBoard.movePiece(clonedPiece, target);

                int score;
                if (depth == maxDepth) {
                    
                    score = calculateMoveScore(beforeBoard, clonedBoard, pos, target);

                    // Only negate if it's not the root player's turn
                    if (isBlackTurn != m_recommendForBlack) {
                        score = -score;
                    }
                }
                else {
                    score = minMax(clonedBoard, !isBlackTurn, depth + 1, maxDepth);
                }

                if (isBlackTurn == m_recommendForBlack) {
                    bestScore = std::max(bestScore, score);
                }
                else {
                    bestScore = std::min(bestScore, score);
                }

                bestScore = std::max(bestScore, score);

            }
        }
    }

    return (bestScore == INT_MIN || bestScore == INT_MAX) ? 0 : bestScore;

}


/**
 * Generates all possible positions on a standard chess board.
 *
 * @return A vector containing all 64 board positions in algebraic notation.
 */
std::vector<std::string> PossibleMoves::allPositionsOnBoard() const {

    std::vector<std::string> positions;

    for (char row = 'a'; row <= 'h'; ++row) {
        for (char col = '1'; col <= '8'; ++col) {
            positions.push_back(std::string{ row, col });
        }
    }

    return positions;
}


/**
 * Returns the priority queue containing the best moves found.
 *
 * @return A constant reference to the priority queue of best moves.
 */
const PriorityQueue<PossibleMovement>& PossibleMoves::getBestMoves() const {
    return m_bestMoves;
}
