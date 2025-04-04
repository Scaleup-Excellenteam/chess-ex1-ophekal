#pragma once
#include <string>

class Piece
{
public:
	Piece(const std::string& position, bool isBlack, const std::string& name);
	virtual ~Piece() = default;
	virtual bool isMoveValid(const std::string& targetPosition) const =0;
	virtual void move(const std::string& targetPosition);
	std::string getName() const;
	std::string getPosition() const;
	bool isBlack() const;

protected:
	std::string m_name;       // The type of the piece
	std::string m_position;   // Current position on the board (e.g., "e4")
	bool m_isBlack;      // True if black, false if white
};