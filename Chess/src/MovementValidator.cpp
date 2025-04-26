#include "MovementValidator.h"



/**
 * Validates whether a move for the given piece to the target position is legal.
 * Checks both the directional validity (according to piece rules)
 * and whether the path is clear of obstructions.
 *
 * @param piece Pointer to the piece being moved.
 * @param targetPosition Target destination on the board.
 * @param board Current state of the board with all active pieces.
 * @return True if the move is legal; otherwise, false.
 */
bool MovementValidator::isMoveLegal(const Piece* piece, const std::string& targetPosition, const BoardMap& board) const {
    
    if (!piece->isDirectionValid(targetPosition)) {
        return false;
    }

    // Special handling for pawns
    if (piece->getName() == "Pawn") {
        return isPawnMoveLegal(piece, targetPosition, board);
    }

    return isPathClear(piece, targetPosition, board);
};


/**
 * Checks if the path between the piece's current position and the target position is clear.
 * Skips this check for King (1-square range).
 *
 * @param piece Pointer to the piece being moved.
 * @param targetPosition Target square.
 * @param board The board map with current pieces.
 * @return True if path is unobstructed; otherwise, false.
 */
bool MovementValidator::isPathClear(const Piece* piece, const std::string& targetPosition, const BoardMap& board) const{
    
    std::string pieceName = piece->getName();
    
    if (pieceName == "King") {
        return true;
    }

    auto [startRow, startCol] = piece->positionToCoords(piece->getPosition());
    auto [endRow, endCol] = piece->positionToCoords(targetPosition);

    // virtical/horizontal path
    if (startRow == endRow || startCol == endCol) {
        return isStraightPathClear(startRow, startCol, endRow, endCol, board);
    }

    // diagonal path
    if (std::abs(endRow - startRow) == std::abs(endCol - startCol)) {
        return isDiagonalPathClear(startRow, startCol, endRow, endCol, board);
    }

    // Should never happen for valid pieces
    return false;
}


/**
 * Checks whether the horizontal or vertical path between two coordinates is free of pieces.
 *
 * @param startRow Starting row index.
 * @param startCol Starting column index.
 * @param endRow Ending row index.
 * @param endCol Ending column index.
 * @param board The current board state.
 * @return True if path is clear; otherwise, false.
 */
bool MovementValidator::isStraightPathClear(int startRow, int startCol, int endRow, int endCol, const BoardMap& board) const {

    if (startRow == endRow) {
        // Horizontal movement
        int start = std::min(startCol, endCol) + 1;
        int end = std::max(startCol, endCol);

        for (int col = start; col < end; col++) {
            std::string positionToCheck = coordsToPosition(startRow, col);
            if (board.find(positionToCheck) != board.end()) {
                return false;
            }
        }
    }
    else {
        // Vertical movement
        int start = std::min(startRow, endRow) + 1;
        int end = std::max(startRow, endRow);

        for (int row = start; row < end; row++) {
            std::string positionToCheck = coordsToPosition(row, startCol);
            if (board.find(positionToCheck) != board.end()) {
                return false;
            }
        }
    }

    return true;
}


/**
 * Checks whether the diagonal path between two coordinates is free of pieces.
 *
 * @param startRow Starting row index.
 * @param startCol Starting column index.
 * @param endRow Ending row index.
 * @param endCol Ending column index.
 * @param board The current board state.
 * @return True if path is clear; otherwise, false.
 */
bool MovementValidator::isDiagonalPathClear(int startRow, int startCol, int endRow, int endCol, const BoardMap& board) const {
    
    int rowDirection = (endRow > startRow) ? 1 : -1;
    int colDirection = (endCol > startCol) ? 1 : -1;

    int row = startRow + rowDirection;
    int col = startCol + colDirection;

    while (row != endRow && col != endCol) {
        std::string positionToCheck = coordsToPosition(row, col);
        if (board.find(positionToCheck) != board.end()) {
            return false;
        }
        row += rowDirection;
        col += colDirection;
    }
    return true;
}


/**
 * Converts a pair of (row, column) coordinates into a chessboard-style position string.
 *
 * @param row The row index (0 = 'a', 7 = 'h').
 * @param col The column index (0 = '1', 7 = '8').
 * @return A string representing the position (e.g., "e4").
 */
std::string MovementValidator::coordsToPosition(int row, int col) const {
    char rowChar = 'a' + row;
    char colChar = '1' + col;
    return std::string(1, rowChar) + std::string(1, colChar);
}


bool MovementValidator::isPawnMoveLegal(const Piece* piece, const std::string& targetPosition, const BoardMap& board) const {

    auto [startRow, startCol] = piece->positionToCoords(piece->getPosition());
    auto [endRow, endCol] = piece->positionToCoords(targetPosition);

    // check if the movement is diagonal
    if (startCol != endCol) {
        
        // check if there's an opponents piece in target position
        auto it = board.find(targetPosition);
        if (it == board.end()) {
            return false;
        }

        Piece* targetPiece = it->second.get();
        if (targetPiece->isBlack() == piece->isBlack()) {
            return false;
        }

        // means the target piece is the opponenets, valid movement
        return true;
    }

    // check foward movement
    // target must be empty in order to allow movement
    if (board.find(targetPosition) != board.end()) {
        return false;
    }

    // If it's a two-square move, all square must be empty
    int forwardDirection = piece->isBlack() ? -1 : 1;

    if (std::abs(endRow - startRow) == 2) {
        int intermediateRow = startRow + forwardDirection;
        std::string intermediatePos = coordsToPosition(intermediateRow, startCol);

        // if there's a piece in the path, not valid
        if (board.find(intermediatePos) != board.end()) {
            return false;
        }
    }

    // if got here means the movement is one square forward and is valid
    return true;

}