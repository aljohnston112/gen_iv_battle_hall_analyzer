#ifndef BATTLE_SIMULATOR_H
#define BATTLE_SIMULATOR_H

#include "custom_pokemon.h"

std::pair<bool, std::unordered_set<const MoveInfo*>> battle(
    const CustomPokemon& player,
    const CustomPokemon& opponent
);

#endif //BATTLE_SIMULATOR_H
