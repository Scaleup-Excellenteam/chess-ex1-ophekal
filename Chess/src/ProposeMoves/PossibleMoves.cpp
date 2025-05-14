#include "ProposeMoves/PossibleMoves.h"
#include "PriorityQueue.h"
#include <climits>
#include <algorithm>
#include <iostream> // Added for debug prints

const int PAWN_VALUE = 100;
const int KNIGHT_VALUE = 320;
const int BISHOP_VALUE = 330;
const int ROOK_VALUE = 500;
const int QUEEN_VALUE = 900;
const int KING_VALUE = 20000;

const int THREATENED_BY_WEAKER_PENALTY = -200;
const int THREATENS_STRONGER_BONUS = 150;
const int CAPTURE_BONUS_MULTIPLIER = 10;




PossibleMoves::PossibleMoves(const std::string& boardString, const bool& blackTurn, const MovementValidator& movementValidator)
    : m_board(boardString), m_isBlackTurn(blackTurn), m_movementValidator(movementValidator) {
    std::cout << "PossibleMoves constructed with boardString: " << boardString << std::endl;
}

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

int PossibleMoves::calculateMoveScore(Board& boardBefore, Board& boardAfter, const std::string& from, const std::string& to) {
    std::cout << "[calculateMoveScore] From " << from << " To " << to << std::endl;
    auto movedPiece = boardAfter.getPieceAt(to);
    if (!movedPiece) {
        std::cout << "No moved piece found at " << to << std::endl;
        return 0;
    }

    int score = 0;
    auto capturedPiece = boardBefore.getPieceAt(to);
    if (capturedPiece && capturedPiece->isBlack() != movedPiece->isBlack()) {
        int captureValue = getPieceValue(capturedPiece);
        score += captureValue * CAPTURE_BONUS_MULTIPLIER;
        std::cout << "Captured piece at " << to << " worth " << captureValue << " -> score = " << score << std::endl;
    }

    for (const auto& [pos, enemyPiece] : boardAfter.getBoard()) {
        if (enemyPiece && enemyPiece->isBlack() != movedPiece->isBlack()) {
            if (m_movementValidator.isMoveLegal(enemyPiece.get(), to, boardAfter.getBoard())) {
                if (getPieceValue(enemyPiece.get()) < getPieceValue(movedPiece)) {
                    score += THREATENED_BY_WEAKER_PENALTY;
                    std::cout << "Threatened by weaker piece at " << pos << std::endl;
                }
            }
        }
    }

    for (const auto& [pos, targetPiece] : boardAfter.getBoard()) {
        if (targetPiece && targetPiece->isBlack() != movedPiece->isBlack()) {
            if (m_movementValidator.isMoveLegal(movedPiece, pos, boardAfter.getBoard())) {
                if (getPieceValue(targetPiece.get()) > getPieceValue(movedPiece)) {
                    score += THREATENS_STRONGER_BONUS;
                    std::cout << "Threatens stronger piece at " << pos << std::endl;
                }
            }
        }
    }

    std::cout << "Final move score: " << score << std::endl;
    return score;
}




void PossibleMoves::findPossibleMoves(int numOfTurns) {
    std::cout << "[findPossibleMoves] Start - numOfTurns: " << numOfTurns << std::endl;
    std::cout << "===== STARTING NEW MOVE SEARCH =====" << std::endl;

    // Clear previous best moves
    while (!m_bestMoves.getQueue().empty()) {
        m_bestMoves.poll();
    }

    // For statistics
    int totalMovesEvaluated = 0;
    int movesWithNonZeroScore = 0;

    for (const auto& [pos, piece] : m_board.getBoard()) {
        if (piece && piece->isBlack() == m_isBlackTurn) {
            std::cout << "Analyzing piece at " << pos << " (" << piece->getName() << ")" << std::endl;

            for (const std::string& target : allPositionsOnBoard()) {
                if (target == pos) continue; // Skip moves to the same position

                if (!m_movementValidator.isMoveLegal(piece.get(), target, m_board.getBoard())) continue;

                totalMovesEvaluated++;

                // Check if it's a capture move
                auto targetPiece = m_board.getPieceAt(target);
                bool isCapture = (targetPiece != nullptr && targetPiece->isBlack() != piece->isBlack());

                std::cout << "  Testing move: " << pos << " -> " << target
                    << (isCapture ? " (CAPTURE)" : "") << std::endl;

                Board clonedBoard(m_board);
                Piece* clonedPiece = clonedBoard.getPieceAt(pos);
                if (!clonedPiece) {
                    std::cout << "  Cloned piece at " << pos << " is null" << std::endl;
                    continue;
                }

                // Move first, then calculate score
                clonedBoard.movePiece(clonedPiece, target);

                int immediateScore = calculateMoveScore(m_board, clonedBoard, pos, target);
                std::cout << "  Immediate score: " << immediateScore << std::endl;

                int opponentBestScore = 0;
                if (numOfTurns > 1) {
                    std::cout << "  Calculating opponent's best response..." << std::endl;
                    opponentBestScore = minMax(clonedBoard, !m_isBlackTurn, 1, numOfTurns);
                    std::cout << "  Opponent's best score: " << opponentBestScore << std::endl;
                }

                int finalScore = immediateScore - opponentBestScore;
                std::cout << "  Final evaluated score: " << finalScore << std::endl;

                if (finalScore != 0) {
                    movesWithNonZeroScore++;
                }

                PossibleMovement movement;
                movement.setFrom(pos);
                movement.setDestination(target);
                movement.setScore(finalScore);

                m_bestMoves.push(movement);
            }
        }
    }

    std::cout << "Total moves evaluated: " << totalMovesEvaluated << std::endl;
    std::cout << "Moves with non-zero score: " << movesWithNonZeroScore << std::endl;
    std::cout << "[findPossibleMoves] End" << std::endl;
    printBestMoves();
}






int PossibleMoves::evaluateBoard(const Board& board, bool forBlack) const {
    int score = 0;
    for (const auto& [pos, piece] : board.getBoard()) {
        if (!piece) continue;

        int value = getPieceValue(piece.get());
        score += (piece->isBlack() == forBlack) ? value : -value;
    }

    return score;
}


int PossibleMoves::minMax(Board& board, bool isBlackTurn, int depth, int maxDepth) {
    
    if (depth == maxDepth) {
        return evaluateBoard(board, m_isBlackTurn);
    }

    int bestScore = (isBlackTurn == m_isBlackTurn) ? INT_MIN : INT_MAX;

    for (const auto& [pos, piece] : board.getBoard()) {
        if (!piece || piece->isBlack() != isBlackTurn) continue;

        for (const std::string& target : allPositionsOnBoard()) {
            
            if (target == pos) continue;
            
            if (!m_movementValidator.isMoveLegal(piece.get(), target, board.getBoard())) continue;

            Board clonedBoard(board);
            Piece* clonedPiece = clonedBoard.getPieceAt(pos);
            if (!clonedPiece) continue;

            clonedBoard.movePiece(clonedPiece, target);
            int score = minMax(clonedBoard, !isBlackTurn, depth + 1, maxDepth);

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


// Add this function to your PossibleMoves class
void PossibleMoves::printBestMoves() const {
    std::cout << "=== TOP MOVES IN QUEUE ===" << std::endl;
    int i = 1;
    for (const auto& move : m_bestMoves.getQueue()) {
        std::cout << i++ << ") " << move << " with score: " << move.getScore() << std::endl;
    }
    std::cout << "=========================" << std::endl;
}
