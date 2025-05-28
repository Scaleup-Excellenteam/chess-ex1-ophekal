// Chess 
#include "Chess.h"
#include "GameController.h"
#include <iostream>
#include <Exceptions/StringFormatException.h>
#include <Exceptions/EmptyQueueException.h>

int main()
{
	string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr"; 

	try {

		Chess a(board);

		int wantedDepth;
		std::cout << "Please enter search depth (higher values give better moves but take longer): ";
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

			MoveResult result = controller.validateMovement(res);
			codeResponse = int(result);

			try {
				auto moves = controller.recommendMoves();
				std::string formatted = controller.formatRecommendations(moves);
				a.setCodeResponse(codeResponse);
				res = a.getInput(formatted);
			}
			catch (const EmptyQueueException& e) {
				std::cerr << "Warning: " << e.what() << std::endl;
				std::string formatted = "No moves available";
				a.setCodeResponse(codeResponse);
				res = a.getInput(formatted);
			}
		}
	}

	catch (const StringFormatException& e) {
		std::cerr << "Error creating game: " << e.what() << std::endl;
		std::cout << "Please check your board string format." << std::endl;
		return 1;
	}
	catch (const EmptyQueueException& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	cout << endl << "Exiting " << endl; 
	return 0;
}