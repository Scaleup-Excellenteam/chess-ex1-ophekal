#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "Piece.h"
#include "Factory/PieceFactory.h"

class BoardManager
{
public:
	BoardManager(const std::string& boardString);
	virtual ~BoardManager() = default;

protected:
	std::unordered_map<std::string, std::unique_ptr<Piece>> m_board;       // The polymorphic mapping of the board

};