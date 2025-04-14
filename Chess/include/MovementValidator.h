#pragma once
#include "Pieces/Piece.h"
#include <unordered_map>
#include <memory>

class MovementValidator {

public:
    using BoardMap = std::unordered_map<std::string, std::unique_ptr<Piece>>;

    MovementValidator() = default;
    bool isMoveLegal(const Piece* piece, const std::string& targetPosition, const BoardMap& board);

private:
    bool isPathClear(const Piece* piece, const std::string& targetPosition, const BoardMap& board);
    bool isStraightPathClear(int startRow, int startCol, int endRow, int endCol, const BoardMap& board);
    bool isDiagonalPathClear(int startRow, int startCol, int endRow, int endCol, const BoardMap& board);
    std::string coordsToPosition(int row, int col);
};