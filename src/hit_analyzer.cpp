#include "hit_analyzer.h"

#include <cmath>
#include <ranges>

#include "battle_hall_data_source.h"
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

std::unordered_map<Stat, int> get_stats_for_serebii(
    const std::string& form,
    const SerebiiPokemon& serebii_pokemon
) {
    std::unordered_map<Stat, int> base_stats;
    if (form == "all") {
        base_stats = serebii_pokemon.base_stats;
    } else {
        base_stats = serebii_pokemon.form_to_base_stats.at(form);
    }
    std::unordered_map<Stat, int> stats;
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
    const auto all_moves =
        get_all_pokemon_moves(all_serebii_pokemon);
    const auto all_hall_pokemon =
        get_all_battle_hall_pokemon(all_moves);


    // std::unordered_set<Move> all_move_enums;
    // for (int i = 0; i < static_cast<int>(Move::Count); ++i) {
    //     all_move_enums.insert(static_cast<Move>(i));
    // }
    // for (const auto& move_info : all_moves) {
    //     all_move_enums.erase(move_map[move_info.name]);
    // }
}
