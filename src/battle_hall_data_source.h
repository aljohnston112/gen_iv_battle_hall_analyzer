#ifndef BATTLE_HALL_DATA_SOURCE_H
#define BATTLE_HALL_DATA_SOURCE_H

#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "custom_pokemon.h"
#include "nature.h"
#include "serebii_pokemon_data_source.h"

static constexpr uint8_t NUMBER_OF_GROUPS = 4;
static constexpr uint8_t MAX_RANK = 10;

constexpr uint8_t NUMBER_OF_MOVES = 4;
constexpr uint8_t NUMBER_OF_EVS = 6;

constexpr uint8_t NUMBER_OF_TYPES = static_cast<uint8_t>(PokemonType::COUNT);
const std::map<uint8_t, std::set<uint8_t>> GROUP_TO_RANKS = {
    {1, {1, 2, 3, 4, 5}},
    {2, {3, 4, 5, 6, 7, 8}},
    {3, {6, 7, 8, 9, 10}},
    {4, {9, 10}},
};

struct BattleHallPokemon {
    std::string name;
    std::string item;
    std::vector<const MoveInfo*> moves;
    Nature nature;
    std::unordered_map<Stat, uint8_t> evs;
};

std::array<
    std::vector<BattleHallPokemon>,
    NUMBER_OF_GROUPS
> get_all_battle_hall_pokemon(
    const std::vector<const MoveInfo*>& all_move_infos
);

void print_all_battle_hall_pokemon(
    const std::array<
        std::vector<BattleHallPokemon>,
        NUMBER_OF_GROUPS
    >& group_to_hall_pokemon
);

std::array<
    std::array<
        std::array<std::vector<CustomPokemon>, NUMBER_OF_TYPES>,
        MAX_RANK
    >,
    NUMBER_OF_GROUPS
> get_all_custom_hall_pokemon(
    const std::unordered_map<
        std::string,
        SerebiiPokemon
    >& name_to_serebii_pokemon,
    const std::array<
        std::vector<BattleHallPokemon>,
        NUMBER_OF_GROUPS
    >& group_to_battle_hall_pokemon,
    const std::vector<const MoveInfo*>& all_move_infos
);

void export_battle_hall_pokemon(
    const std::array<
        std::vector<BattleHallPokemon>,
        NUMBER_OF_GROUPS
    >& group_to_hall_pokemon,
    const std::array<
        std::array<
            std::array<std::vector<CustomPokemon>, NUMBER_OF_TYPES>,
            MAX_RANK
        >,
        NUMBER_OF_GROUPS
    >& group_to_rank_to_over_2
);

#endif //BATTLE_HALL_DATA_SOURCE_H
