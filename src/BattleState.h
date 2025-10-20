#ifndef BATTLE_SIMULATOR_H
#define BATTLE_SIMULATOR_H

#include "custom_pokemon.h"

struct MoveDamagePairHash {
    std::size_t operator()(
        const std::pair<const MoveInfo*, int>& p
    ) const noexcept {
        const std::size_t h1 = std::hash<const MoveInfo*>{}(p.first);
        const std::size_t h2 = std::hash<int>{}(p.second);
        return h1 ^ (h2 << 8);
    }
};

struct BattleResultEntry {
    const CustomPokemon* opponent;
    bool won;
    std::unordered_set<
        std::pair<const MoveInfo*, int>,
        MoveDamagePairHash
    > player_moves;
    std::unordered_set<
        std::pair<const MoveInfo*, int>,
        MoveDamagePairHash
    > opponent_moves;
};

BattleResultEntry battle(
    const CustomPokemon& player,
    const CustomPokemon& opponent
);

#endif //BATTLE_SIMULATOR_H
