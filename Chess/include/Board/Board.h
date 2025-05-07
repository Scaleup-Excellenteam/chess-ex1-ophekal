#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "Factory/PieceFactory.h"


class Board {
public:   
    Board(const std::string& boardString);
	Board(const Board& other); 
    Piece* getPieceAt(const std::string&) const;
    void movePiece(Piece* piece, const std::string& to);
    Piece* removePieceAt(const std::string& position);
    void placePiece(Piece* piece, const std::string& position);
    const std::unordered_map<std::string, std::unique_ptr<Piece>>& getPieces() const;


private:
    std::unordered_map<std::string, std::unique_ptr<Piece>> m_pieces;
    std::string charToPieceName(char symbol) const;
	std::string indexToPosition(int index) const;

};