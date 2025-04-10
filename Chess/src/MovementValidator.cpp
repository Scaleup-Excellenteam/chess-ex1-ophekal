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
    if (pieceName = "King") {
        return true;
    }

    auto [startRow, startCol] = piece->positionToCoords(piece->getPosition());
    auto [endRow, endCol] = piece->positionToCoords(targetPosition);


}


bool MovementValidator::isStraightPathClear(int startRow, int startCol, int endRow, int endCol, const BoardMap& board) {}


bool MovementValidator::isDiagonalPathClear(int startRow, int startCol, int endRow, int endCol, const BoardMap& board) {}


std::string MovementValidator::coordsToPosition(int row, int col) {}
