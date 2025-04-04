#include "Factory/PieceFactory.h"

std::unordered_map<std::string, PieceFactory::PieceCreator>& PieceFactory::getRegistry() {
    static std::unordered_map<std::string, PieceCreator> registry;
    return registry;
}

bool PieceFactory::registerPiece(const std::string& name, PieceCreator creator) {
    getRegistry().emplace(name, creator);
    return true;
}

std::unique_ptr<Piece> PieceFactory::createPiece(const std::string& name, const std::string& position, bool isBlack) {
    auto it = getRegistry().find(name);
    if (it == getRegistry().end()) {
        return nullptr;
    }
    return it->second(position, isBlack);

