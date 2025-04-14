#include "Pieces/Piece.h"


/**
 * Constructs a Piece with the specified position, color, and name.
 *
 * @param position The initial position of the piece on the board.
 * @param isBlack A boolean indicating whether the piece is black (true) or white (false).
 * @param name The name/type of the piece.
 */
Piece::Piece(const std::string& position, bool isBlack, const std::string& name)
	:m_position(position), m_isBlack(isBlack), m_name(name){}


/**
 * Sets the piece's position to the given target position.
 * Assumes that the move has already been validated.
 *
 * @param targetPosition The new position to move the piece to.
 */
void Piece::move(const std::string& targetPosition) {
	m_position = targetPosition;
}


/**
 * Converts a board position string into a pair of zero-based row and column indices.
 *
 * @param position The position on the board in algebraic notation.
 * @return A pair of integers representing (row, column) as zero-based indices.
 */
std::pair<int, int> Piece::positionToCoords(const std::string& position) const {

	int row = position[0] - 'a';
	int col = position[1] - '1';
	return { row,col };
}


// Getters

/**
 * Gets the name of the piece.
 *
 * @return A string representing the name/type of the piece (e.g., "Rook").
 */
std::string Piece::getName() const {
	return m_name;
}


/**
 * Gets the current position of the piece on the board.
 *
 * @return A string representing the piece's position (e.g., "D5").
 */
std::string Piece::getPosition() const {
	return m_position;
}


/**
 * Checks whether the piece is black.
 *
 * @return true if the piece is black, false if it is white.
 */
bool Piece::isBlack() const {
	return m_isBlack;
}