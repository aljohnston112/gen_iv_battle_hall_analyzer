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
        const auto& [type, streaks] =
            ordered_streak_results[depth];
        index_type_pairs.emplace_back(
            index, std::make_pair(type, streaks.at(index)));
        --depth;
    }

    std::ranges::sort(index_type_pairs);

    std::cout << "Best sum: " << best_sum << "\n";
    for (const auto& [index, pair] :
         index_type_pairs
    ) {
        const auto& [type, streak] = pair;
        std::cout << "Over 2: " << index << ", Type: " << TYPE_TO_STRING.
            at(type)
            << ", Rank: " << streak << "\n";
    }
}

void print_max_streak(
    const std::array<
        std::array<int, NUMBER_OF_TYPES>,
        NUMBER_OF_TYPES
    >& streak_results,
    const std::array<int, NUMBER_OF_TYPES>& lowest_over_2_for_types,
    const std::array<int, NUMBER_OF_TYPES>& type_to_rank_to_skip
) {
    std::vector<std::pair<PokemonType, std::array<int, NUMBER_OF_TYPES>>>
        ordered_streak_results;
    ordered_streak_results.reserve(streak_results.size());
    for (uint8_t type_index = 0; type_index < NUMBER_OF_TYPES; type_index++) {
        const auto type = static_cast<PokemonType>(type_index);
        const auto& arr =
            streak_results[type_index];
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
        std::cout << std::format("{} \n", TYPE_TO_STRING.at(type));
        int i = 0;
        for (const auto& streak : streaks) {
            std::cout << std::format("{}, {} \n", i, streak);
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

                    uint8_t type_index = static_cast<int>(type);
                    if (type_to_rank_to_skip.at(type_index) != -1) {
                        total_streak -= type_to_rank_to_skip.at(type_index);
                    }
                }
            }
        }
    }
    std::ranges::sort(assignments);
    std::cout << "\nTotal streak: " << total_streak << "\n";
    for (const auto& [j, pair] : assignments) {
        const auto& [type, rank] = pair;
        const auto lowest_over_2 =
            lowest_over_2_for_types[static_cast<int>(type)];
        std::cout << "Over 2: "
            << std::max(j, lowest_over_2)
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

std::array<int, NUMBER_OF_TYPES> get_lowest_over_2_for_all_types(
    const std::array<int, NUMBER_OF_TYPES>& type_to_rank_to_skip
) {
    std::array<int, NUMBER_OF_TYPES> lowest_over_2_for_types{};
    for (int8_t i = 0; i < NUMBER_OF_TYPES; i++) {
        int lowest_over_2_for_type = 0;
        for (uint8_t j = 0; j < NUMBER_OF_TYPES; j++) {
            if (i != j && type_to_rank_to_skip[i] >= 2) {
                lowest_over_2_for_type++;
            }
        }
        if (lowest_over_2_for_type == 0) {
            lowest_over_2_for_type = -1;
        }
        lowest_over_2_for_types[i] =
            std::min(NUMBER_OF_TYPES - 1, lowest_over_2_for_type);
    }
    return lowest_over_2_for_types;
}

std::array<
    std::array<
        std::array<
            std::unordered_set<
                CustomPokemon,
                CustomPokemonHash,
                CustomPokemonEq
            >,
            NUMBER_OF_TYPES
        >,
        MAX_RANK
    >,
    NUMBER_OF_TYPES
> aggregate_hall_pokemon(
    const std::array<
        std::array<
            std::array<std::vector<CustomPokemon>, NUMBER_OF_TYPES>,
            MAX_RANK
        >,
        NUMBER_OF_GROUPS
    >& group_to_rank_to_over_2_to_hall_pokemon,
    const std::array<int, NUMBER_OF_TYPES>& lowest_over_2_for_types
) {
    std::array<
        std::array<
            std::array<
                std::unordered_set<
                    CustomPokemon,
                    CustomPokemonHash,
                    CustomPokemonEq
                >,
                NUMBER_OF_TYPES
            >,
            MAX_RANK
        >,
        NUMBER_OF_TYPES
    > type_to_rank_to_over_2{};
    for (uint8_t group_number = 4; group_number > 0; group_number--) {
        const auto& rank_to_over_2_ =
            group_to_rank_to_over_2_to_hall_pokemon.at(group_number - 1);
        for (const uint8_t rank :
             std::ranges::reverse_view(GROUP_TO_RANKS.at(group_number))
        ) {
            const auto& over_2_to_hall_pokemon =
                rank_to_over_2_.at(rank - 1);
            for (int8_t over_2 = NUMBER_OF_TYPES - 1; over_2 >= 0; over_2--) {
                const auto& hall_pokemon =
                    over_2_to_hall_pokemon.at(over_2);
                for (const auto& opponent_pokemon : hall_pokemon) {
                    for (const auto type : opponent_pokemon.types) {
                        if (type == PokemonType::COUNT) {
                            continue;
                        }
                        const auto lowest_over_2 =
                            lowest_over_2_for_types[static_cast<int>(type)];
                        if (over_2 >= lowest_over_2) {
                            type_to_rank_to_over_2[
                                static_cast<int>(type)
                            ][rank - 1][over_2].insert(opponent_pokemon);
                        }
                    }
                }
            }
        }
    }
    return type_to_rank_to_over_2;
}

std::vector<BattleEntry> initialize_battles(
    const std::array<
        std::array<
            std::array<
                std::unordered_set<
                    CustomPokemon,
                    CustomPokemonHash,
                    CustomPokemonEq
                >,
                NUMBER_OF_TYPES
            >,
            MAX_RANK
        >,
        NUMBER_OF_TYPES
    >& type_to_rank_to_over_2,
    const std::array<int, NUMBER_OF_TYPES>& type_to_rank_to_skip,
    const std::unordered_map<
        std::string,
        std::vector<CustomPokemon>
    >& player_pokemon_forms
) {
    std::vector<BattleEntry> battles{};
    for (uint8_t i = 0; i < NUMBER_OF_TYPES; i++) {
        auto type = static_cast<PokemonType>(i);
        const auto& rank_to_over_2 = type_to_rank_to_over_2[i];

        if (type == PokemonType::COUNT) {
            continue;
        }
        int lowest_rank = 0;
        lowest_rank = std::max(type_to_rank_to_skip[i], 0);
        for (int8_t rank = 10; rank > lowest_rank; rank--) {
            if (!rank_to_over_2[rank - 1].empty()) {
                const auto& over_2_to_hall_pokemon =
                    rank_to_over_2.at(rank - 1);
                for (int8_t over_2 = NUMBER_OF_TYPES - 1;
                     over_2 >= 0;
                     over_2--
                ) {
                    if (!over_2_to_hall_pokemon[over_2].empty()) {
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
    return battles;
}

std::pair<
    std::array<
        std::array<
            std::array<
                std::unordered_set<
                    ResultEntry,
                    ResultEntryHash,
                    ResultEntryEq
                >,
                NUMBER_OF_TYPES
            >,
            MAX_RANK
        >,
        NUMBER_OF_TYPES
    >,
    std::unordered_map<const MoveInfo*, int>
> simulate_battles(
    const std::vector<BattleEntry>& battles
) {
    std::array<
        std::array<
            std::array<
                std::unordered_set<
                    ResultEntry,
                    ResultEntryHash,
                    ResultEntryEq
                >,
                NUMBER_OF_TYPES
            >,
            MAX_RANK
        >,
        NUMBER_OF_TYPES
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
        const uint8_t type_index = static_cast<int>(type);
        if (!won) {
            type_to_rank_to_over_2_losses[
                type_index
            ][rank - 1][over_2].insert(results);
        } else {
            for (const auto& move : moves) {
                if (used_moves.contains(move)) {
                    used_moves[move]++;
                } else {
                    used_moves[move] = 1;
                }
            }
        }
    }
    return {type_to_rank_to_over_2_losses, used_moves};
}

std::pair<
    std::vector<ResultEntry>,
    std::array<
        std::array<int, NUMBER_OF_TYPES>,
        NUMBER_OF_TYPES
    >
> get_battle_results(
    const std::array<
        std::array<
            std::array<
                std::unordered_set<
                    ResultEntry,
                    ResultEntryHash,
                    ResultEntryEq
                >,
                NUMBER_OF_TYPES
            >,
            MAX_RANK
        >,
        NUMBER_OF_TYPES
    >& type_to_rank_to_over_2_losses,
    const std::array<int, NUMBER_OF_TYPES>& lowest_over_2_for_types
) {
    std::vector<ResultEntry> first_losses;
    std::array<
        std::array<int, NUMBER_OF_TYPES>,
        NUMBER_OF_TYPES
    > streak_results{};
    for (int type_index = 0; type_index < NUMBER_OF_TYPES; type_index++) {
        const auto& rank_to_over_2 =
            type_to_rank_to_over_2_losses[type_index];
        auto& streaks = streak_results[type_index];
        streaks.fill(-1);
        auto lowest_over_2 = lowest_over_2_for_types[type_index];
        const bool include_all = lowest_over_2 < 0;
        lowest_over_2 = std::max(0, lowest_over_2);
        for (int rank = 1; rank <= 10; rank++) {
            const auto& over_2_to_hall_pokemon =
                rank_to_over_2.at(rank - 1);
            for (int over_2 = lowest_over_2;
                 over_2 < NUMBER_OF_TYPES;
                 over_2++
            ) {
                const auto& hall_pokemon =
                    over_2_to_hall_pokemon.at(over_2);
                for (const auto& result_entry : hall_pokemon) {
                    first_losses.emplace_back(result_entry);
                }
                if (!hall_pokemon.empty()) {
                    break;
                }
                if (over_2 > lowest_over_2 || include_all) {
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
    return {first_losses, streak_results};
}

void print_used_moves(
    const std::unordered_map<const MoveInfo*, int>& used_moves
) {
    printf("Moves used: ");
    for (const auto& [move, times_used] : used_moves) {
        std::cout << std::format(
            "{}: {}, ",
            move->name,
            times_used
        );
    }
    printf("\n");
}

void print_walls(
    const std::array<int, NUMBER_OF_TYPES>& lowest_over_2_for_types,
    const std::vector<ResultEntry>& first_losses
) {
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
        const auto lowest_over_2 =
            lowest_over_2_for_types[static_cast<int>(type)];
        if (over_2 >= lowest_over_2 &&
            (!printed_types.contains(type) ||
                (printed_types.contains(type) &&
                    printed_types.at(type) == rank))
        ) {
            std::cout << std::format(
                "Type: {}, Rank: {:02}, Over 2: {:02}, {}, "
                "Level: {}, Ability: {}, ",
                TYPE_TO_STRING.at(type),
                rank,
                over_2,
                get_pokemon_name(
                    opponent.name
                ),
                opponent.level,
                ABILITY_TO_STRING.at(opponent.ability)
            );
            for (const auto& move : moves) {
                std::cout << std::format(
                    "Move: {}, ",
                    move->name
                );
            }
            printf("\n");
            printed_types[type] = rank;
        }
    }
}

void battle_all(
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
    const std::array<int, NUMBER_OF_TYPES>& type_to_rank_to_skip
) {
    const auto lowest_over_2_for_types =
        get_lowest_over_2_for_all_types(
            type_to_rank_to_skip
        );
    const auto type_to_rank_to_over_2 =
        aggregate_hall_pokemon(
            group_to_rank_to_over_2_to_hall_pokemon,
            type_to_rank_to_skip
        );
    const auto battles = initialize_battles(
        type_to_rank_to_over_2,
        type_to_rank_to_skip,
        player_pokemon_forms
    );

    const auto [
        type_to_rank_to_over_2_losses,
        used_moves
    ] = simulate_battles(battles);


    const auto [
        first_losses,
        streak_results
    ] = get_battle_results(
        type_to_rank_to_over_2_losses,
        lowest_over_2_for_types
    );

    print_used_moves(used_moves);

    print_walls(lowest_over_2_for_types, first_losses);

    print_max_streak(
        streak_results,
        lowest_over_2_for_types,
        type_to_rank_to_skip
    );
}

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
    const std::array<int, NUMBER_OF_TYPES>& type_to_rank_to_skip
) {
    battle_all(
        group_to_rank_to_over_2_to_hall_pokemon,
        player_pokemon_forms,
        type_to_rank_to_skip
    );
}
