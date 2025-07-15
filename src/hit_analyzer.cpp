#include "hit_analyzer.h"

#include <algorithm>
#include <format>
#include <future>
#include <iostream>
#include <ranges>
#include <stack>

#include "hungarian.h"

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

void print_best_selection(
    const int best_sum,
    std::stack<int> best_selection,
    const std::vector<
        std::pair<PokemonType, std::array<int, NUMBER_OF_TYPES>>
    >& ordered_streak_results
) {
    std::vector<std::pair<int, std::pair<PokemonType, int>>> index_type_pairs;
    int depth = static_cast<int>(best_selection.size()) - 1;
    while (!best_selection.empty()) {
        int index = best_selection.top();
        best_selection.pop();
        const auto& [type, streaks] = ordered_streak_results[depth];
        index_type_pairs.emplace_back(
            index, std::make_pair(type, streaks.at(index)));
        --depth;
    }

    std::ranges::sort(index_type_pairs);

    std::cout << "Best sum: " << best_sum << "\n";
    for (const auto& [index, pair] : index_type_pairs) {
        const auto& [type, streak] = pair;
        std::cout << "Over 2: " << index << ", Type: " << TYPE_TO_STRING.
            at(type)
            << ", Rank: " << streak << "\n";
    }
}

void print_max_streak(
    const std::unordered_map<
        PokemonType,
        std::array<int, NUMBER_OF_TYPES>
    >& streak_results,
    const int lowest_over_two,
    std::unordered_map<PokemonType, int> type_to_rank_to_skip
) {
    std::vector<std::pair<PokemonType, std::array<int, NUMBER_OF_TYPES>>>
        ordered_streak_results;
    ordered_streak_results.reserve(streak_results.size());
    for (const auto& [type, arr] :
         streak_results
    ) {
        ordered_streak_results.emplace_back(type, arr);
    }
    std::ranges::sort(
        ordered_streak_results,
        [](const auto& a, const auto& b) {
            if (a.second[0] == b.second[0]) {
                int ai = -1;
                for (int i = 0; i < NUMBER_OF_TYPES; i++) {
                    ai++;
                    if (a.second[i] == -1) {
                        break;
                    }
                }
                int bi = -1;
                for (int i = 0; i < NUMBER_OF_TYPES; i++) {
                    bi++;
                    if (b.second[i] == -1) {
                        break;
                    }
                }
                return ai < bi;
            }
            return a.second[0] > b.second[0];
        }
    );

    for (const auto& [type, streaks] :
         ordered_streak_results
    ) {
        printf(std::format("{} \n", TYPE_TO_STRING.at(type)).c_str());
        int i = 0;
        for (const auto& streak : streaks) {
            printf(std::format("{}, {} \n", i, streak).c_str());
            i++;
        }
    }

    // libhungarian requires a C style array
    std::vector<int> flat_streaks;
    const int rows = static_cast<int>(ordered_streak_results.size());
    flat_streaks.reserve(rows * NUMBER_OF_TYPES);
    for (const auto& streaks :
         ordered_streak_results | std::views::values
    ) {
        flat_streaks.insert(
            flat_streaks.end(),
            streaks.begin(),
            streaks.end()
        );
    }
    int** streak_matrix = array_to_matrix(
        flat_streaks.data(),
        rows,

        NUMBER_OF_TYPES
    );

    hungarian_problem_t p;
    hungarian_init(
        &p,
        streak_matrix,
        rows, NUMBER_OF_TYPES,
        HUNGARIAN_MODE_MAXIMIZE_UTIL
    );
    hungarian_solve(&p);

    std::vector<std::pair<int, std::pair<PokemonType, int>>> assignments;
    int total_streak = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < NUMBER_OF_TYPES; j++) {
            if (p.assignment[i][j] == 1) {
                auto [type, streaks] =
                    ordered_streak_results.at(i);
                int rank = streaks.at(j);
                assignments.emplace_back(
                    j,
                    std::make_pair(type, rank)
                );
                if (rank > -1) {
                    total_streak += rank;
                    if (type_to_rank_to_skip.contains(type) &&
                        type_to_rank_to_skip.at(type) != -1
                    ) {
                        total_streak -= type_to_rank_to_skip.at(type);
                    }
                }
            }
        }
    }
    std::ranges::sort(assignments);
    std::cout << "\nTotal streak: " << total_streak << "\n";
    for (const auto& [j, pair] : assignments) {
        const auto& [type, rank] = pair;
        std::cout << "Over 2: " << std::max(j, lowest_over_two)
            << ", Type: " << TYPE_TO_STRING.at(type)
            << ", Rank: " << rank << "\n";
    }

    for (const auto& pair : assignments | std::views::values) {
        const auto& [type, rank] = pair;
        const auto& s = TYPE_TO_STRING.at(type);
        auto upper_view = s | std::views::transform([](char c) {
            return std::toupper(static_cast<unsigned char>(c));
        });
        std::string upper(upper_view.begin(), upper_view.end());

        std::cout << "{ PokemonType::" << upper + ", " << rank << "},\n";
    }
    hungarian_free(&p);
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
    >& player_pokemon_forms,
    std::unordered_map<PokemonType, int> type_to_rank_to_skip
) {
    int lowest_over_2 = 0;
    for (const auto& rank :
         type_to_rank_to_skip | std::views::values
    ) {
        if (rank >= 2) {
            lowest_over_2++;
        }
    }
    lowest_over_2 = std::min(NUMBER_OF_TYPES - 1, lowest_over_2);

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
            for (int8_t over_2 = NUMBER_OF_TYPES - 1; over_2 >= 0; over_2--) {
                if (over_2 >= lowest_over_2) {
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
    }

    std::vector<BattleEntry> battles{};
    for (const auto& [
             type,
             rank_to_over_2
         ] : type_to_rank_to_over_2
    ) {
        if (type == PokemonType::COUNT) {
            continue;
        }
        int lowest_rank = 0;
        if (type_to_rank_to_skip.contains(type)) {
            lowest_rank = type_to_rank_to_skip.at(type);
        }
        for (int8_t rank = 10; rank > lowest_rank; rank--) {
            if (rank_to_over_2.contains(rank)) {
                const auto& over_2_to_hall_pokemon =
                    rank_to_over_2.at(rank);
                for (int8_t over_2 = NUMBER_OF_TYPES - 1;
                     over_2 >= 0;
                     over_2--
                ) {
                    if (over_2_to_hall_pokemon.contains(over_2)) {
                        const auto& hall_pokemon =
                            over_2_to_hall_pokemon.at(over_2);
                        for (const auto& opponent_pokemon : hall_pokemon) {
                            for (const auto& player_pokemon_ :
                                 player_pokemon_forms | std::views::values
                            ) {
                                for (const auto& player_pokemon :
                                     player_pokemon_
                                ) {
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
    std::unordered_map<const MoveInfo*, int> used_moves{};
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
            if (!type_to_rank_to_over_2_losses[type][rank].contains(over_2)) {
                type_to_rank_to_over_2_losses[type][rank][over_2] =
                    std::unordered_set<
                        ResultEntry, ResultEntryHash, ResultEntryEq>{};
            }
            for (const auto& move : moves) {
                if (used_moves.contains(move)) {
                    used_moves[move]++;
                } else {
                    used_moves[move] = 1;
                }
            }
        }
    }

    std::unordered_map<PokemonType, std::array<int, NUMBER_OF_TYPES>>
        streak_results{};
    std::vector<ResultEntry> first_losses;
    for (const auto& [
             type,
             rank_to_over_2
         ] : type_to_rank_to_over_2_losses
    ) {
        if (type == PokemonType::COUNT) {
            continue;
        }
        auto& streaks = streak_results[type];
        streaks.fill(-1);
        for (int rank = 0; rank <= 10; rank++) {
            if (rank_to_over_2.contains(rank)) {
                const auto& over_2_to_hall_pokemon =
                    rank_to_over_2.at(rank);
                for (int over_2 = lowest_over_2;
                     over_2 < NUMBER_OF_TYPES;
                     over_2++
                ) {
                    if (over_2_to_hall_pokemon.contains(over_2)) {
                        const auto& hall_pokemon =
                            over_2_to_hall_pokemon.at(over_2);
                        for (const auto& result_entry : hall_pokemon) {
                            first_losses.emplace_back(result_entry);
                        }
                        if (!hall_pokemon.empty()) {
                            break;
                        }
                        streaks[over_2] = std::max(streaks[over_2], rank);
                    }
                }
            } else if (type_to_rank_to_skip.contains(type) &&
                type_to_rank_to_skip.at(type) != -1 &&
                type_to_rank_to_skip.at(type) < rank
            ) {
                for (int over_2 = lowest_over_2;
                     over_2 < NUMBER_OF_TYPES;
                     over_2++
                ) {
                    streaks[over_2] = std::max(streaks[over_2], rank);
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
    for (const auto& [move, times_used] : used_moves) {
        printf(std::format(
                "{}: {}, ",
                move->name,
                times_used
            ).c_str()
        );
    }
    printf("\n");

    std::unordered_map<PokemonType, int> printed_types{};
    for (const auto& [
             type,
             rank,
             over_2,
             opponent,
             won,
             moves
         ] : first_losses
    ) {
        if (over_2 >= lowest_over_2 &&
            (!printed_types.contains(type) ||
                (printed_types.contains(type) &&
                    printed_types.at(type) == rank))
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
            printed_types[type] = rank;
        }
    }
    print_max_streak(streak_results, lowest_over_2, type_to_rank_to_skip);
}

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
    const std::unordered_map<PokemonType, int>& type_to_rank_to_skip
) {
    battle_all(
        group_to_rank_to_over_2,
        player_pokemon_forms,
        type_to_rank_to_skip
    );
}
