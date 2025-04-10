#include "MovementValidator.h"



// The main validation method that combines direction validation and path checking
bool MovementValidator::isMoveLegal(const Piece* piece, const std::string& targetPosition, const BoardMap& board) {
    
    if (!piece->isDirectionValid(targetPosition)) {
        return false;
    }

    // check if the path is clear
    return isPathClear(piece, targetPosition, board);
};


bool MovementValidator::isPathClear(const Piece* piece, const std::string& targetPosition, const BoardMap& board) {
    
    std::string pieceName = piece->getName();
    
    // King moves one square so no path to check
    if (pieceName == "King") {
        return true;
    }

    auto [startRow, startCol] = piece->positionToCoords(piece->getPosition());
    auto [endRow, endCol] = piece->positionToCoords(targetPosition);

    // virtcal or horizontal path
    if (startRow == endRow || startCol == endCol) {
        return isStraightPathClear(startRow, startCol, endRow, endCol, board);
    }

    // diagonal path
    if (std::abs(endRow - startRow) == std::abs(endCol - startCol)) {
        return isDiagonalPathClear(startRow, startCol, endRow, endCol, board);
    }

    // will not reach here
    return false;
}


bool MovementValidator::isStraightPathClear(int startRow, int startCol, int endRow, int endCol, const BoardMap& board) {

    if (startRow == endRow) {
        // since the move is horizontal only the col changes
        int start = std::min(startCol, endCol) + 1;
        int end = std::max(startCol, endCol);

        for (int col = start; col < end; col++) {
            std::string positionToCheck = coordsToPosition(startRow, col);

            if (board.find(positionToCheck) != board.end()) {
                // means there's a piece in the way
                return false;
            }
        }
    }
    else {
        // the movement is vertical
        int start = std::min(startRow, endRow) + 1;
        int end = std::max(startRow, endRow);

        for (int row = start; row < end; row++) {
            std::string positionToCheck = coordsToPosition(row, startCol);

            if (board.find(positionToCheck) != board.end()) {
                // means there's a piece in the way
                return false;
            }
        }
    }

    return true; // path is clear
}


bool MovementValidator::isDiagonalPathClear(int startRow, int startCol, int endRow, int endCol, const BoardMap& board) {
    return false;

}


std::string MovementValidator::coordsToPosition(int row, int col) {
    char rowChar = 'a' + row;
    char colChar = '1' + col;
    return std::string(1, rowChar) + std::string(1, colChar);
}
