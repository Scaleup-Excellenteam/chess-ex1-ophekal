#pragma once

#include <string>
#include <ostream>

class chessMove {
private:
	int m_score;
	std::string m_from;
	std::string m_destination;

public:
	chessMove(const std::string& from, const std::string& to, int score = 0) {}
	void setFrom(const std::string& from);
	void setDestination(const std::string& to);
	void setScore(int score);
	
	int getScore()const;
	const std::string& getFrom() const;
	const std::string& getDestination() const;
};


// Global operator overloads
int operator()(const chessMove& m1, const chessMove& m2) const {
std::ostream& operator<<(std::ostream& os, const chessMove& obj);