#include "hit_analyzer.h"

#include <cmath>
#include <ranges>

#include "battle_hall_data_source.h"
#include "battle_simulator.h"
#include "config.h"
#include "custom_pokemon.h"
#include "nature.h"
#include "serebii_pokemon_data_source.h"

uint get_damage_to_defender(
    const CustomPokemon& attacker,
    const CustomPokemon& defender,
    const MoveInfo& move
) {
    return 0;
}

std::optional<MoveInfo> get_best_move_against_defender(
    const CustomPokemon& attacker,
    const CustomPokemon& defender
) {
    std::optional<MoveInfo> best_move;
    uint best_damage = 0;
    for (const auto& move : attacker.moves) {
        if (const uint damage =
                get_damage_to_defender(attacker, defender, move);
            damage > best_damage
        ) {
            best_damage = damage;
            best_move.value() = move;
        }
    }
    return best_move;
}

std::unordered_map<Stat, uint16_t> get_stats_for_serebii(
    const std::string& form,
    const SerebiiPokemon& serebii_pokemon
) {
    std::unordered_map<Stat, uint16_t> base_stats;
    if (form == "all" ||
        form == "Plant Cloak" ||
        form == "Normal Forme" ||
        form == "Land Forme" ||
        form == "Altered Forme"
    ) {
        base_stats = serebii_pokemon.base_stats;
    } else {
        base_stats = serebii_pokemon.form_to_base_stats.at(form);
    }
    std::unordered_map<Stat, uint16_t> stats;
    for (const auto& [stat, value] : base_stats) {
        stats[stat] = get_stat(
            LEVEL,
            stat,
            value,
            0,
            0,
            NATURE_MAP.at(NatureEnum::HARDY)
        );
    }
    return stats;
}

std::unordered_map<std::string, CustomPokemon> convert_serebii_to_custom(
    const SerebiiPokemon& serebii_pokemon
) {
    std::unordered_map<std::string, CustomPokemon> customs;
    const auto moves =
        get_moves_for_serebii_pokemon(serebii_pokemon);
    for (const auto& [form, form_moves] : moves) {
        customs[form] =
        {
            .name = serebii_pokemon.name,
            .level = LEVEL,
            .item = "",
            .types = serebii_pokemon.types,
            .moves = form_moves,
            .stats = get_stats_for_serebii(form, serebii_pokemon)
        };
    }
    return customs;
}

void analyze() {
    const auto all_serebii_pokemon =
        get_all_serebii_pokemon();
    // const auto& serebii_pokemon = all_serebii_pokemon.at("Bulbasaur");
    // const auto& player_pokemon_forms =
    //     convert_serebii_to_custom(serebii_pokemon);

    const auto group_to_rank_to_over_2 =
        get_all_custom_hall_pokemon(
            all_serebii_pokemon,
            get_all_battle_hall_pokemon(
                get_all_pokemon_moves(
                    all_serebii_pokemon
                )
            )
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
                        for (const auto& player_pokemon :
                             player_pokemon_forms
                        ) {
                            battle(player_pokemon.second, opponent_pokemon);
                        }
                    }
                }
            }
        }
    }
}
