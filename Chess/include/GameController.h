#pragma once

#include <string>
#include "BoardManager.h"

class GameController
{
public:
	GameController(const std::string& boardString);
	bool isMovemenetValid(const std::string& response);

private:
	BoardManager m_boardManager;
	bool m_isBlackTurn;
	void updateIsBlackTurn(bool isBlackTurn);
	bool isCurrentPlayerBlack () const;



};