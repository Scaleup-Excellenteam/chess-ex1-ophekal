#include "ProposeMoves/chessMove.h"



chessMove::chessMove(const std::string& from, const std::string& to, int score = 0)
	:m_from(from), m_to(to), m_score(score) {}


void chessMove::setFrom(const std::string& from) {
	m_from = from;
}

void chessMove::setDestination(const std::string& to) {
	m_destination = to;
}

void chessMove::setScore(int score) {
	m_score = score;
}

void chessMove::addScore(int points) {
	m_score += points;
}

int chessMove::getScore() const
{
	return m_score;
}

const std::string& chessMove::getFrom() const
{
	return m_from;
}

const std::string& chessMove::getDestination() const
{
	return m_destination;
}



//-----------------------------------------------------------------------------
// Global operators implementations
//----------------------------------------------------------------------------- 
int operator()(const chessMove& m1, const chessMove& m2) const {
	return m1.getScore() - m2.getScore(); //higher score = better move
}


std::ostream& operator<<(std::ostream& os, const chessMove& obj) {
	os << obj.getFrom() << " -> " << obj.getDestination();
	return os;
}
