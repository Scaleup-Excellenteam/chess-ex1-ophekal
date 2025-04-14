#include "Factory/PieceFactory.h"


// Retrieves the static registry that maps piece names to their corresponding creation functions
std::unordered_map<std::string, PieceFactory::PieceCreator>& PieceFactory::getRegistry() {
    static std::unordered_map<std::string, PieceCreator> registry;
    return registry;
}


// Registers a new piece type with its associated creator function
bool PieceFactory::registerPiece(const std::string& name, PieceCreator creator) {
    getRegistry().emplace(name, creator);
    return true;
}


// Creates a new piece instance based on the provided name and attributes.
std::unique_ptr<Piece> PieceFactory::createPiece(const std::string& name, const std::string& position, bool isBlack) {
    auto it = getRegistry().find(name);
    if (it == getRegistry().end()) {
        return nullptr;
    }
    return it->second(position, isBlack);
}
