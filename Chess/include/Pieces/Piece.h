#pragma once
#include <string>

class Piece
{
public:
	Piece(const string& position, bool isBlack, const string& name);
	virtual ~Piece() = default;
	virtual bool isMoveValid(const string& targetPosition) const =0;
	virtual void move(const string& targetPosition);
	string getName() const;
	string getPosition() const;
	bool isBlack() const;

protected:
	string m_name;       // The type of the piece
	string m_position;   // Current position on the board (e.g., "e4")
	bool m_isBlack;      // True if black, false if white
};