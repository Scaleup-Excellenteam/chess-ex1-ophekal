#include "ProposeMoves/PossibleMoves.h"
#include "PriorityQueue.h"
#include <climits>
#include <algorithm>
#include <iostream>

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




PossibleMoves::PossibleMoves(const MovementValidator& movementValidator)
    : m_movementValidator(movementValidator) {}


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
        return 0;
    }

    int score = 0;

    auto capturedPiece = boardBefore.getPieceAt(to);
    if (capturedPiece && capturedPiece->isBlack() != movedPiece->isBlack()) {
        int captureValue = getPieceValue(capturedPiece);
        score += captureValue * CAPTURE_BONUS_MULTIPLIER;
        std::cout << "Captured piece at " << to << " worth " << captureValue << " -> score = " << score << std::endl;
    }

    /*
    * for (const auto& [pos, enemyPiece] : boardAfter.getBoard()) {
        if (enemyPiece && enemyPiece->isBlack() != movedPiece->isBlack()) {
            if (m_movementValidator.isMoveLegal(enemyPiece.get(), to, boardAfter.getBoard())) {
                if (getPieceValue(enemyPiece.get()) < getPieceValue(movedPiece)) {
                    score += THREATENED_BY_WEAKER_PENALTY;
                    std::cout << "Threatened by weaker piece at " << pos << std::endl;
                }
            }
        }
    }
    */
    for (const auto& [pos, enemyPiece] : boardAfter.getBoard()) {
        if (!enemyPiece || enemyPiece->isBlack() == movedPiece->isBlack()) continue;

        // Check if this enemy piece can move to the new location of our moved piece
        if (m_movementValidator.isMoveLegal(enemyPiece.get(), to, boardAfter.getBoard())) {
            int threatValue = getPieceValue(enemyPiece.get());
            int myValue = getPieceValue(movedPiece);

            if (threatValue < myValue) {
                score += THREATENED_BY_WEAKER_PENALTY;
                std::cout << "Threatened by weaker piece at " << pos << std::endl;
            }
            else {
                score += THREATENED_BY_STRONGER_PENALTY;
                std::cout << "Threatened by stronger or equal piece at " << pos << std::endl;
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


void PossibleMoves::findPossibleMoves(int depth, bool isBlack, const Board& board ) {

    std::cout << "[findPossibleMoves] Start"<< std::endl;

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

                
                std::cout << "[findPossibleMoves] Immediate score for " << pos
                    << " -> " << target << ": " << immediateScore << std::endl;

                // Calculate future score through minMax algorithm
                int futureScore = 0;
                if (depth > 0) {
                    futureScore = minMax(clonedBoard, !m_isBlackTurn, 1, depth);
                    //futureScore = -futureScore;
                    std::cout << "[findPossibleMoves] Future score from minMax: " << futureScore << std::endl;
                }

                
               // int score = minMax(clonedBoard, !m_isBlackTurn, 0, depth);

               // std::cout << "Score for move " << pos << " -> " << target << " is " << score << ")\n";

                // Final score is immediate + future
                int finalScore = immediateScore + futureScore;

                std::cout << "[findPossibleMoves] Final score for " << pos
                    << " -> " << target << ": " << finalScore << std::endl;

                // Create movement object and add to priority queue
                PossibleMovement movement;
                movement.setFrom(pos);
                movement.setDestination(target);
                movement.setScore(finalScore);
                m_bestMoves.push(movement);
            }
        }
    }
    //printBestMoves();
}



int PossibleMoves::minMax(Board& board, bool isBlackTurn, int depth, int maxDepth) {

    if (depth > maxDepth) {
        return 0;
    }

    int bestScore = (isBlackTurn == m_recommendForBlack) ? INT_MIN : INT_MAX;
    //int bestScore = INT_MIN;


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
                    
                    std::cout << "[minMax] Depth " << depth << " move " << pos << " -> " << target
                        << " evaluated with score " << score << std::endl;

                    // Convert score to be from root player's perspective:
                   // if (isBlackTurn != m_recommendForBlack) {
                      //  score = -score;
                  //  }

                }
                else {
                    score = minMax(clonedBoard, !isBlackTurn, depth + 1, maxDepth);
                }

                /*
                *      if (isBlackTurn == m_recommendForBlack) {
                    bestScore = std::max(bestScore, score);
                }
                else {
                    bestScore = std::min(bestScore, score);
                }            
                */
                if (isBlackTurn == m_recommendForBlack) {
                    bestScore = std::max(bestScore, score);
                }
                else {
                    bestScore = std::min(bestScore, score);
                }
                
              //  if (isBlackTurn != m_recommendForBlack) {
              //      score = -score;
              //  }

                bestScore = std::max(bestScore, score);

            }
        }
    }

    return (bestScore == INT_MIN || bestScore == INT_MAX) ? 0 : bestScore;

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



/*
* int PossibleMoves::minMax(Board& board, bool isBlackTurn, int depth, int maxDepth) {

    // Only terminate when we've exceeded the maximum depth
    if (depth > maxDepth) {
        // Simply return 0 since we don't need an additional evaluation
        return 0;
    }

    // Initialize best score depending on whose turn it is
    int bestScore = (isBlackTurn == m_recommendForBlack) ? INT_MIN : INT_MAX;

    // Iterate through all pieces of the current player
    for (const auto& [pos, piece] : board.getBoard()) {

        // Only consider moves for pieces of the current player
        if (piece && piece->isBlack() == isBlackTurn) {

            for (const std::string& target : allPositionsOnBoard()) {

                if (target == pos) continue;

                auto targetPiece = board.getPieceAt(target);
                if (targetPiece && (targetPiece->isBlack() == isBlackTurn)) continue;

                if (!m_movementValidator.isMoveLegal(piece.get(), target, board.getBoard())) continue;

                // Make move
                Board clonedBoard(board);
                Piece* clonedPiece = clonedBoard.getPieceAt(pos);
                if (!clonedPiece) continue;

                // Save board state before move
                Board beforeBoard(clonedBoard);

                // Simulate the move
                clonedBoard.movePiece(clonedPiece, target);

                // Calculate immediate score for this move
                int immediateScore = calculateMoveScore(beforeBoard, clonedBoard, pos, target);

                // Get opponent's response
                int opponentScore = minMax(clonedBoard, !isBlackTurn, depth + 1, maxDepth);

                // Calculate total score
                int totalScore;

                if (isBlackTurn == m_recommendForBlack) {
                    // If it's our turn, we want to maximize our score
                    totalScore = immediateScore - opponentScore;
                    bestScore = std::max(bestScore, totalScore);
                }
                else {
                    // If it's opponent's turn, they want to minimize our score
                    totalScore = opponentScore - immediateScore;
                    bestScore = std::min(bestScore, totalScore);
                }
            }
        }
    }

    if (isBlackTurn != m_recommendForBlack) {
        // If it's the opponent's turn, flip the sign of the best score
        // to represent it from our perspective
        bestScore = -bestScore;
    }
    return bestScore;

}

*/

/*
* 
* void PossibleMoves::findPossibleMoves(int depth, bool isBlack, const Board& board ) {

    std::cout << "[findPossibleMoves] Start - numOfTurns: " << depth << std::endl;
    std::cout << "===== STARTING NEW MOVE SEARCH =====" << std::endl;

    // Clear previous best moves
    while (!m_bestMoves.getQueue().empty()) {
        m_bestMoves.poll();
    }

    // save the color of player we recommend moves for
    m_recommendForBlack = isBlack;

    // save color of current player
    m_isBlackTurn = isBlack;


    for (const auto& [pos, piece] : board.getBoard()) {
        
        if (piece && piece->isBlack() == m_recommendForBlack) {
            
            // For each possible destination position
            for (const std::string& target : allPositionsOnBoard()) {

                if (target == pos) continue; // Skip if target is same as origin

                auto targetPiece = board.getPieceAt(target);
                if (targetPiece && (targetPiece->isBlack() == m_isBlackTurn)) continue;

                // Check if the move is legal
                if (!m_movementValidator.isMoveLegal(piece.get(), target, board.getBoard())) continue;

                // Create a copy of the board to simulate the move
                Board clonedBoard(board);
                Piece* clonedPiece = clonedBoard.getPieceAt(pos);
                if (!clonedPiece) continue;

                // Save board state before move
                Board beforeBoard(clonedBoard);

                // Simulate the move
                clonedBoard.movePiece(clonedPiece, target);

                // Calculate immediate score for this move
                int immediateScore = calculateMoveScore(beforeBoard, clonedBoard, pos, target);

                // The opponent now gets to move, so we start minMax with opponent's turn
                int opponentScore = minMax(clonedBoard, !m_isBlackTurn, 1, depth);

                // Total score for this move
                int finalScore = immediateScore - opponentScore;
                //int finalScore = immediateScore + opponentScore;

                // Debug logging
                std::cout << "Move " << pos << " -> " << target << " immediate score: " << immediateScore << std::endl;
                std::cout << "Opponent's best response score: " << opponentScore << std::endl;
                std::cout << "Final score after subtraction: " << finalScore << std::endl;

                // Create movement object and add to priority queue
                PossibleMovement movement;
                movement.setFrom(pos);
                movement.setDestination(target);
                movement.setScore(finalScore);

                m_bestMoves.push(movement);
            }
        }
    }

    std::cout << "[findPossibleMoves] End" << std::endl;
    printBestMoves();
}


*/

// possibl moves: 
/*
* 
int PossibleMoves::minMax(Board& board, bool isBlackTurn, int depth, int maxDepth) {

    // Print entry to function
    std::cout << "Entering minMax - Depth: " << depth << ", Player: " << (isBlackTurn ? "Black" : "White") << std::endl;


    // Only terminate when we've exceeded the maximum depth
    if (depth > maxDepth) {
        // Simply return 0 since we don't need an additional evaluation
      //  std::cout << "  Reached max depth, returning 0" << std::endl;
        return 0;
    }

    // Initialize best score depending on whose turn it is
    int bestScore = (isBlackTurn == m_recommendForBlack) ? INT_MIN : INT_MAX;
    // std::cout << "  Initial bestScore: " << bestScore << std::endl;


     // Iterate through all pieces of the current player
    for (const auto& [pos, piece] : board.getBoard()) {

        // Only consider moves for pieces of the current player
        if (piece && piece->isBlack() == isBlackTurn) {

            for (const std::string& target : allPositionsOnBoard()) {

                if (target == pos) continue;

                auto targetPiece = board.getPieceAt(target);
                if (targetPiece && (targetPiece->isBlack() == isBlackTurn)) continue;

                if (!m_movementValidator.isMoveLegal(piece.get(), target, board.getBoard())) continue;

                // Print move being considered
            //    std::cout << "  Trying move - " << pos << " -> " << target << std::endl;


                // Make move
                Board clonedBoard(board);
                Piece* clonedPiece = clonedBoard.getPieceAt(pos);
                if (!clonedPiece) continue;

                // Save board state before move
                Board beforeBoard(clonedBoard);

                // Simulate the move
                clonedBoard.movePiece(clonedPiece, target);

                // Calculate immediate score for this move
                int immediateScore = calculateMoveScore(beforeBoard, clonedBoard, pos, target);
                //    std::cout << "    Immediate score: " << immediateScore << std::endl;


                    // Get opponent's response
                  //  std::cout << "    Recursing to find opponent's response..." << std::endl;
                int futureScore = minMax(clonedBoard, !isBlackTurn, depth + 1, maxDepth);
                // std::cout << "    Returned from recursion, opponent score: " << futureScore << std::endl;


                if (isBlackTurn == m_recommendForBlack) {
                    // If it's our turn, we want to maximize our score
                    bestScore = std::max(bestScore, immediateScore + futureScore);
                }
                else {
                    // If it's opponent's turn, they want to minimize our score
                    bestScore = std::min(bestScore, -immediateScore + futureScore);
                }
            }
        }
    }

    // No need to flip sign - the scores are consistently evaluated from the perspective
    // of the player we're recommending moves for
    std::cout << "Exiting minMax - Depth: " << depth
        << ", Player: " << (isBlackTurn ? "Black" : "White")
        << ", Final bestScore: " << bestScore << std::endl;
    return bestScore;

}


*/

// minmax
/*
* int PossibleMoves::minMax(Board& board, bool isBlackTurn, int depth, int maxDepth) {

    // Print entry to function
    std::cout << "Entering minMax - Depth: " << depth << ", Player: " << (isBlackTurn ? "Black" : "White") << std::endl;


    // Only terminate when we've exceeded the maximum depth
    if (depth > maxDepth) {
        // Simply return 0 since we don't need an additional evaluation
      //  std::cout << "  Reached max depth, returning 0" << std::endl;
        return 0;
    }

    // Initialize best score depending on whose turn it is
    int bestScore = (isBlackTurn == m_recommendForBlack) ? INT_MIN : INT_MAX;
   // std::cout << "  Initial bestScore: " << bestScore << std::endl;


    // Iterate through all pieces of the current player
    for (const auto& [pos, piece] : board.getBoard()) {

        // Only consider moves for pieces of the current player
        if (piece && piece->isBlack() == isBlackTurn) {

            for (const std::string& target : allPositionsOnBoard()) {

                if (target == pos) continue;

                auto targetPiece = board.getPieceAt(target);
                if (targetPiece && (targetPiece->isBlack() == isBlackTurn)) continue;

                if (!m_movementValidator.isMoveLegal(piece.get(), target, board.getBoard())) continue;

                // Print move being considered
            //    std::cout << "  Trying move - " << pos << " -> " << target << std::endl;


                // Make move
                Board clonedBoard(board);
                Piece* clonedPiece = clonedBoard.getPieceAt(pos);
                if (!clonedPiece) continue;

                // Save board state before move
                Board beforeBoard(clonedBoard);

                // Simulate the move
                clonedBoard.movePiece(clonedPiece, target);

                // Calculate immediate score for this move
                int immediateScore = calculateMoveScore(beforeBoard, clonedBoard, pos, target);
            //    std::cout << "    Immediate score: " << immediateScore << std::endl;


                // Get opponent's response
              //  std::cout << "    Recursing to find opponent's response..." << std::endl;
                int futureScore = minMax(clonedBoard, !isBlackTurn, depth + 1, maxDepth);
               // std::cout << "    Returned from recursion, opponent score: " << futureScore << std::endl;


                if (isBlackTurn == m_recommendForBlack) {
                    // If it's our turn, we want to maximize our score
                    bestScore = std::max(bestScore, immediateScore + futureScore);
                }
                else {
                    // If it's opponent's turn, they want to minimize our score
                    bestScore = std::min(bestScore, -immediateScore + futureScore);
                }
            }
        }
    }

    // No need to flip sign - the scores are consistently evaluated from the perspective
    // of the player we're recommending moves for
    std::cout << "Exiting minMax - Depth: " << depth
        << ", Player: " << (isBlackTurn ? "Black" : "White")
        << ", Final bestScore: " << bestScore << std::endl;
    return bestScore;

}


*/