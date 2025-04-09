#include "Pieces/Piece.h"


Piece::Piece(const std::string& position, bool isBlack, const std::string& name)
	:m_position(position), m_isBlack(isBlack), m_name(name){}


// sets the position once the movement had been checked and found valid
void Piece::move(const std::string& targetPosition) {
	m_position = targetPosition;
}

std::string Piece::getName() const {
	return m_name;
}

std::string Piece::getPosition() const {
	return m_position;
}

bool Piece::isBlack() const {
	return m_isBlack;
}

// Converts board stringposition to index
std::pair<int, int> Piece::positionToCoords(const std::string& position) const {

	int row = position[0] - 'a';
	int col = position[1] - '1';
	return { row,col };
}