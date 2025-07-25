#include "hit_analyzer.h"

#include <algorithm>
#include <cassert>
#include <format>
#include <future>
#include <iostream>
#include <ranges>

#include "hungarian.h"

#include "battle_hall_data_source.h"
#include "BattleState.h"
#include "config.h"
#include "custom_pokemon.h"
#include "serebii_pokemon_data_source.h"
#include "thread_pool.h"

constexpr uint bits_for_max_rank = std::bit_width(MAX_RANK);
constexpr uint bits_for_over_2 = std::bit_width(NUMBER_OF_TYPES);
constexpr uint bits_for_pokemon =
    std::bit_width(static_cast<uint>(Pokemon::Count));


struct PairHash {
    std::size_t operator()(
        const std::pair<unsigned char, unsigned char>& p
    ) const noexcept {
        const std::size_t h1 = std::hash<unsigned char>{}(p.first);
        const std::size_t h2 = std::hash<unsigned char>{}(p.second);
        return h1 ^ (h2 << 8);
    }
};

struct BattleEntry {
    const CustomPokemon& player;
    const CustomPokemon& opponent;
    PokemonType type;
    uint8_t rank;
    uint8_t over_2;
};

struct BattleEntryHash {
    std::size_t operator()(const BattleEntry& entry) const {
        return (static_cast<uint64_t>(entry.type) <<
                (bits_for_level +
                    (bits_for_ability * 2) +
                    (bits_for_pokemon * 2))) |
            ((static_cast<u_int64_t>(entry.opponent.level) <<
                    (((bits_for_ability * 2) + (bits_for_pokemon * 2)))) |
                (static_cast<uint64_t>(entry.opponent.name) <<
                    ((2 * bits_for_ability) + bits_for_pokemon)) |
                (static_cast<uint64_t>(entry.opponent.ability) <<
                    (bits_for_ability + bits_for_pokemon)) |
                (static_cast<uint64_t>(entry.player.name) << bits_for_ability) |
                static_cast<uint64_t>(entry.player.ability));
    }
};

struct BattleEntryEq {
    bool operator()(const BattleEntry& a, const BattleEntry& b) const {
        return a.opponent.name == b.opponent.name &&
            a.opponent.ability == b.opponent.ability &&
            a.player.name == b.player.name &&
            a.player.ability == b.player.ability &&
            a.opponent.level == b.opponent.level &&
            a.type == b.type;
    }
};

struct ResultEntry {
    PokemonType type;
    uint8_t rank;
    uint8_t over_2;
    const CustomPokemon* opponent;
    bool won;
    std::vector<const MoveInfo*> moves;
};

struct ResultEntryHash {
    std::size_t operator()(const ResultEntry& entry) const {
        return std::hash<int>{}(static_cast<int>(entry.opponent->name)) ^
            (std::hash<int>{}(static_cast<int>(entry.opponent->ability)) << 1);
    }
};

struct ResultEntryEq {
    bool operator()(const ResultEntry& a, const ResultEntry& b) const {
        return a.opponent->name == b.opponent->name &&
            a.opponent->ability == b.opponent->ability;
    }
};

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
                const CustomPokemon*,
                CustomPokemonHash,
                CustomPokemonEq
            >,
            NUMBER_OF_TYPES
        >,
        MAX_RANK
    >,
    NUMBER_OF_TYPES
> filter_and_aggregate_hall_pokemon(
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
                    const CustomPokemon*,
                    CustomPokemonHash,
                    CustomPokemonEq
                >,
                NUMBER_OF_TYPES
            >,
            MAX_RANK
        >,
        NUMBER_OF_TYPES
    > type_to_rank_to_over_2_to_hall_pokemon{};
    for (uint8_t group_number = 1; group_number <= 4; group_number++) {
        const auto& rank_to_over_2_to_hall_pokemon =
            group_to_rank_to_over_2_to_hall_pokemon.at(group_number - 1);
        for (const uint8_t rank : GROUP_TO_RANKS.at(group_number)) {
            const auto& over_2_to_hall_pokemon =
                rank_to_over_2_to_hall_pokemon.at(rank - 1);
            for (int8_t over_2 = 0; over_2 < NUMBER_OF_TYPES; over_2++) {
                const auto& hall_pokemon =
                    over_2_to_hall_pokemon.at(over_2);
                for (const auto& opponent_pokemon : hall_pokemon) {
                    for (const auto type : opponent_pokemon.types) {
                        if (type == PokemonType::COUNT) {
                            continue;
                        }
                        const auto type_index = static_cast<uint8_t>(type);
                        const auto lowest_over_2 =
                            lowest_over_2_for_types[type_index];
                        if (over_2 >= lowest_over_2) {
                            type_to_rank_to_over_2_to_hall_pokemon[
                                type_index
                            ][rank - 1][over_2].insert(&opponent_pokemon);
                        }
                    }
                }
            }
        }
    }
    return type_to_rank_to_over_2_to_hall_pokemon;
}

std::unordered_map<
    Pokemon,
    std::vector<BattleEntry>
> initialize_battles(
    const std::array<
        std::array<
            std::array<
                std::unordered_set<
                    const CustomPokemon*,
                    CustomPokemonHash,
                    CustomPokemonEq
                >,
                NUMBER_OF_TYPES
            >,
            MAX_RANK
        >,
        NUMBER_OF_TYPES
    >& type_to_rank_to_over_2_to_hall_pokemon,
    const std::unordered_map<
        std::string,
        std::vector<CustomPokemon>
    >& player_pokemon_forms
) {
    std::unordered_map<
        Pokemon,
        std::vector<BattleEntry>
    > battles{};
    for (uint8_t type_index = 0; type_index < NUMBER_OF_TYPES; type_index++) {
        const auto type = static_cast<PokemonType>(type_index);
        const auto& rank_to_over_2_to_hall_pokemon =
            type_to_rank_to_over_2_to_hall_pokemon[type_index];
        for (uint8_t rank = 1; rank <= 10; rank++) {
            const auto& over_2_to_hall_pokemon =
                rank_to_over_2_to_hall_pokemon.at(rank - 1);
            for (uint8_t over_2 = 0; over_2 < NUMBER_OF_TYPES; over_2++) {
                const auto& hall_pokemon =
                    over_2_to_hall_pokemon.at(over_2);
                for (const auto& opponent_pokemon : hall_pokemon) {
                    for (const auto& player_pokemon_nature_variants :
                         player_pokemon_forms | std::views::values
                    ) {
                        for (const auto& player_pokemon :
                             player_pokemon_nature_variants
                        ) {
                            battles[player_pokemon.name].emplace_back(
                                BattleEntry{
                                    .player = player_pokemon,
                                    .opponent = *opponent_pokemon,
                                    .type = type,
                                    .rank = rank,
                                    .over_2 = over_2
                                }
                            );
                        }
                    }
                }
            }
        }
    }
    return battles;
}

void do_battle(
    const BattleEntry& battle_entry,
    std::promise<ResultEntry>&& promise
) {
    auto [won, moves] =
        battle(battle_entry.player, battle_entry.opponent);
    promise.set_value(
        ResultEntry{
            .type = battle_entry.type,
            .rank = battle_entry.rank,
            .over_2 = battle_entry.over_2,
            .opponent = &battle_entry.opponent,
            .won = won,
            .moves = std::move(moves)
        }
    );
}

std::pair<
    std::unordered_map<
        Pokemon,
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
        >
    >,
    std::unordered_map<
        Pokemon,
        std::unordered_map<const MoveInfo*, int>
    >
> simulate_battles(
    const std::unordered_map<
        Pokemon,
        std::vector<BattleEntry>
    >& form_to_battles
) {
    std::unordered_map<
        Pokemon,
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
        >
    > form_to_type_to_rank_to_over_2_to_losses{};
    std::unordered_map<
        Pokemon,
        std::unordered_map<const MoveInfo*, int>
    > used_moves{};
    for (const auto& [form, battles] :
         form_to_battles
    ) {
        const auto battle_result_entries =
            thread_pool::ThreadPool::getCPUWorkInstance()->
            createAndRunTasks<ResultEntry, std::vector<BattleEntry>,
                              BattleEntry>(
                do_battle,
                battles
            );
        for (auto& battle_result_entry : battle_result_entries) {
            const auto& [
                type,
                rank,
                over_2,
                opponent,
                won,
                moves
            ] = battle_result_entry;
            const auto type_index = static_cast<uint8_t>(type);
            if (!won) {
                form_to_type_to_rank_to_over_2_to_losses[form][
                    type_index
                ][rank - 1][over_2].insert(battle_result_entry);
            } else {
                for (const auto& move : moves) {
                    if (used_moves[form].contains(move)) {
                        used_moves[form][move]++;
                    } else {
                        used_moves[form][move] = 1;
                    }
                }
            }
        }
    }
    return {form_to_type_to_rank_to_over_2_to_losses, used_moves};
}

std::pair<
    std::unordered_map<Pokemon, std::vector<ResultEntry>>,
    std::unordered_map<
        Pokemon,
        std::array<
            std::array<int, NUMBER_OF_TYPES>,
            NUMBER_OF_TYPES
        >
    >
> get_battle_results(
    const std::unordered_map<
        Pokemon,
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
        >
    >& form_to_type_to_rank_to_over_2_to_losses,
    const std::array<int, NUMBER_OF_TYPES>& lowest_over_2_for_types
) {
    std::unordered_map<Pokemon, std::vector<ResultEntry>> form_to_first_losses;
    std::unordered_map<
        Pokemon,
        std::array<
            std::array<int, NUMBER_OF_TYPES>,
            NUMBER_OF_TYPES
        >
    > type_to_over_2_to_streak{};

    for (const auto& [
             form,
             type_to_rank_to_over_2_to_losses
         ] : form_to_type_to_rank_to_over_2_to_losses
    ) {
        for (uint8_t type_index = 0;
             type_index < NUMBER_OF_TYPES;
             type_index++
        ) {
            const auto& rank_to_over_2_to_losses =
                type_to_rank_to_over_2_to_losses[type_index];
            auto& over_2_to_streaks =
                type_to_over_2_to_streak[form][type_index];
            over_2_to_streaks.fill(-1);
            auto lowest_over_2 = lowest_over_2_for_types[type_index];
            lowest_over_2 = std::max(0, lowest_over_2);
            for (int rank = 1; rank <= 10; rank++) {
                const auto& over_2_to_losses =
                    rank_to_over_2_to_losses.at(rank - 1);
                for (int over_2 = lowest_over_2;
                     over_2 < NUMBER_OF_TYPES;
                     over_2++
                ) {
                    const auto& losses =
                        over_2_to_losses.at(over_2);
                    for (const auto& loss : losses) {
                        form_to_first_losses[form].emplace_back(loss);
                    }
                    if (!losses.empty()) {
                        break;
                    }
                    if (over_2 > lowest_over_2 || lowest_over_2 == 0) {
                        over_2_to_streaks[over_2] =
                            std::max(over_2_to_streaks[over_2], rank);
                    }
                }
            }
        }

        std::ranges::sort(
            form_to_first_losses[form],
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
    }
    return {form_to_first_losses, type_to_over_2_to_streak};
}

void print_used_moves(
    const std::unordered_map<const MoveInfo*, int>& used_moves
) {
    std::vector<std::pair<const MoveInfo*, int>> sorted_moves(
        used_moves.begin(), used_moves.end()
    );
    std::ranges::sort(
        sorted_moves,
        [](const auto& a, const auto& b) {
            return a.second > b.second; // max to min
        }
    );

    printf("Moves used: ");
    for (const auto& [move_info, times_used] : sorted_moves) {
        std::cout
            << std::format(
                "{}: {}, ",
                move_info->name,
                times_used
            );
    }
    printf("\n");
}

void print_walls(
    const std::array<int, NUMBER_OF_TYPES>& lowest_over_2_for_types,
    const std::vector<ResultEntry>& first_losses,
    const std::array<
        std::pair<PokemonType, int>,
        NUMBER_OF_TYPES
    >& over_2_to_rank_and_type_assignment
) {
    std::array<
        std::array<
            std::array<
                std::vector<
                    std::pair<
                        const CustomPokemon*,
                        std::vector<const MoveInfo*>
                    >
                >, NUMBER_OF_TYPES>,
            MAX_RANK
        >,
        NUMBER_OF_TYPES
    > type_to_rank_to_over_2_to_losses{};
    for (const auto& [
             type,
             rank,
             over_2,
             opponent,
             won,
             moves
         ] : first_losses
    ) {
        type_to_rank_to_over_2_to_losses[
            static_cast<int>(type)
        ][rank - 1][over_2].emplace_back(opponent, moves);
    }

    if (USE_HIGHEST_RANK_FOR_WALLS) {
        for (uint8_t over_2 = 0; over_2 < NUMBER_OF_TYPES; over_2++) {
            const auto& [type, rank] =
                over_2_to_rank_and_type_assignment[over_2];
            if (rank != 10) {
                const auto& over_2_to_losses =
                    type_to_rank_to_over_2_to_losses[
                        static_cast<int>(type)
                    ][rank == -1 ? 0 : rank];
                for (int i = 0; i < NUMBER_OF_TYPES; i++) {
                    if (const auto& losses = over_2_to_losses[i];
                        !losses.empty()
                    ) {
                        for (const auto& [
                                 opponent,
                                 moves
                             ] : losses
                        ) {
                            std::cout <<
                                std::format(
                                    "Type: {}, Rank: {:02}, Over 2: {:02}, {}, "
                                    "Level: {}, Ability: {}, ",
                                    TYPE_TO_STRING.at(type),
                                    rank + 1,
                                    over_2,
                                    get_pokemon_name(
                                        opponent->name
                                    ),
                                    opponent->level,
                                    ABILITY_TO_STRING.at(opponent->ability)
                                );
                            for (const auto& move : moves) {
                                std::cout <<
                                    std::format(
                                        "Move: {}, ",
                                        move->name
                                    );
                            }
                            printf("\n");
                        }
                        break;
                    }
                }
            }
        }
    } else {
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
                std::cout <<
                    std::format(
                        "Type: {}, Rank: {:02}, Over 2: {:02}, {}, "
                        "Level: {}, Ability: {}, ",
                        TYPE_TO_STRING.at(type),
                        rank,
                        over_2,
                        get_pokemon_name(
                            opponent->name
                        ),
                        opponent->level,
                        ABILITY_TO_STRING.at(opponent->ability)
                    );
                for (const auto& move : moves) {
                    std::cout <<
                        std::format(
                            "Move: {}, ",
                            move->name
                        );
                }
                printf("\n");
                printed_types[type] = rank;
            }
        }
    }
}

std::vector<
    std::pair<PokemonType, std::array<int, NUMBER_OF_TYPES>>
> get_ordered_type_and_over_2_to_streak_pairs(
    const std::array<
        std::array<int, NUMBER_OF_TYPES>,
        NUMBER_OF_TYPES
    >& type_to_over_2_to_streak
) {
    std::vector<
        std::pair<PokemonType, std::array<int, NUMBER_OF_TYPES>>
    > ordered_type_and_over_2_to_streak_pairs{};
    ordered_type_and_over_2_to_streak_pairs.reserve(
        type_to_over_2_to_streak.size());
    for (uint8_t type_index = 0; type_index < NUMBER_OF_TYPES; type_index++) {
        const auto type = static_cast<PokemonType>(type_index);
        const auto& over_2_to_streak =
            type_to_over_2_to_streak[type_index];
        ordered_type_and_over_2_to_streak_pairs.emplace_back(
            type,
            over_2_to_streak
        );
    }
    // Order them by what rank they can reach
    // and then by what over_2 they lose at
    std::ranges::sort(
        ordered_type_and_over_2_to_streak_pairs,
        [](const auto& a, const auto& b) {
            if (a.second[0] == b.second[0]) {
                // Same rank at over_2 of 0
                // Count the number of over_2s where at least rank 1 can be beat
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
    return ordered_type_and_over_2_to_streak_pairs;
}

void print_streaks(
    const std::vector<std::pair<PokemonType, std::array<int, NUMBER_OF_TYPES>>>&
    ordered_type_and_over_2_to_streak_pairs
) {
    for (const auto& [type, streaks] :
         ordered_type_and_over_2_to_streak_pairs
    ) {
        std::cout << std::format("{} \n", TYPE_TO_STRING.at(type));
        int over_2 = 0;
        for (const auto& streak : streaks) {
            std::cout << std::format("{}, {} \n", over_2, streak);
            over_2++;
        }
    }
}

int** create_streak_matrix(
    std::vector<
        std::pair<PokemonType, std::array<int, NUMBER_OF_TYPES>>
    > ordered_type_and_over_2_to_streak_pairs,
    const uint8_t number_of_rows,
    const uint8_t number_of_columns
) {
    std::vector<int> flat_type_to_over_2_to_streaks{};
    flat_type_to_over_2_to_streaks.reserve(
        number_of_rows * number_of_columns
    );
    for (const auto& over_2_to_streak :
         ordered_type_and_over_2_to_streak_pairs | std::views::values
    ) {
        flat_type_to_over_2_to_streaks.insert_range(
            flat_type_to_over_2_to_streaks.end(),
            over_2_to_streak
        );
    }
    return array_to_matrix(
        flat_type_to_over_2_to_streaks.data(),
        number_of_rows,
        number_of_columns
    );
}

std::pair<
    int,
    std::array<
        std::pair<PokemonType, int>,
        NUMBER_OF_TYPES
    >
> calculate_over_2_assignments(
    const std::vector<
        std::pair<PokemonType, std::array<int, NUMBER_OF_TYPES>>
    >& ordered_type_and_over_2_to_streak_pairs,
    const std::array<int, NUMBER_OF_TYPES>& type_to_rank_to_skip
) {
    // libhungarian requires a C style array
    const auto number_of_rows =
        static_cast<uint8_t>(ordered_type_and_over_2_to_streak_pairs.size());
    constexpr int number_of_columns = NUMBER_OF_TYPES;
    const auto streak_matrix =
        create_streak_matrix(
            ordered_type_and_over_2_to_streak_pairs,
            number_of_rows,
            number_of_columns
        );

    // Run the hungarian algorithm to assign each type to a unique over_2
    // in a way that maximizes total streak
    hungarian_problem_t hungarian_problem;
    hungarian_init(
        &hungarian_problem,
        streak_matrix,
        number_of_rows,
        number_of_columns,
        HUNGARIAN_MODE_MAXIMIZE_UTIL
    );
    hungarian_solve(&hungarian_problem);

    // Calculate total streak and store the over_2 assignment for each type
    std::array<
        std::pair<PokemonType, int>,
        number_of_columns
    > over_2_to_type_and_rank{};
    uint16_t total_streak = 0;
    for (uint8_t row = 0; row < number_of_rows; row++) {
        for (uint8_t over_2 = 0; over_2 < number_of_columns; over_2++) {
            if (hungarian_problem.assignment[row][over_2] == 1) {
                auto [type, over_2_to_streak] =
                    ordered_type_and_over_2_to_streak_pairs.at(row);
                int rank = over_2_to_streak.at(over_2);
                over_2_to_type_and_rank[over_2] = std::make_pair(type, rank);
                if (rank > -1) {
                    total_streak += rank;
                    if (const uint8_t type_index = static_cast<int>(type);
                        type_to_rank_to_skip.at(type_index) != -1
                    ) {
                        total_streak -= type_to_rank_to_skip.at(type_index);
                    }
                }
            }
        }
    }
    hungarian_free(&hungarian_problem);
    free(streak_matrix);
    return {total_streak, over_2_to_type_and_rank};
}

void print_max_streaks(
    const std::array<
        std::pair<PokemonType, int>,
        NUMBER_OF_TYPES
    >& type_to_over_2_assignment,
    const std::array<int, NUMBER_OF_TYPES>& lowest_over_2_for_types
) {
    // Print ranks beaten in the order they should be challenged
    int over_2 = 0;
    for (const auto& [type, rank] :
         type_to_over_2_assignment
    ) {
        const auto lowest_over_2 =
            lowest_over_2_for_types[static_cast<int>(type)];
        std::cout << "Over 2: "
            << std::max(over_2, lowest_over_2)
            << ", Type: " << TYPE_TO_STRING.at(type)
            << ", Rank: " << rank << "\n";
        over_2++;
    }

    for (const auto& [type, rank] :
         type_to_over_2_assignment
    ) {
        const auto& s = TYPE_TO_STRING.at(type);
        auto upper_view = s | std::views::transform([](const char c) {
            return std::toupper(static_cast<unsigned char>(c));
        });
        std::string upper(upper_view.begin(), upper_view.end());
        std::cout << "{ PokemonType::" << upper + ", " << rank << "},\n";
    }
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
    const auto lowest_over_2_for_types =
        get_lowest_over_2_for_all_types(
            type_to_rank_to_skip
        );
    const auto type_to_rank_to_over_2_to_hall_pokemon =
        filter_and_aggregate_hall_pokemon(
            group_to_rank_to_over_2_to_hall_pokemon,
            lowest_over_2_for_types
        );
    const auto battles =
        initialize_battles(
            type_to_rank_to_over_2_to_hall_pokemon,
            player_pokemon_forms
        );
    const auto [
        form_to_type_to_rank_to_over_2_to_losses,
        form_to_used_moves
    ] = simulate_battles(battles);
    const auto [
        form_to_first_losses,
        type_to_over_2_to_streak
    ] = get_battle_results(
        form_to_type_to_rank_to_over_2_to_losses,
        lowest_over_2_for_types
    );

    for (const auto& form :
         form_to_type_to_rank_to_over_2_to_losses | std::views::keys
    ) {
        print_used_moves(form_to_used_moves.at(form));

        const auto ordered_type_and_over_2_to_streak_pairs =
            get_ordered_type_and_over_2_to_streak_pairs(
                type_to_over_2_to_streak.at(form)
            );
        if (FULL_PRINT) {
            print_streaks(ordered_type_and_over_2_to_streak_pairs);
        }
        const auto [
            total_streak,
            over_2_to_rank_and_type_assignment
        ] = calculate_over_2_assignments(
            ordered_type_and_over_2_to_streak_pairs,
            type_to_rank_to_skip
        );
        if (FULL_PRINT) {
            print_walls(
                lowest_over_2_for_types,
                form_to_first_losses.at(form),
                over_2_to_rank_and_type_assignment
            );
        }
        std::cout
            << POKEMON_TO_STRING.at(form)
            << "'s total streak: " << total_streak << "\n";
        if (FULL_PRINT) {
            print_max_streaks(
                over_2_to_rank_and_type_assignment,
                lowest_over_2_for_types
            );
        }
    }
}
