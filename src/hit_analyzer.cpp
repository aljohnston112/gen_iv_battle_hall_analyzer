#include "hit_analyzer.h"

#include <format>
#include <ranges>

#include "battle_hall_data_source.h"
#include "battle_simulator.h"
#include "custom_pokemon.h"
#include "serebii_pokemon_data_source.h"

void battle_all(
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
    >& player_pokemon_forms
) {
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
                    for (const auto& player_pokemon_ :
                         player_pokemon_forms | std::views::values
                    ) {
                        for (const auto& player_pokemon : player_pokemon_) {
                            const auto won =
                                battle(player_pokemon, opponent_pokemon);
                            if (won) {
                                printf(std::format(
                                        "Rank: {:02}, Over 2: {:02}, {}\n",
                                        rank,
                                        over_2,
                                        get_pokemon_name(opponent_pokemon.name
                                        )
                                    ).c_str()
                                );
                            }
                        }
                    }
                }
            }
        }
    }
}

void analyze() {
    const auto all_serebii_pokemon =
        get_all_serebii_pokemon();
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

    // for (const auto& [_, serebii_pokemon] : all_serebii_pokemon) {
    //     const auto& player_pokemon_forms =
    //         convert_serebii_to_custom(serebii_pokemon);
    //     battle_all(group_to_rank_to_over_2, player_pokemon_forms);
    // }
    const auto& player_pokemon_forms =
        convert_serebii_to_custom(all_serebii_pokemon.at("Sunkern"));
    battle_all(group_to_rank_to_over_2, player_pokemon_forms);
}
