#pragma once


// Represents the possible outcomes when attempting to move a piece on the board
enum class MoveResult {
    NoPieceAtSource = 11,
    OpponentPieceAtSource = 12,
    PlayerPieceAtTarget = 13,
    InvalidMoveOrBlocked = 21,
    MoveCausesCheck = 31,
    ValidMoveCausesCheck = 41,
    ValidMove = 42
};
