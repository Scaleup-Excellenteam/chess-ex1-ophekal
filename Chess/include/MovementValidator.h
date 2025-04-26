#pragma once
#include "Pieces/Piece.h"
#include <unordered_map>
#include <memory>

class MovementValidator {

public:
    using BoardMap = std::unordered_map<std::string, std::unique_ptr<Piece>>;

    MovementValidator() = default;
    bool isMoveLegal(const Piece* piece, const std::string& targetPosition, const BoardMap& board) const;

private:
    bool isPathClear(const Piece* piece, const std::string& targetPosition, const BoardMap& board) const;
    bool isStraightPathClear(int startRow, int startCol, int endRow, int endCol, const BoardMap& board) const;
    bool isDiagonalPathClear(int startRow, int startCol, int endRow, int endCol, const BoardMap& board) const;
    bool isPawnMoveLegal(const Piece* piece, const std::string& targetPosition, const BoardMap& board) const;
    std::string coordsToPosition(int row, int col) const;
};