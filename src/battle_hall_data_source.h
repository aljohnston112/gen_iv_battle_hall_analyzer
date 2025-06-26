#ifndef BATTLE_HALL_DATA_SOURCE_H
#define BATTLE_HALL_DATA_SOURCE_H
#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "custom_pokemon.h"
#include "nature.h"
#include "serebii_pokemon_data_source.h"

constexpr int NUMBER_OF_MOVES = 4;
constexpr int NUMBER_OF_EVS = 6;

constexpr uint8_t NUMBER_OF_TYPES = 17;
const std::map<uint8_t, std::set<uint8_t>> GROUP_TO_RANKS = {
    {1, {1, 2, 3, 4, 5}},
    {2, {3, 4, 5, 6, 7, 8}},
    {3, {6, 7, 8, 9, 10}},
    {4, {9, 10}},
};

struct BattleHallPokemon {
    std::string name;
    std::string item;
    std::vector<MoveInfo> moves;
    Nature nature;
    std::unordered_map<Stat, uint8_t> evs;
};

using AllBattleHallPokemon =
std::unordered_map<uint8_t, std::vector<BattleHallPokemon>>;

AllBattleHallPokemon get_all_battle_hall_pokemon(
    const std::unordered_map<Move, MoveInfo>& all_moves
);

void print_all_battle_hall_pokemon(const AllBattleHallPokemon& data);

std::unordered_map<
    uint8_t,
    std::unordered_map<
        uint8_t, std::unordered_map<uint8_t, std::vector<CustomPokemon>>
    >
> get_all_custom_hall_pokemon(
    const std::unordered_map<std::string, SerebiiPokemon>& all_serebii_pokemon,
    const AllBattleHallPokemon& all_battle_hall_pokemon
);


#endif //BATTLE_HALL_DATA_SOURCE_H
