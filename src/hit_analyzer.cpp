#include "hit_analyzer.h"

#include <ranges>

#include "battle_hall_data_source.h"
#include "battle_simulator.h"
#include "custom_pokemon.h"
#include "serebii_pokemon_data_source.h"


void analyze() {
    const auto all_serebii_pokemon =
        get_all_serebii_pokemon();
    // const auto& serebii_pokemon = all_serebii_pokemon.at("Bulbasaur");
    // const auto& player_pokemon_forms =
    //     convert_serebii_to_custom(serebii_pokemon);

    const auto all_moves =
        get_all_pokemon_moves(
            all_serebii_pokemon
        );
    const auto group_to_rank_to_over_2 =
        get_all_custom_hall_pokemon(
            all_serebii_pokemon,
            get_all_battle_hall_pokemon(
                all_moves
            ),
            all_moves
        );

    for (const auto& [_, serebii_pokemon] : all_serebii_pokemon) {
        const auto& player_pokemon_forms =
            convert_serebii_to_custom(serebii_pokemon);
        for (uint8_t group_number = 4; group_number > 0; group_number--) {
            const auto& rank_to_over_2 =
                group_to_rank_to_over_2.at(group_number);
            for (auto it = GROUP_TO_RANKS.at(group_number).rbegin();
                 it != GROUP_TO_RANKS.at(group_number).rend();
                 ++it
            ) {
                uint8_t rank = *it;
                const auto& over_2_to_hall_pokemon =
                    rank_to_over_2.at(rank);
                for (int8_t over_2 = 17; over_2 >= 0; over_2--) {
                    const auto& hall_pokemon =
                        over_2_to_hall_pokemon.at(over_2);
                    for (const auto& opponent_pokemon : hall_pokemon) {
                        for (const auto& [
                                 form,
                                 player_pokemon_
                             ] : player_pokemon_forms
                        ) {
                            for (const auto& player_pokemon : player_pokemon_) {
                                battle(player_pokemon, opponent_pokemon);
                            }
                        }
                    }
                }
            }
        }
    }
}
