#include "Pieces/Piece.h"


Piece::Piece(const string& position, bool isBlack, const string& name)
	:m_position(position), m_isBlack(isBlack), m_name(name){}

// sets the position once the movement had been checked and found valid
void Piece::move(const string& targetPosition) {
	m_position = targetPosition;
}

string Piece::getName() const {
	return m_name;
}

string Piece::getPosition() const {
	return m_position;
}

bool Piece::isBlack() const {
	return m_isBlack;
}
