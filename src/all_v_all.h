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

const std::unordered_set team = {
    Pokemon::Kingdra,
    Pokemon::Gallade,
    Pokemon::PorygonZ,
    Pokemon::Arcanine,
    Pokemon::Ambipom,
    Pokemon::Starmie,
};

inline void change_stats(CustomPokemon& pokemon) {
    if (const auto& name = pokemon.name;
        name == Pokemon::Kingdra
    ) {
        pokemon.stats[static_cast<int>(Stat::ATTACK)] = 171;
       pokemon.stats[static_cast<int>(Stat::DEFENSE)] = 125;
       // pokemon.stats[static_cast<int>(Stat::SPECIAL_ATTACK)] = 107;
        pokemon.stats[static_cast<int>(Stat::SPECIAL_DEFENSE)] = 107;
       pokemon.stats[static_cast<int>(Stat::SPEED)] = 114;
    } else if (name == Pokemon::Kangaskhan) {
        //
    } else if (name == Pokemon::Gallade) {
        //
    } else if (name == Pokemon::Infernape) {
        //
    } else if (name == Pokemon::PorygonZ) {
        //
    } else if (name == Pokemon::Staraptor) {
        //
    }
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
    for (auto& player_pokemon_forms :
         pokemon_to_forms | std::views::values
    ) {
        for (auto& player_pokemon : player_pokemon_forms) {
            auto player_name = player_pokemon.name;
            if (TEAM_ONLY && !team.contains(player_name)) {
                continue;
            }
            if (TEAM_ONLY) {
                change_stats(player_pokemon);
            }
            for (const auto& opponent_pokemon_forms :
                 pokemon_to_forms | std::views::values
            ) {
                for (const auto& opponent_pokemon : opponent_pokemon_forms) {
                    pokemon_to_battles[
                            std::make_pair(
                                player_name,
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
        std::unordered_map<
            std::unordered_set<const MoveInfo*>,
            std::unordered_set<
                std::pair<Pokemon, Ability>,
                PokemonPairHash
            >,
            MoveSetHash
        >,
        PokemonPairHash
    >& pokemon_to_moves_with_beaten_opponents,
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::unordered_map<
            std::pair<Pokemon, Ability>,
            std::unordered_set<
                std::pair<const MoveInfo*, int>,
                MoveDamagePairHash
            >,
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
            std::unordered_set<const MoveInfo*> move_set{};
            for (const auto& move : moves | std::views::keys) {
                move_set.insert(move);
            }
            if (won) {
                pokemon_to_moves_with_beaten_opponents[
                    player_pokemon
                ][move_set].insert(
                    std::make_pair(
                        opponent_pokemon->name,
                        opponent_pokemon->ability
                    )
                );
            } else {
                pokemon_to_losses[
                    player_pokemon
                ][
                    std::make_pair(
                        opponent_pokemon->name,
                        opponent_pokemon->ability
                    )
                ] = opponent_moves;
            }
        }
    }
}

inline std::unordered_map<
    std::pair<Pokemon, Ability>,
    std::unordered_set<const MoveInfo*>,
    PokemonPairHash
> get_top_2_moves(
    const std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::unordered_map<
            std::unordered_set<const MoveInfo*>,
            std::unordered_set<
                std::pair<Pokemon, Ability>,
                PokemonPairHash
            >,
            MoveSetHash
        >,
        PokemonPairHash
    >& pokemon_to_moves_with_beaten_opponents,
    std::unordered_set<
        std::pair<Pokemon, Ability>,
        PokemonPairHash
    >& all_opponents
) {
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::unordered_set<const MoveInfo*>,
        PokemonPairHash
    > pokemon_to_top_move_sets{};

    std::unordered_map<
        std::pair<Pokemon, Ability>,
        uint64_t,
        PokemonPairHash
    > opponents_to_total_times_beaten{};
    for (const auto& pokemon : all_opponents
    ) {
        opponents_to_total_times_beaten[pokemon] = 0;
    }
    for (const auto& moves_with_beaten_opponents :
         pokemon_to_moves_with_beaten_opponents | std::views::values
    ) {
        for (const auto& moves :
             moves_with_beaten_opponents | std::views::values
        ) {
            for (const auto& move : moves)
                opponents_to_total_times_beaten[move]++;
        }
    }

    for (auto& [
             pokemon,
             moves_with_beaten_opponents
         ] : pokemon_to_moves_with_beaten_opponents
    ) {
        std::unordered_set<const MoveInfo*> all_moves{};
        for (const auto& moves :
             moves_with_beaten_opponents | std::views::keys
        ) {
            all_moves.insert_range(moves);
        }
        size_t number_of_moves = std::min(
            static_cast<size_t>(3),
            all_moves.size()
        );

        std::unordered_map<
            std::pair<Pokemon, Ability>,
            uint16_t,
            PokemonPairHash
        > opponents_to_times_beaten{};
        for (const auto& opponent : all_opponents) {
            opponents_to_times_beaten[opponent] = 0;
        }
        uint16_t max_uint16_t = std::numeric_limits<uint16_t>::max();
        std::unordered_set<const MoveInfo*> picked_moves{};
        while (picked_moves.size() < number_of_moves) {
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

            // Find the move set that beat the most opponents
            // that have been beaten the least
            std::pair<
                std::unordered_set<const MoveInfo*>,
                std::unordered_set<
                    std::pair<Pokemon, Ability>,
                    PokemonPairHash
                >
            > move_set_to_opponents_beaten{};
            uint16_t max_beaten = 0;
            double best_score = 0;
            for (auto& [
                     move_set,
                     opponents_beaten
                 ] : moves_with_beaten_opponents
            ) {
                // Find the best move set for the current opponents
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
                    move_set_to_opponents_beaten = std::make_pair(
                        move_set,
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
                        move_set_to_opponents_beaten = std::make_pair(
                            move_set,
                            opponents_beaten
                        );
                        best_score = score;
                    }
                }
            }

            // Take the opponents that the best move sets can beat
            // and increment their beaten count
            for (const auto& beaten_opponent :
                 move_set_to_opponents_beaten.second
            ) {
                opponents_to_times_beaten[beaten_opponent]++;
            }

            for (const auto& move :
                 move_set_to_opponents_beaten.first
            ) {
                picked_moves.insert(move);
            }

            pokemon_to_top_move_sets[pokemon].insert_range(
                move_set_to_opponents_beaten.first
            );
        }
        if (pokemon.first == Pokemon::Meowth) {
            volatile int a;
        }
    }

    return pokemon_to_top_move_sets;
}

inline void aggregate_second_run(
    const std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<BattleResultEntry>,
        PokemonPairHash
    >& pokemon_to_battle_result_entries,
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::unordered_map<
            std::pair<Pokemon, Ability>,
            std::unordered_set<
                std::pair<const MoveInfo*, int>,
                MoveDamagePairHash
            >,
            PokemonPairHash
        >,
        PokemonPairHash
    >& pokemon_to_losses
) {
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::unordered_map<
            std::unordered_set<const MoveInfo*>,
            std::unordered_set<
                std::pair<Pokemon, Ability>,
                PokemonPairHash
            >,
            MoveSetHash
        >,
        PokemonPairHash
    > pokemon_to_moves_to_beaten_opponents{};
    aggregate_battle_results(
        pokemon_to_moves_to_beaten_opponents,
        pokemon_to_losses,
        pokemon_to_battle_result_entries
    );
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
        std::unordered_map<
            std::unordered_set<const MoveInfo*>,
            std::unordered_set<
                std::pair<Pokemon, Ability>,
                PokemonPairHash
            >,
            MoveSetHash
        >,
        PokemonPairHash
    > pokemon_to_moves_with_beaten_opponents{};
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
            std::unordered_set<
                std::pair<const MoveInfo*, int>,
                MoveDamagePairHash
            >,
            PokemonPairHash
        >,
        PokemonPairHash
    > pokemon_to_losses{};
    aggregate_battle_results(
        pokemon_to_moves_with_beaten_opponents,
        pokemon_to_losses,
        pokemon_to_battle_result_entries
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

    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::unordered_set<const MoveInfo*>,
        PokemonPairHash
    > pokemon_to_move_sets =
        get_top_2_moves(
            pokemon_to_moves_with_beaten_opponents,
            all_opponents
        );

    pokemon_to_forms =
        get_all_pokemon_to_analyze(pokemon_name_to_serebii_pokemon);
    for (auto& [
             pokemon,
             pokemon_forms
         ] : pokemon_to_forms
    ) {
        for (auto& pokemon_form : pokemon_forms) {
            if (TEAM_ONLY && !team.contains(pokemon_form.name)) {
                continue;
            }
            if (TEAM_ONLY) {
                change_stats(pokemon_form);
            }
            const auto& ability = pokemon_form.ability;
            pokemon_form.moves.clear();
            for (const auto& move :
                 pokemon_to_move_sets[
                     std::make_pair(pokemon, ability)
                 ]
            ) {
                pokemon_form.moves.emplace_back(move);
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
            if (TEAM_ONLY && !team.contains(player_pokemon.name)) {
                continue;
            }
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

    pokemon_to_losses.clear();
    aggregate_second_run(
        pokemon_to_battle_result_entries,
        pokemon_to_losses
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
         ] : pokemon_to_move_sets
    ) {
        const auto& pokemon = pokemon_and_ability.first;
        if (banned.contains(pokemon)) {
            continue;
        }
        const auto& move_counts =
            wins_and_sorted_moves_with_win_counts;
        static std::unordered_set<Move> moves_to_skip = {
            // Move::Counter,
            // Move::MirrorCoat
        };
        bool skip = false;
        for (const auto& move : move_counts) {
            if (moves_to_skip.contains(move->move)) {
                skip = true;
                break;
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
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::pair<
                std::unordered_set<const MoveInfo*>,
                std::vector<
                    std::pair<
                        std::pair<Pokemon, Ability>,
                        std::unordered_set<
                            std::pair<const MoveInfo*, int>,
                            MoveDamagePairHash
                        >
                    >
                >
            >
        >
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
            // bool skip = false;
            // for (const auto& poke_pair :
            //      pokemon_team_member_to_moves_and_loss_list | std::views::keys
            // ) {
            //     if (poke_pair.first == pokemon.first) {
            //         skip = true;
            //     }
            // }
            // if (skip) {
            //     continue;
            // }

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

        std::unordered_set<const MoveInfo*> move_counts =
            pokemon_to_move_sets[
                pokemon_to_opponents_beaten.first
            ];

        std::unordered_set<const MoveInfo*> move_set{};
        for (const auto& move : move_counts) {
            move_set.insert(move);
        }

        pokemon_team_member_to_moves_and_loss_list.emplace_back(
            pokemon_to_opponents_beaten.first,
            std::make_pair(
                move_set,
                std::vector<
                    std::pair<
                        std::pair<Pokemon, Ability>,
                        std::unordered_set<
                            std::pair<const MoveInfo*, int>,
                            MoveDamagePairHash
                        >
                    >
                >{}
            )
        );
        for (const auto& [
                 opponent,
                 opponent_moves
             ] : pokemon_to_losses[pokemon_to_opponents_beaten.first]
        ) {
            pokemon_team_member_to_moves_and_loss_list.back().second.second.
                emplace_back(
                    std::pair(
                        opponent,
                        opponent_moves
                    )
                );
        }
        std::sort(
            pokemon_team_member_to_moves_and_loss_list.back().second.second.
            begin(),
            pokemon_team_member_to_moves_and_loss_list.back().second.second.
            end(),
            [](const auto& a, const auto& b) {
                auto maxA = std::max_element(
                    a.second.begin(), a.second.end(),
                    [](const auto& x, const auto& y) {
                        return x.second < y.second;
                    }
                );
                auto maxB = std::max_element(
                    b.second.begin(), b.second.end(),
                    [](const auto& x, const auto& y) {
                        return x.second < y.second;
                    }
                );
                const int valA =
                    (maxA == a.second.end()) ? 0 : maxA->second;
                const int valB =
                    (maxB == b.second.end()) ? 0 : maxB->second;
                return valA > valB; // descending
            }
        );
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
            for (const auto& [
                     move,
                     damage
                 ] : opponent_moves
            ) {
                std::cout
                    << move->name
                    << "("
                    << damage
                    << "),";
            }
            std::cout << '\n';
        }
    }
}


#endif //ALL_V_ALL_H
