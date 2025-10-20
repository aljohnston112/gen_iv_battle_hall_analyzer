#ifndef ALL_V_ALL_H
#define ALL_V_ALL_H

#include <future>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

#include "BattleState.h"
#include "config.h"
#include "custom_pokemon.h"
#include "serebii_pokemon_data_source.h"
#include "thread_pool.h"

struct BattleAllEntry {
    CustomPokemon player;
    CustomPokemon opponent;
};

inline void battle_all(
    const BattleAllEntry& battle_entry,
    std::promise<BattleResultEntry>&& promise
) {
    auto [
        _,
        won,
        moves,
        opponent_moves
    ] = battle(battle_entry.player, battle_entry.opponent);
    promise.set_value(
        BattleResultEntry{
            .opponent = &battle_entry.opponent,
            .won = won,
            .player_moves = std::move(moves),
            .opponent_moves = std::move(opponent_moves)
        }
    );
}

struct MoveSetHash {
    std::size_t operator()(
        const std::unordered_set<const MoveInfo*>& move_set
    ) const {
        std::vector sorted_moves(move_set.begin(), move_set.end());
        std::sort(sorted_moves.begin(), sorted_moves.end());

        std::size_t hash = 0;
        int i = 0;
        for (const MoveInfo* move : sorted_moves) {
            const int shift = i *
                std::bit_width(static_cast<uint>(Move::Count));
            hash |= static_cast<std::size_t>(move->move) << shift;
            i++;
        }
        return hash;
    }
};

struct PokemonPairHash {
    std::size_t operator()(const std::pair<Pokemon, Ability>& p) const {
        return (static_cast<int>(p.first) << bits_for_ability) |
            static_cast<int>(p.second);
    }
};

inline double get_score(
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        uint64_t,
        PokemonPairHash
    >& times_opponents_beaten,
    const std::unordered_set<
        std::pair<Pokemon, Ability>, PokemonPairHash
    >& beaten
) {
    double score = 0;
    for (const auto& o_pair : beaten) {
        if (times_opponents_beaten[o_pair] != 0) {
            score += 1.0 / static_cast<double>(times_opponents_beaten[o_pair]);
        } else {
            score += 1;
        }
    }
    return score;
}

static const std::unordered_set banned = {
    Pokemon::Palkia,
    Pokemon::Arceus,
    Pokemon::OriginGiratina,
    Pokemon::Dialga,
    Pokemon::Mewtwo,
    Pokemon::AlteredGiratina,
    Pokemon::Lugia,
    Pokemon::Jirachi,
    Pokemon::Groudon,
    Pokemon::HoOh,
    Pokemon::Mew,
    Pokemon::Latias,
    Pokemon::Darkrai,
    Pokemon::Rayquaza,
    Pokemon::Kyogre,
    Pokemon::AttackDeoxys,
    Pokemon::Moltres,
    Pokemon::Salamence,
    Pokemon::Latios,
    Pokemon::Metagross,
    Pokemon::Garchomp,
    Pokemon::Zapdos,
    Pokemon::Flygon,
    Pokemon::Dragonite,
    Pokemon::Articuno,
    Pokemon::Zapdos,
    Pokemon::Entei,
    Pokemon::Raikou,
    Pokemon::Suicune,
    Pokemon::Tyranitar,
    Pokemon::Celebi,
    Pokemon::Regice,
    Pokemon::Regigigas,
    Pokemon::Regirock,
    Pokemon::Registeel,
    Pokemon::DefenseDeoxys,
    Pokemon::NormalDeoxys,
    Pokemon::SpeedDeoxys,
    Pokemon::FanRotom,
    Pokemon::FrostRotom,
    Pokemon::HeatRotom,
    Pokemon::MowRotom,
    Pokemon::WashRotom,
    Pokemon::Uxie,
    Pokemon::Azelf,
    Pokemon::Mesprit,
    Pokemon::Heatran,
    Pokemon::Cresselia,
    Pokemon::Phione,
    Pokemon::Manaphy,
    Pokemon::LandShaymin,
    Pokemon::SkyShaymin,
    Pokemon::Slaking,
    Pokemon::Wobbuffet
};

inline std::unordered_map<
    Pokemon,
    std::vector<CustomPokemon>
> get_all_pokemon_to_analyze(
    const std::unordered_map<
        std::string,
        SerebiiPokemon
    >& name_to_serebii_pokemon
) {
    std::unordered_map<
        Pokemon,
        std::vector<CustomPokemon>
    > pokemon_to_forms{};
    const auto all_pokemon_forms =
        get_pokemon_forms(name_to_serebii_pokemon);
    for (const auto& pokemon_forms : all_pokemon_forms) {
        for (const auto& pokemon_variants :
             pokemon_forms | std::views::values
        ) {
            for (const auto& pokemon : pokemon_variants) {
                if (!banned.contains(pokemon.name)) {
                    pokemon_to_forms[pokemon.name].emplace_back(pokemon);
                }
            }
        }
    }
    return pokemon_to_forms;
}

inline std::unordered_map<
    std::pair<Pokemon, Ability>,
    std::vector<BattleAllEntry>,
    PokemonPairHash
> get_battle_entries(
    std::unordered_map<
        Pokemon,
        std::vector<CustomPokemon>
    > pokemon_to_forms
) {
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<BattleAllEntry>,
        PokemonPairHash
    > pokemon_to_battles{};
    pokemon_to_battles.reserve(
        pokemon_to_forms.size() * pokemon_to_forms.size()
    );
    for (const auto& player_pokemon_forms :
         pokemon_to_forms | std::views::values
    ) {
        for (const auto& player_pokemon : player_pokemon_forms) {
            for (const auto& opponent_pokemon_forms :
                 pokemon_to_forms | std::views::values
            ) {
                for (const auto& opponent_pokemon : opponent_pokemon_forms) {
                    pokemon_to_battles[
                            std::make_pair(
                                player_pokemon.name,
                                player_pokemon.ability
                            )
                        ].
                        emplace_back(
                            BattleAllEntry{
                                .player = player_pokemon,
                                .opponent = opponent_pokemon,
                            }
                        );
                }
            }
        }
    }
    return pokemon_to_battles;
}

inline void aggregate_battle_results(
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::pair<
            int,
            std::unordered_map<
                std::unordered_set<const MoveInfo*>,
                int,
                MoveSetHash
            >
        >,
        PokemonPairHash
    >& pokemon_to_wins_and_moves_with_win_counts,
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::unordered_map<
            std::pair<Pokemon, Ability>,
            std::unordered_set<const MoveInfo*>,
            PokemonPairHash
        >,
        PokemonPairHash
    >& pokemon_to_losses,
    const std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<BattleResultEntry>,
        PokemonPairHash
    >& pokemon_to_battle_result_entries
) {
    for (const auto& [
             player_pokemon,
             battle_result_entries
         ] : pokemon_to_battle_result_entries
    ) {
        for (const auto& [
                 opponent_pokemon,
                 won,
                 moves,
                 opponent_moves
             ] : battle_result_entries
        ) {
            if (won) {
                auto& [
                    wins,
                    move_counts
                ] = pokemon_to_wins_and_moves_with_win_counts[player_pokemon];
                wins++;
                move_counts[moves]++;
            } else {
                pokemon_to_losses[player_pokemon][
                    std::make_pair(opponent_pokemon->name,
                                   opponent_pokemon->ability)
                ] = opponent_moves;
            }
        }
    }
}

inline std::unordered_map<
    std::pair<Pokemon, Ability>,
    std::pair<
        int,
        std::vector<
            std::pair<std::unordered_set<const MoveInfo*>, int>
        >
    >,
    PokemonPairHash
> get_top_2_moves(
    const std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::pair<
            int,
            std::unordered_map<
                std::unordered_set<const MoveInfo*>,
                int,
                MoveSetHash
            >
        >,
        PokemonPairHash
    >& pokemon_to_wins_and_moves_with_win_counts
) {
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::pair<
            int,
            std::vector<
                std::pair<std::unordered_set<const MoveInfo*>, int>
            >
        >,
        PokemonPairHash
    > pokemon_to_wins_and_sorted_moves_with_win_counts{};

    for (const auto& [
             player_pokemon,
             wins_and_move_counts
         ] : pokemon_to_wins_and_moves_with_win_counts
    ) {
        auto [wins, move_counts] =
            wins_and_move_counts;
        std::vector<
            std::pair<std::unordered_set<const MoveInfo*>, int>
        > move_count_vector(
            move_counts.begin(),
            move_counts.end()
        );
        std::sort(
            move_count_vector.begin(),
            move_count_vector.end(),
            [](const auto& a, const auto& b) {
                return b.second < a.second;
            }
        );
        pokemon_to_wins_and_sorted_moves_with_win_counts[player_pokemon] =
            std::make_pair(
                wins,
                std::move(move_count_vector)
            );
    }
    for (auto& [pokemon, wins_and_sorted_move_counts] :
         pokemon_to_wins_and_sorted_moves_with_win_counts
    ) {
        auto& [
            wins,
            sorted_move_counts
        ] = wins_and_sorted_move_counts;
        const uint8_t number_of_moves = 3;
        if (sorted_move_counts.size() > number_of_moves) {
            wins = 0;
            int extra_move_index = -1;
            bool types_are_the_same = true;
            auto type = PokemonType::COUNT;
            for (const auto& move :
                 sorted_move_counts[0].first
            ) {
                if (type == PokemonType::COUNT) {
                    type = move->type;
                } else {
                    types_are_the_same = type == move->type;
                }
                if (!types_are_the_same) {
                    break;
                }
            }
            for (int i = 1; i < number_of_moves; i++) {
                if (types_are_the_same) {
                    for (const auto& move :
                         sorted_move_counts[i].first
                    ) {
                        if (type == PokemonType::COUNT) {
                            type = move->type;
                        } else {
                            types_are_the_same = type == move->type;
                        }
                        if (!types_are_the_same) {
                            break;
                        }
                    }
                }
                if (!types_are_the_same) {
                    break;
                }
            }
            if (types_are_the_same) {
                int i = number_of_moves;
                while (i < sorted_move_counts.size()) {
                    for (const auto& move :
                         sorted_move_counts[i].first
                    ) {
                        if (move->type != type) {
                            extra_move_index = i;
                            break;
                        }
                    }
                    i++;
                }
            }
            if (extra_move_index != -1) {
                const auto move_cont =
                    sorted_move_counts[extra_move_index];
                wins += move_cont.second;
                sorted_move_counts.resize(number_of_moves);
                sorted_move_counts.emplace_back(move_cont);
            } else {
                sorted_move_counts.resize(number_of_moves);
            }
            for (int i = 0; i < number_of_moves; i++) {
                wins += sorted_move_counts[i].second;
            }
        }
    }
    return pokemon_to_wins_and_sorted_moves_with_win_counts;
}

inline void aggregate_second_run(
    const std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<BattleResultEntry>,
        PokemonPairHash
    >& pokemon_to_battle_result_entries,
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::pair<
            int,
            std::vector<
                std::pair<std::unordered_set<const MoveInfo*>, int>
            >
        >,
        PokemonPairHash
    >& pokemon_to_wins_and_sorted_moves_with_win_counts
) {
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::pair<
            int,
            std::unordered_map<
                std::unordered_set<const MoveInfo*>,
                int,
                MoveSetHash
            >
        >,
        PokemonPairHash
    > pokemon_to_wins_and_moves_with_win_counts{};
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::unordered_map<
            std::pair<Pokemon, Ability>,
            std::unordered_set<const MoveInfo*>,
            PokemonPairHash
        >,
        PokemonPairHash
    > pokemon_to_losses{};
    aggregate_battle_results(
        pokemon_to_wins_and_moves_with_win_counts,
        pokemon_to_losses,
        pokemon_to_battle_result_entries
    );

    for (const auto& [
             pokemon,
             wins_and_move_counts
         ] : pokemon_to_wins_and_moves_with_win_counts
    ) {
        auto [wins, move_counts] =
            wins_and_move_counts;
        std::vector<
            std::pair<std::unordered_set<const MoveInfo*>, int>
        > sorted_move_counts(
            move_counts.begin(),
            move_counts.end()
        );
        std::sort(
            sorted_move_counts.begin(),
            sorted_move_counts.end(),
            [](const auto& a, const auto& b) {
                return b.second < a.second;
            }
        );
        pokemon_to_wins_and_sorted_moves_with_win_counts[pokemon] =
            std::make_pair(
                wins,
                std::move(sorted_move_counts)
            );
    }
}

inline void analyze_all(
    const std::unordered_map<
        std::string,
        SerebiiPokemon
    >& pokemon_name_to_serebii_pokemon
) {
    std::unordered_map<
        Pokemon,
        std::vector<CustomPokemon>
    > pokemon_to_forms =
        get_all_pokemon_to_analyze(pokemon_name_to_serebii_pokemon);

    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<BattleAllEntry>,
        PokemonPairHash
    > pokemon_to_battles = get_battle_entries(pokemon_to_forms);

    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::pair<
            int,
            std::unordered_map<
                std::unordered_set<const MoveInfo*>,
                int,
                MoveSetHash
            >
        >,
        PokemonPairHash
    > pokemon_to_wins_and_moves_with_win_counts{};
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<BattleResultEntry>,
        PokemonPairHash
    > pokemon_to_battle_result_entries{};
    for (const auto& [
             pokemon,
             battles
         ] : pokemon_to_battles
    ) {
        auto battle_result_entries =
            thread_pool::ThreadPool::getCPUWorkInstance()->
            createAndRunTasks<
                BattleResultEntry,
                std::vector<BattleAllEntry>,
                BattleAllEntry
            >(battle_all, battles);
        pokemon_to_battle_result_entries[pokemon].append_range(
            battle_result_entries
        );
    }
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::unordered_map<
            std::pair<Pokemon, Ability>,
            std::unordered_set<const MoveInfo*>,
            PokemonPairHash
        >,
        PokemonPairHash
    > pokemon_to_losses{};
    aggregate_battle_results(
        pokemon_to_wins_and_moves_with_win_counts,
        pokemon_to_losses,
        pokemon_to_battle_result_entries
    );

    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::pair<
            int,
            std::vector<
                std::pair<std::unordered_set<const MoveInfo*>, int>
            >
        >,
        PokemonPairHash
    > pokemon_to_wins_and_sorted_moves_with_win_counts =
        get_top_2_moves(pokemon_to_wins_and_moves_with_win_counts);

    pokemon_to_forms =
        get_all_pokemon_to_analyze(pokemon_name_to_serebii_pokemon);
    for (auto& [
             pokemon,
             pokemon_forms
         ] : pokemon_to_forms
    ) {
        for (auto& pokemon_form : pokemon_forms) {
            const auto& ability = pokemon_form.ability;
            pokemon_form.moves.clear();
            for (const auto& moves :
                 pokemon_to_wins_and_sorted_moves_with_win_counts[
                     std::make_pair(pokemon, ability)
                 ].second | std::views::keys
            ) {
                for (const auto& move : moves) {
                    pokemon_form.moves.emplace_back(move);
                }
            }
        }
    }

    std::unordered_map<
        Pokemon,
        std::unordered_map<Ability, std::vector<BattleAllEntry>>
    > pokemon_to_ability_to_battles{};
    for (const auto& player_pokemon_forms :
         pokemon_to_forms | std::views::values
    ) {
        for (const auto& player_pokemon : player_pokemon_forms) {
            for (const auto& opponent_pokemon_forms :
                 get_all_pokemon_to_analyze(pokemon_name_to_serebii_pokemon) |
                 std::views::values
            ) {
                for (const auto& opponent_pokemon : opponent_pokemon_forms) {
                    pokemon_to_ability_to_battles[
                        player_pokemon.name
                    ][player_pokemon.ability].emplace_back(
                        BattleAllEntry{
                            .player = player_pokemon,
                            .opponent = opponent_pokemon,
                        }
                    );
                }
            }
        }
    }

    pokemon_to_battle_result_entries.clear();
    for (const auto& [
             pokemon,
             ability_to_battles
         ] : pokemon_to_ability_to_battles
    ) {
        for (const auto& [ability, battles] :
             ability_to_battles
        ) {
            auto battle_result_entries =
                thread_pool::ThreadPool::getCPUWorkInstance()->
                createAndRunTasks<
                    BattleResultEntry,
                    std::vector<BattleAllEntry>,
                    BattleAllEntry
                >(battle_all, battles);
            pokemon_to_battle_result_entries[
                std::make_pair(pokemon, ability)
            ].append_range(battle_result_entries);
        }
    }

    pokemon_to_wins_and_sorted_moves_with_win_counts.clear();
    aggregate_second_run(
        pokemon_to_battle_result_entries,
        pokemon_to_wins_and_sorted_moves_with_win_counts
    );

    // Get the opponents beaten for each pokemon
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<std::pair<Pokemon, Ability>>,
        PokemonPairHash
    > pokemon_to_beaten_opponents{};
    for (const auto& [
             pokemon_and_ability,
             wins_and_sorted_moves_with_win_counts
         ] : pokemon_to_wins_and_sorted_moves_with_win_counts
    ) {
        const auto& pokemon = pokemon_and_ability.first;
        if (banned.contains(pokemon)) {
            continue;
        }
        const auto& move_counts =
            wins_and_sorted_moves_with_win_counts.second;
        static std::unordered_set<Move> moves_to_skip = {
            // Move::Counter,
            // Move::MirrorCoat
        };
        bool skip = false;
        for (const auto& moves :
             move_counts | std::views::keys
        ) {
            for (const auto& move : moves) {
                if (moves_to_skip.contains(move->move)) {
                    skip = true;
                    break;
                }
            }
        }
        if (skip ||
            pokemon == Pokemon::HeatRotom ||
            pokemon == Pokemon::WashRotom ||
            pokemon == Pokemon::FrostRotom ||
            pokemon == Pokemon::FanRotom ||
            pokemon == Pokemon::MowRotom
        ) {
            continue;
        }
        const auto& battle_results =
            pokemon_to_battle_result_entries.at(pokemon_and_ability);
        for (const auto& [
                 opponent,
                 won,
                 moves,
                 _
             ] : battle_results
        ) {
            if (!won) {
                continue;
            }
            pokemon_to_beaten_opponents[pokemon_and_ability].emplace_back(
                opponent->name,
                opponent->ability
            );
        }
    }

    // Sort the pokemon based on how many battles they can win
    // and count how many times each opponent was beaten
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<std::pair<Pokemon, Ability>>>
    > pokemon_and_defeated_opponents_sorted_by_size{};
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        uint64_t,
        PokemonPairHash
    > opponents_to_total_times_beaten{};
    for (const auto& pokemon : pokemon_to_battles |
         std::views::keys
    ) {
        opponents_to_total_times_beaten[pokemon] = 0;
    }
    for (const auto& [
             pokemon,
             opponents_beaten
         ] : pokemon_to_beaten_opponents
    ) {
        pokemon_and_defeated_opponents_sorted_by_size.emplace_back(
            pokemon,
            opponents_beaten
        );
        for (const auto& opponent : opponents_beaten) {
            opponents_to_total_times_beaten[opponent]++;
        }
    }

    std::sort(
        pokemon_and_defeated_opponents_sorted_by_size.begin(),
        pokemon_and_defeated_opponents_sorted_by_size.end(),
        [](
        const std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<std::pair<Pokemon, Ability>>>& a,
        const std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<std::pair<Pokemon, Ability>>>& b
    ) {
            return (a.second).size() > (b.second).size();
        }
    );

    // Get all the opponents
    std::unordered_set<
        std::pair<Pokemon, Ability>,
        PokemonPairHash
    > all_opponents{};
    for (const auto& opponent_pokemon :
         get_all_pokemon_to_analyze(pokemon_name_to_serebii_pokemon) |
         std::views::values
    ) {
        for (const auto& pokemon : opponent_pokemon) {
            all_opponents.insert(
                std::make_pair(pokemon.name, pokemon.ability)
            );
        }
    }

    // Remove pokemon that have no wins
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::unordered_set<std::pair<Pokemon, Ability>, PokemonPairHash>,
        PokemonPairHash
    > candidate_pokemon_to_beaten_opponents;
    for (const auto& [
             pokemon,
             beaten_list
         ] : pokemon_and_defeated_opponents_sorted_by_size
    ) {
        if (beaten_list.empty()) {
            continue;
        }
        std::unordered_set<
            std::pair<Pokemon, Ability>,
            PokemonPairHash
        > beaten_set{};
        for (const auto& opponent_pokemon :
             beaten_list
        ) {
            beaten_set.insert(opponent_pokemon);
        }
        candidate_pokemon_to_beaten_opponents[pokemon] = beaten_set;
    }

    // Find the 6 pokemon with good enough coverage
    // Best takes too long
    constexpr uint8_t TEAM_SIZE = 6;
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::pair<
            std::unordered_set<const MoveInfo*>,
            std::vector<
                std::pair<
                    std::pair<Pokemon, Ability>,
                    std::unordered_set<const MoveInfo*>
                >
            >
        >,
        PokemonPairHash
    > pokemon_team_member_to_moves_and_loss_list{};
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        uint16_t,
        PokemonPairHash
    > opponents_to_times_beaten{};
    for (const auto& pokemon : all_opponents) {
        opponents_to_times_beaten[pokemon] = 0;
    }
    uint16_t max_uint16_t = std::numeric_limits<uint16_t>::max();
    while (pokemon_team_member_to_moves_and_loss_list.size() < TEAM_SIZE) {
        // Find the opponents that have been beaten the least
        std::unordered_set<
            std::pair<Pokemon, Ability>,
            PokemonPairHash
        > opponents_to_beat{};
        uint16_t min_times_beaten = max_uint16_t;
        for (const auto& [poke_pair, times_beaten] :
             opponents_to_times_beaten
        ) {
            if (times_beaten < min_times_beaten) {
                opponents_to_beat.clear();
                opponents_to_beat.emplace(poke_pair);
                min_times_beaten = times_beaten;
            } else if (times_beaten == min_times_beaten) {
                opponents_to_beat.emplace(poke_pair);
            }
        }

        // Find the pokemon that beat the most opponents
        // that have been beaten the least
        std::pair<
            std::pair<Pokemon, Ability>,
            std::unordered_set<
                std::pair<Pokemon, Ability>,
                PokemonPairHash
            >
        > pokemon_to_opponents_beaten{};
        uint16_t max_beaten = 0;
        double best_score = 0;
        for (const auto& [
                 pokemon,
                 opponents_beaten
             ] : candidate_pokemon_to_beaten_opponents
        ) {
            if (pokemon_team_member_to_moves_and_loss_list.contains(pokemon)) {
                continue;
            }
            // Find the best pokemon for the current opponents
            std::unordered_set<
                std::pair<Pokemon, Ability>,
                PokemonPairHash
            > current_opponents_beaten{};
            current_opponents_beaten.reserve(opponents_to_beat.size());
            for (const auto& opponent :
                 opponents_to_beat
            ) {
                if (opponents_beaten.contains(opponent)) {
                    current_opponents_beaten.emplace(opponent);
                }
            }
            if (std::size_t number_beaten = current_opponents_beaten.size();
                number_beaten > max_beaten
            ) {
                max_beaten = number_beaten;
                pokemon_to_opponents_beaten = std::make_pair(
                    pokemon,
                    opponents_beaten
                );
                best_score = get_score(
                    opponents_to_total_times_beaten,
                    opponents_beaten
                );
            } else if (number_beaten == max_beaten) {
                double score = get_score(
                    opponents_to_total_times_beaten,
                    opponents_beaten
                );
                if (score > best_score) {
                    pokemon_to_opponents_beaten = std::make_pair(
                        pokemon,
                        opponents_beaten
                    );
                    best_score = score;
                } else if (score == best_score) {
                    // printf(
                    //     "Tie between %s:%s and %s:%s\n",
                    //     POKEMON_TO_STRING.at(
                    //         pokemon_to_opponents_beaten.first.first
                    //     ).c_str(),
                    //     ABILITY_TO_STRING.at(
                    //         pokemon_to_opponents_beaten.first.second
                    //     ).c_str(),
                    //     POKEMON_TO_STRING.at(
                    //         pokemon.first
                    //     ).c_str(),
                    //     ABILITY_TO_STRING.at(
                    //         pokemon.second
                    //     ).c_str()
                    // );
                }
            }
        }

        // Take the opponents that the best pokemon can beat
        // and increment their beaten count
        for (const auto& beaten_opponent :
             pokemon_to_opponents_beaten.second
        ) {
            opponents_to_times_beaten[beaten_opponent]++;
        }

        std::vector<
            std::pair<std::unordered_set<const MoveInfo*>, int>
        > move_counts = pokemon_to_wins_and_sorted_moves_with_win_counts[
            pokemon_to_opponents_beaten.first
        ].second;

        std::unordered_set<const MoveInfo*> move_set{};
        for (const auto& moves :
             move_counts | std::views::keys
        ) {
            for (const auto& move : moves) {
                move_set.insert(move);
            }
        }

        pokemon_team_member_to_moves_and_loss_list[
            pokemon_to_opponents_beaten.first
        ].first = move_set;
        for (const auto& [
                 opponent,
                 opponent_moves
             ] : pokemon_to_losses[pokemon_to_opponents_beaten.first]
        ) {
            pokemon_team_member_to_moves_and_loss_list[
                pokemon_to_opponents_beaten.first
            ].second.emplace_back(
                std::pair(
                    opponent,
                    opponent_moves
                )
            );
        }
    }

    for (const auto& [
             pokemon,
             moves_and_losses
         ] : pokemon_team_member_to_moves_and_loss_list
    ) {
        const auto& [
            moves,
            losses
        ] = moves_and_losses;
        std::cout
            << POKEMON_TO_STRING.at(pokemon.first)
            << "  "
            << ABILITY_TO_STRING.at(pokemon.second)
            << '\n';
        for (const auto* move : moves) {
            std::cout
                << "    "
                << move->name
                << '\n';
        }
        std::cout << "Losses:\n";
        for (const auto& [
                 opponent,
                 opponent_moves
             ] : losses
        ) {
            std::cout
                << "    "
                << POKEMON_TO_STRING.at(opponent.first)
                << "  "
                << ABILITY_TO_STRING.at(opponent.second)
                << ": ";
            for (const auto& opponent_move : opponent_moves) {
                std::cout
                    << opponent_move->name
                    << ", ";
            }
            std::cout << '\n';
        }
    }
}


#endif //ALL_V_ALL_H
