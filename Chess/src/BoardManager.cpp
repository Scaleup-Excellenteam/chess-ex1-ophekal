#include "BoardManager.h"


BoardManager::BoardManager(const std::string& boardString){

	for (auto i = 0; i < boardString.size; i++)
	{
		char symbol = boardString[i];
		if (symbol == '#') {
			continue;
		}

	//	std::string pieceName = charToPieceName(symbol);
		bool isBlack = islower(symbol);	// capital letter is white
	//	std::string position = indexToPosition(symbol);

	//	m_board[position] = PieceFactory::createPiece(pieceName, position, isBlack);
	}
}


