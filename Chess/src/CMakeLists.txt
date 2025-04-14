#target_sources (Chess PRIVATE "main.cpp"
#							  "Chess.cpp")

target_sources (Chess PRIVATE "main.cpp"
							  "Chess.cpp"
							  "Pieces/Rook.cpp"
							  "Pieces/Queen.cpp"
							  "Pieces/Piece.cpp"
							  "Pieces/Pawn.cpp"
							  "Pieces/Knight.cpp"
							  "Pieces/King.cpp"
							  "Pieces/Bishop.cpp"
							  "Factory/PieceFactory.cpp"
							  "BoardManager.cpp"
							  "GameController.cpp"
							  "MovementValidator.cpp"
)
