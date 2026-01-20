#ifndef CUSTOMPOKEMON_H
#define CUSTOMPOKEMON_H

#include <cstdint>
#include <string>

#include "abilities.h"
#include "Items.h"
#include "nature.h"


struct CustomPokemon {
    Pokemon name;
    Ability ability;
    uint8_t level;
    Item item;
    std::array<PokemonType, 2> types;
    std::vector<const MoveInfo*> moves;
    std::array<uint16_t, static_cast<int>(Stat::NO_STAT)> stats;
    double pounds;
};

constexpr uint bits_for_ability =
    std::bit_width(static_cast<uint>(Ability::Disabled));
constexpr uint bits_for_level =
    std::bit_width(static_cast<uint>(Ability::Disabled));
constexpr uint bits_for_stat =
    std::bit_width(static_cast<uint>(Stat::NO_STAT));

struct CustomPokemonHash {
    std::size_t operator()(const CustomPokemon* p) const {
        return (static_cast<int>(p->name) << (bits_for_ability + bits_for_level)) |
        (static_cast<int>(p->level) << bits_for_ability) |
            static_cast<int>(p->ability);
    }
};

struct CustomPokemonEq {
    bool operator()(const CustomPokemon* a, const CustomPokemon* b) const {
        return a->name == b->name && a->ability == b->ability && a->level == b->level;
    }
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
