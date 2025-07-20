#ifndef HIT_ANALYZER_H
#define HIT_ANALYZER_H

#include <string>
#include <vector>

#include "battle_hall_data_source.h"
#include "custom_pokemon.h"

void analyze(
    const std::array<
        std::array<
            std::array<std::vector<CustomPokemon>, NUMBER_OF_TYPES>,
            MAX_RANK
        >,
        NUMBER_OF_GROUPS
    >& group_to_rank_to_over_2_to_hall_pokemon,
    const std::unordered_map<
        std::string,
        std::vector<CustomPokemon>
    >& player_pokemon_forms,
    const std::array<int, NUMBER_OF_TYPES>& type_to_rank_to_skip =
        [] {
            std::array<int, NUMBER_OF_TYPES> array{};
            array.fill(-1);
            return array;
        }()
);

#endif //HIT_ANALYZER_H
