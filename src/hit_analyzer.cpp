#include "hit_analyzer.h"

#include <algorithm>
#include <format>
#include <future>
#include <iostream>
#include <ranges>

#include "battle_hall_data_source.h"
#include "battle_simulator.h"
#include "custom_pokemon.h"
#include "serebii_pokemon_data_source.h"
#include "thread_pool.h"

struct BattleEntry {
    CustomPokemon player;
    CustomPokemon opponent;
    PokemonType type;
    uint8_t rank;
    uint8_t over_2;
};

struct ResultEntry {
    PokemonType type;
    uint8_t rank;
    uint8_t over_2;
    CustomPokemon opponent;
    bool won;
    std::vector<const MoveInfo*> moves;
};
struct ResultEntryHash {
    std::size_t operator()(const ResultEntry& entry) const {
        return std::hash<int>{}(static_cast<int>(entry.opponent.name)) ^
               (std::hash<int>{}(static_cast<int>(entry.opponent.ability)) << 1);
    }
};

struct ResultEntryEq {
    bool operator()(const ResultEntry& a, const ResultEntry& b) const {
        return a.opponent.name == b.opponent.name &&
               a.opponent.ability == b.opponent.ability;
    }
};
void do_battle(
    const BattleEntry& battle_entry,
    std::promise<ResultEntry>&& promise
) {
    const auto& [won, moves] =
        battle(battle_entry.player, battle_entry.opponent);
    promise.set_value(
        {
            battle_entry.type,
            battle_entry.rank,
            battle_entry.over_2,
            battle_entry.opponent,
            won,
            moves
        }
    );
}

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
    std::unordered_map<
        PokemonType,
        std::unordered_map<
            uint8_t,
            std::unordered_map<
                uint8_t, std::unordered_set<
                    CustomPokemon,
                    CustomPokemonHash,
                    CustomPokemonEq
                >
            >
        >
    > type_to_rank_to_over_2{};
    for (uint8_t group_number = 4; group_number > 0; group_number--) {
        const auto& rank_to_over_2_ =
            group_to_rank_to_over_2.at(group_number);
        for (const uint8_t rank :
             std::ranges::reverse_view(GROUP_TO_RANKS.at(group_number))
        ) {
            const auto& over_2_to_hall_pokemon =
                rank_to_over_2_.at(rank);
            for (int8_t over_2 = 17; over_2 >= 0; over_2--) {
                const auto& hall_pokemon =
                    over_2_to_hall_pokemon.at(over_2);
                for (const auto& opponent_pokemon : hall_pokemon) {
                    for (const auto type : opponent_pokemon.types) {
                        type_to_rank_to_over_2[type][rank][over_2].insert(
                            opponent_pokemon
                        );
                    }
                }
            }
        }
    }

    std::vector<BattleEntry> battles{};
    for (const auto [
             type,
             rank_to_over_2
         ] : type_to_rank_to_over_2
    ) {
        if (type == PokemonType::COUNT) {
            continue;
        }
        for (int8_t rank = 10; rank > 0; rank--) {
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
                            if (type == PokemonType::NORMAL) {
                                battles.emplace_back(
                                    player_pokemon,
                                    opponent_pokemon,
                                    type,
                                    rank,
                                    over_2
                                );
                            }
                        }
                    }
                }
            }
        }
    }

    std::unordered_map<
        PokemonType,
        std::unordered_map<
            uint8_t,
            std::unordered_map<
                uint8_t, std::unordered_set<
                    ResultEntry,
                    ResultEntryHash,
                    ResultEntryEq
                >
            >
        >
    > type_to_rank_to_over_2_losses{};
    std::unordered_set<const MoveInfo*> used_moves{};
    for (auto& results :
         thread_pool::ThreadPool::getCPUWorkInstance()->
         createAndRunTasks<ResultEntry, std::vector<BattleEntry>, BattleEntry>(
             do_battle,
             battles
         )
    ) {
        const auto& [
            type,
            rank,
            over_2,
            opponent,
            won,
            moves
        ] = results;
        if (!won) {
            type_to_rank_to_over_2_losses[type][rank][over_2].insert(results);
        } else {
            used_moves.insert_range(moves);
        }
    }

    std::vector<ResultEntry> first_losses;
    for (const auto [
             type,
             rank_to_over_2
         ] : type_to_rank_to_over_2_losses
    ) {
        if (type == PokemonType::COUNT) {
            continue;
        }
        for (int8_t rank = 1; rank <= 10; rank++) {
            if (rank_to_over_2.contains(rank)) {
                const auto& over_2_to_hall_pokemon =
                    rank_to_over_2.at(rank);
                for (int8_t over_2 = 0; over_2 <= 17; over_2++) {
                    if (over_2_to_hall_pokemon.contains(over_2)) {
                        const auto& hall_pokemon =
                            over_2_to_hall_pokemon.at(over_2);
                        for (const auto& result_entry : hall_pokemon) {
                            first_losses.emplace_back(result_entry);
                        }
                        if (hall_pokemon.size() > 0) {
                            break;
                        }
                    }
                }
            }
        }
    }

    std::ranges::sort(
        first_losses,
        [](const ResultEntry& a, const ResultEntry& b) {
            if (a.type != b.type) {
                return a.type < b.type;
            }
            if (a.rank != b.rank) {
                return a.rank < b.rank;
            }
            return a.over_2 < b.over_2;
        }
    );

    printf("Moves used: ");
    for (const auto& move : used_moves) {
        printf(std::format(
                "{}, ",
                move->name
            ).c_str()
        );
    }
    printf("\n");
    for (const auto& [
             type,
             rank,
             over_2,
             opponent,
             won,
             moves
         ] : first_losses
    ) {
        printf(std::format(
                "Type: {}, Rank: {:02}, Over 2: {:02}, {}, Level: {}, Ability: {}, ",
                TYPE_TO_STRING.at(type),
                rank,
                over_2,
                get_pokemon_name(
                    opponent.name
                ),
                opponent.level,
                ABILITY_TO_STRING.at(opponent.ability)
            ).c_str()
        );
        for (const auto& move : moves) {
            printf(std::format(
                    "Move: {}, ",
                    move->name
                ).c_str()
            );
        }
        printf("\n");
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

    constexpr bool all = false;
    if (all) {
        for (const auto& [_, serebii_pokemon] : all_serebii_pokemon) {
            const auto& player_pokemon_forms =
                convert_serebii_to_custom(serebii_pokemon);
            battle_all(group_to_rank_to_over_2, player_pokemon_forms);
        }
    } else {
        auto player_pokemon_forms =
            convert_serebii_to_custom(all_serebii_pokemon.at("Azurill"));
        for (auto& [
                 form,
                 p_list
             ] : player_pokemon_forms
        ) {
            for (auto& p : p_list) {
                p.stats[0] = 70;
                p.stats[1] = 40;
                p.stats[2] = 29;
                p.stats[3] = 17;
                p.stats[4] = 29;
                p.stats[5] = 30;
                p.item = Item::FocusSash;
                p.ability = Ability::HugePower;
            }
        }
        battle_all(group_to_rank_to_over_2, player_pokemon_forms);
    }
}
