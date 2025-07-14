#ifndef HIT_ANALYZER_H
#define HIT_ANALYZER_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "custom_pokemon.h"

void analyze(
    const std::unordered_map<
        uint8_t,
        std::unordered_map<
            uint8_t,
            std::unordered_map<uint8_t, std::vector<CustomPokemon>>
        >
    >& group_to_rank_to_over_2,
    const std::unordered_map<
        std::string,
        std::vector<CustomPokemon>
    >& player_pokemon_forms,
    const std::unordered_map<PokemonType, int>& type_to_rank_to_skip =
        std::unordered_map<PokemonType, int>{}
);

#endif //HIT_ANALYZER_H
