#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include "Pieces/Piece.h"


class PieceFactory
{
public:
	using PieceCreator = std::function<std::unique_ptr<Piece>(const std::string&, bool)>;

	static bool registerPiece(const std::string& name, PieceCreator creator);
	static std::unique_ptr<Piece> createPiece(const std::string& name, const std::string& position, bool isBlack);

private:
	static std::unordered_map<std::string, PieceCreator>& getRegistry();

};