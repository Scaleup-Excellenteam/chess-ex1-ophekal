#pragma once

#include <string>
#include <ostream>

class PossibleMovement {
private:

	// maybe change to std::pair!

	int m_score = 0;
	std::string m_from;
	std::string m_destination;

public:
	PossibleMovement() : m_score(0) {}
	void setFrom(const std::string& from);
	void setDestination(const std::string& to);
	void setScore(int score);
	
	int getScore()const;
	const std::string& getFrom() const;
	const std::string& getDestination() const;
};


// Global operator overloads
bool operator>(const PossibleMovement& m1, const PossibleMovement& m2);
std::ostream& operator<<(std::ostream& os, const PossibleMovement& obj);