#pragma once
#include <string>
#include <unordered_map>
#include <memory>


class Piece
{
public:

	using BoardMap = std::unordered_map<std::string, std::unique_ptr<Piece>>;

	Piece(const std::string& position, bool isBlack, const std::string& name);
	virtual ~Piece() = default;
	virtual bool isMoveValid(const std::string& targetPosition, const BoardMap& board) const =0;
	//virtual bool isMoveValid(const std::string& targetPosition, std::unordered_map<std::string, std::unique_ptr<Piece>>& board) const =0;
	void move(const std::string& targetPosition);
	std::string getName() const;
	std::string getPosition() const;
	bool isBlack() const;
	std::pair<int, int> positionToCoords(const std::string& position) const;

protected:
	std::string m_name;			// The type of the piece
	std::string m_position;		// Current position on the board (e.g., "e4")
	bool m_isBlack;				// True if black, false if white
};