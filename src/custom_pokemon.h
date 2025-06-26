#ifndef CUSTOMPOKEMON_H
#define CUSTOMPOKEMON_H
#include <string>
#include <unordered_set>

#include "serebii_pokemon_data_source.h"

struct CustomPokemon {
    std::string name;
    uint8_t level;
    std::string item;
    std::unordered_set<PokemonType> types;
    std::unordered_set<MoveInfo> moves;
    std::unordered_map<Stat, uint16_t> stats;
};

void save_custom_pokemon(
    const std::vector<CustomPokemon>& pokemon_list,
    const std::string& filename
);

std::vector<CustomPokemon> load_custom_pokemon(const std::string& filename);

#endif //CUSTOMPOKEMON_H
