#include "MovementValidator.h"



// Validates whether a move for the given piece to the target position is legal,
// by checking both direction validity and path clearance
bool MovementValidator::isMoveLegal(const Piece* piece, const std::string& targetPosition, const BoardMap& board) {
    
    if (!piece->isDirectionValid(targetPosition)) {
        return false;
    }

    return isPathClear(piece, targetPosition, board);
};


// Checks if the path between the piece's current position and the target position is clear
bool MovementValidator::isPathClear(const Piece* piece, const std::string& targetPosition, const BoardMap& board) {
    
    std::string pieceName = piece->getName();
    
    // King moves one square so no path to check
    if (pieceName == "King") {
        return true;
    }

    auto [startRow, startCol] = piece->positionToCoords(piece->getPosition());
    auto [endRow, endCol] = piece->positionToCoords(targetPosition);

    // virtical or horizontal path
    if (startRow == endRow || startCol == endCol) {
        return isStraightPathClear(startRow, startCol, endRow, endCol, board);
    }

    // diagonal path
    if (std::abs(endRow - startRow) == std::abs(endCol - startCol)) {
        return isDiagonalPathClear(startRow, startCol, endRow, endCol, board);
    }

    // will never reach here
    return false;
}


// Checks if the straight (horizontal or vertical) path between two coordinates is clear
bool MovementValidator::isStraightPathClear(int startRow, int startCol, int endRow, int endCol, const BoardMap& board) {

    if (startRow == endRow) {
        // the movement is horizontal
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
        // the movement is vertical
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


// Checks if the diagonal path between two coordinates is clear
bool MovementValidator::isDiagonalPathClear(int startRow, int startCol, int endRow, int endCol, const BoardMap& board) {
    
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


// Utility to convert coordinates back to a chess board position
std::string MovementValidator::coordsToPosition(int row, int col) {
    char rowChar = 'a' + row;
    char colChar = '1' + col;
    return std::string(1, rowChar) + std::string(1, colChar);
}
