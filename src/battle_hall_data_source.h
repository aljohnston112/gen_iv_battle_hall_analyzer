#ifndef BATTLE_HALL_DATA_SOURCE_H
#define BATTLE_HALL_DATA_SOURCE_H
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

constexpr int NUMBER_OF_MOVES = 4;
constexpr int NUMBER_OF_EVS = 6;

struct BattleHallPokemon {
    std::string name;
    std::string item;
    std::vector<std::string> moves;
    std::string nature;
    std::vector<uint8_t> evs;
};

using GroupedPokemon =
    std::unordered_map<uint8_t, std::vector<BattleHallPokemon>>;

GroupedPokemon get_battle_hall_pokemon();

void print_battle_hall_pokemon();

#endif //BATTLE_HALL_DATA_SOURCE_H
