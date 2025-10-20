#ifndef BATTLE_SIMULATOR_H
#define BATTLE_SIMULATOR_H

#include "custom_pokemon.h"

struct BattleResultEntry {
    const CustomPokemon* opponent;
    bool won;
    std::unordered_set<const MoveInfo*> player_moves;
    std::unordered_set<const MoveInfo*> opponent_moves;
};

BattleResultEntry battle(
    const CustomPokemon& player,
    const CustomPokemon& opponent
);

#endif //BATTLE_SIMULATOR_H
