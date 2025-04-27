#include "ProposeMoves/PossibleMovement.h"

void PossibleMovement::setFrom(const std::string& from) {
	m_from = from;
}

void PossibleMovement::setDestination(const std::string& to) {
	m_destination = to;
}

void PossibleMovement::setScore(int score) {
	m_score = score;
}

int PossibleMovement::getScore() const
{
	return m_score;
}

const std::string& PossibleMovement::getFrom() const
{
	return m_from;
}

const std::string& PossibleMovement::getDestination() const
{
	return m_destination;
}



//-----------------------------------------------------------------------------
// Global operators implementations
//----------------------------------------------------------------------------- 
bool operator>(const PossibleMovement& m1, const PossibleMovement& m2) {
	return m1.getScore() > m2.getScore();
}


std::ostream& operator<<(std::ostream& os, const PossibleMovement& obj) {
	os << obj.getFrom() << " " << obj.getDestination();
	return os;
}
