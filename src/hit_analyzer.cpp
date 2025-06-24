#include "hit_analyzer.h"

#include <cmath>
#include <ranges>

#include "battle_hall_data_source.h"
#include "config.h"
#include "moves.h"
#include "nature.h"
#include "serebii_pokemon_data_source.h"

struct CustomPokemon {
    std::string name;
    std::string item;
    std::unordered_set<MoveInfo> moves;
    std::unordered_map<Stat, int> stats;
};

uint get_damage_to_defender(
    const CustomPokemon& attacker,
    const CustomPokemon& defender,
    const MoveInfo& move
) {
    return 0;
}

std::optional<Move> get_best_move_against_defender(
    const CustomPokemon& attacker,
    const CustomPokemon& defender
) {
    std::optional<Move> best_move;
    uint best_damage = 0;
    for (const auto& move : attacker.moves) {
        if (const uint damage =
                get_damage_to_defender(attacker, defender, move);
            damage > best_damage
        ) {
            best_damage = damage;
            best_move.value = move;
        }
    }
    return best_move;
}

int calculate_hp_stat(
    const uint8_t base,
    const uint8_t iv,
    const u_int16_t ev
) {
    return ((2 * base + iv + (ev / 4)) * 100) / 100 + LEVEL + 10;
}

int calculate_non_hp_stat(
    const Stat stat,
    const uint8_t base,
    const uint8_t iv,
    const u_int16_t ev,
    const Nature nature
) {
    const int intermediate = ((2 * base + iv + (ev / 4)) * LEVEL) / 100 + 5;
    double nature_multiplier = 1.0;
    if (nature.down == stat) {
        nature_multiplier = 0.9;
    } else if (nature.up == stat) {
        nature_multiplier = 1.1;
    }
    return static_cast<int>(std::floor(intermediate * nature_multiplier));
}

uint16_t get_stat(
    const Stat stat,
    const uint8_t base,
    const uint8_t iv,
    const u_int16_t ev,
    const Nature nature
) {
    if (stat == Stat::HEALTH) {
        return calculate_hp_stat(base, iv, ev);
    }
    return calculate_non_hp_stat(
        stat,
        base,
        iv,
        ev,
        nature
    );
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
        get_all_serebii_moves(serebii_pokemon);
    for (const auto& [form, form_moves] : moves) {
        customs[form] =
        {
            .name = serebii_pokemon.name,
            .item = "",
            .moves = form_moves,
            .stats = get_stats_for_serebii(form, serebii_pokemon)
        };
    }
    return customs;
}

CustomPokemon convert_hall_to_custom(const BattleHallPokemon& hall_poke) {
    // TODO
}

void analyze() {
    const auto hall_pokemon = get_battle_hall_pokemon();
    const auto serebii_pokemon =
        get_serebii_pokemon();
    const auto all_moves =
        get_all_pokemon_moves(serebii_pokemon);

    // std::unordered_set<Move> all_move_enums;
    // for (int i = 0; i < static_cast<int>(Move::Count); ++i) {
    //     all_move_enums.insert(static_cast<Move>(i));
    // }
    // for (const auto& move_info : all_moves) {
    //     all_move_enums.erase(move_map[move_info.name]);
    // }

    for (const auto& serebii_poke :
         serebii_pokemon | std::ranges::views::values
    ) {
        const auto player_forms =
            convert_serebii_to_custom(serebii_poke);
        for (const auto& player: player_forms | std::views::values) {
            for (const auto& [
                     group_number,
                     group
                 ] : hall_pokemon
            ) {
                for (const auto& hall_poke : group) {
                    const auto opponent = convert_hall_to_custom(hall_poke);
                    const auto serebii_move =
                        get_best_move_against_defender(
                            player,
                            opponent
                        );
                }
                printf("");
            }
        }
    }
}
