// Chess 
#include "Chess.h"
#include "GameController.h"
#include <iostream>

int main()
{
	string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr"; 
	//string board = "R#BQKB#RPPPPPPPP################################ppppppppr#bqkb#r"; 
	Chess a(board);

	int wantedDepth;
	std::cout << "Enter the number of turns you want to take into consideration\n";
	std::cin >> wantedDepth;

	GameController controller(board, wantedDepth);

	int codeResponse = 0;

	auto moves = controller.recommendMoves();
	std::string formatted = controller.formatRecommendations(moves);

	string res = a.getInput(formatted);
	while (res != "exit")
	{
		/* 
		codeResponse value : 
		Illegal movements : 
		11 - there is not piece at the source  
		12 - the piece in the source is piece of your opponent
		13 - there one of your pieces at the destination 
		21 - illegal movement of that piece 
		31 - this movement will cause you checkmate

		legal movements : 
		41 - the last movement was legal and cause check 
		42 - the last movement was legal, next turn 
		*/
		/**/ 
		{ 
			MoveResult result = controller.validateMovement(res);
			codeResponse = int(result);			
		}
		/**/

		auto moves = controller.recommendMoves();
		std::string formatted = controller.formatRecommendations(moves);

		a.setCodeResponse(codeResponse);
		res = a.getInput(formatted); 
	}

	cout << endl << "Exiting " << endl; 
	return 0;
}