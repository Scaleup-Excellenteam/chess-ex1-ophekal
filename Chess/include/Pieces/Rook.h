#pragma once
#include "Pieces/Piece.h"

class Rook: public Piece
{
public:
	Rook(const string& position, bool isBlack);
	bool isMoveValid(string targetPosition) const override;

};