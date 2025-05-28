#include "ProposeMoves/PossibleMovement.h"

/**
 * Sets the starting position of the movement.
 *
 * @param from The starting position.
 */
void PossibleMovement::setFrom(const std::string& from) {
	m_from = from;
}


/**
 * Sets the destination position of the movement.
 *
 * @param to The destination position.
 */
void PossibleMovement::setDestination(const std::string& to) {
	m_destination = to;
}


/**
 * Sets the score for this movement.
 *
 * @param score The score to assign to this movement.
 */
void PossibleMovement::setScore(int score) {
	m_score = score;
}


/**
 * Returns the score of the movement.
 *
 * @return The current score of the movement.
 */
int PossibleMovement::getScore() const
{
	return m_score;
}


/**
 * Returns the starting position of the movement.
 *
 * @return A constant reference to the starting position.
 */
const std::string& PossibleMovement::getFrom() const
{
	return m_from;
}


/**
 * Returns the destination position of the movement.
 *
 * @return A constant reference to the destination position.
 */
const std::string& PossibleMovement::getDestination() const
{
	return m_destination;
}



//-----------------------------------------------------------------------------
// Global operators implementations
//----------------------------------------------------------------------------- 

/**
 * Greater than operator to compare two movements by score.
 *
 * @param m1 The first movement.
 * @param m2 The second movement.
 * @return True if m1 has a higher score than m2.
 */
bool operator>(const PossibleMovement& m1, const PossibleMovement& m2) {
	return m1.getScore() > m2.getScore();
}


/**
 * Stream insertion operator for PossibleMovement objects.
 *
 * @param os The output stream.
 * @param obj The movement to output.
 * @return The output stream.
 */
std::ostream& operator<<(std::ostream& os, const PossibleMovement& obj) {
	os << obj.getFrom() << " -> " << obj.getDestination();
	return os;
}
