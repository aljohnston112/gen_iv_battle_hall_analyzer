#ifndef CUSTOMPOKEMON_H
#define CUSTOMPOKEMON_H
#include <cstdint>
#include <set>
#include <string>

#include "abilities.h"
#include "nature.h"


struct CustomPokemon {
    Pokemon name;
    Ability ability;
    uint8_t level;
    Item item;
    std::set<PokemonType> types;
    std::vector<const MoveInfo*> moves;
    std::array<uint16_t, static_cast<int>(Stat::NO_STAT)> stats;
};

void save_custom_pokemon(
    const std::vector<CustomPokemon>& pokemon_list,
    const std::string& filename
);

std::vector<CustomPokemon> load_custom_pokemon(
    const std::string& filename,
    const std::vector<const MoveInfo*>& all_moves
);
#endif //CUSTOMPOKEMON_H
