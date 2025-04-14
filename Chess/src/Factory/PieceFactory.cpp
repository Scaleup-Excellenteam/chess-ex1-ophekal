#include "Factory/PieceFactory.h"


/**
 * Retrieves the static registry that maps piece names to their corresponding creation functions.
 *
 * @return A reference to the static unordered_map that holds the mapping of piece names
 *         to their corresponding creation functions (PieceCreator).
 */
std::unordered_map<std::string, PieceFactory::PieceCreator>& PieceFactory::getRegistry() {
    static std::unordered_map<std::string, PieceCreator> registry;
    return registry;
}


/**
 * Registers a new piece type with its associated creator function.
 *
 * @param name The name of the piece type (e.g., "pawn", "queen").
 * @param creator The PieceCreator function that creates the piece.
 *
 * @return A boolean indicating whether the registration was successful (always returns true).
 */
bool PieceFactory::registerPiece(const std::string& name, PieceCreator creator) {
    getRegistry().emplace(name, creator);
    return true;
}


/**
 * Creates a new piece instance based on the provided name and attributes.
 *
 * @param name The name of the piece type to create (e.g., "pawn", "queen").
 * @param position The position of the piece on the board (usually represented as a string, e.g., "A1").
 * @param isBlack A boolean indicating whether the piece is black (true) or white (false).
 *
 * @return A unique pointer to the created Piece if the piece type exists in the registry,
 *         otherwise, returns nullptr if the piece name is not found.
 */
std::unique_ptr<Piece> PieceFactory::createPiece(const std::string& name, const std::string& position, bool isBlack) {
    auto it = getRegistry().find(name);
    if (it == getRegistry().end()) {
        return nullptr;
    }
    return it->second(position, isBlack);
}
