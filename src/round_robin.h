#ifndef GEN_IV_BATTLE_HALL_ANALYZER_ROUND_ROBIN_H
#define GEN_IV_BATTLE_HALL_ANALYZER_ROUND_ROBIN_H
#include <cmath>
#include <future>
#include <unordered_map>
#include <vector>

#include "BattleState.h"
#include "config.h"
#include "custom_pokemon.h"
#include "Pokemon.h"
#include "serebii_pokemon_data_source.h"
#include "thread_pool.h"

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
    Pokemon::Wobbuffet,
    Pokemon::Smeargle
};

struct PokemonPairHash {
    std::size_t operator()(const std::pair<Pokemon, Ability>& p) const {
        return (static_cast<int>(p.first) << bits_for_ability) |
            static_cast<int>(p.second);
    }
};

inline std::unordered_map<
    std::pair<Pokemon, Ability>,
    std::vector<CustomPokemon>,
    PokemonPairHash
> get_all_pokemon_to_analyze(
    const std::unordered_map<
        std::string,
        SerebiiPokemon
    >& name_to_serebii_pokemon
) {
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<CustomPokemon>,
        PokemonPairHash
    > pokemon_to_forms{};
    const auto all_pokemon_forms =
        get_pokemon_forms(name_to_serebii_pokemon);
    for (const auto& pokemon_forms : all_pokemon_forms) {
        for (const auto& pokemon_variants :
             pokemon_forms | std::views::values
        ) {
            for (const auto& pokemon : pokemon_variants) {
                if (!banned.contains(pokemon.name)) {
                    pokemon_to_forms[
                        std::make_pair(pokemon.name, pokemon.ability)
                    ].emplace_back(pokemon);
                }
            }
        }
        // if (pokemon_to_forms.size() > 1) {
        //     break;
        // }
    }
    return pokemon_to_forms;
}

struct BattleEntry {
    CustomPokemon& player;
    CustomPokemon& opponent;
};

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

inline void get_battle_entries(
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<CustomPokemon>,
        PokemonPairHash
    >& pokemon_to_forms,
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleEntry>
        >
    >& pokemon_to_battles
) {
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
            std::vector<BattleEntry> battles_for_player{};
            for (auto& opponent_pokemon_forms :
                 pokemon_to_forms | std::views::values
            ) {
                for (auto& opponent_pokemon : opponent_pokemon_forms) {
                    battles_for_player.emplace_back(
                        BattleEntry{
                            .player = player_pokemon,
                            .opponent = opponent_pokemon,
                        }
                    );
                }
            }
            pokemon_to_battles.push_back(
                std::make_pair(
                    std::make_pair(
                        player_name,
                        player_pokemon.ability
                    ),
                    battles_for_player
                )
            );
        }
    }
}

struct MoveSetHash {
    std::size_t operator()(
        const std::unordered_set<Move>& move_set
    ) const {
        std::vector sorted_moves(move_set.begin(), move_set.end());
        std::sort(sorted_moves.begin(), sorted_moves.end());

        std::size_t hash = 0;
        int i = 0;
        for (Move move : sorted_moves) {
            const int shift = i *
                std::bit_width(static_cast<uint>(Move::Count));
            hash |= static_cast<std::size_t>(move) << shift;
            i++;
        }
        return hash;
    }
};

inline void get_battle_entries(
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<CustomPokemon>,
        PokemonPairHash
    >& player_pokemon_to_forms,
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<CustomPokemon>,
        PokemonPairHash
    >& opponent_pokemon_to_forms,
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleEntry>
        >
    >& pokemon_to_battles
) {
    pokemon_to_battles.reserve(
        player_pokemon_to_forms.size() * opponent_pokemon_to_forms.size()
    );
    for (auto& player_pokemon_forms :
         player_pokemon_to_forms | std::views::values
    ) {
        for (auto& player_pokemon : player_pokemon_forms) {
            auto player_name = player_pokemon.name;
            if (TEAM_ONLY && !team.contains(player_name)) {
                continue;
            }
            if (TEAM_ONLY) {
                change_stats(player_pokemon);
            }
            std::vector<BattleEntry> battles_for_player{};
            for (auto& opponent_pokemon_forms :
                 opponent_pokemon_to_forms | std::views::values
            ) {
                for (auto& opponent_pokemon : opponent_pokemon_forms) {
                    battles_for_player.emplace_back(
                        BattleEntry{
                            .player = player_pokemon,
                            .opponent = opponent_pokemon,
                        }
                    );
                }
            }
            pokemon_to_battles.push_back(
                std::make_pair(
                    std::make_pair(
                        player_name,
                        player_pokemon.ability
                    ),
                    battles_for_player
                )
            );
        }
    }
}

inline void battle_all(
    const BattleEntry& battle_entry,
    std::promise<BattleResultEntry>&& promise
) {
    promise.set_value(
        battle(battle_entry.player, battle_entry.opponent)
    );
}

inline void do_battles(
    const std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleEntry>
        >
    >& pokemon_to_battles,
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleResultEntry>
        >
    >& pokemon_to_battle_result_entries
) {
    // #pragma omp parallel for num_threads(NUMBER_OF_THREADS)
    //     for (size_t i = 0; i < pokemon_to_battles.size(); ++i) {
    //         const auto& pokemon_and_battles = pokemon_to_battles[i];
    //         const auto& pokemon = pokemon_and_battles.first;
    //         const auto& battles = pokemon_and_battles.second;
    //         auto& results =
    //             pokemon_to_battle_result_entries.at(pokemon);
    //         results.resize(battles.size());
    //         for (size_t j = 0; j < battles.size(); ++j) {
    //             results[j] = std::move(
    //                 battle(battles[j].player, battles[j].opponent));
    //         }
    //     }

    for (size_t i = 0; i < pokemon_to_battles.size(); ++i) {
        const auto& [
            pokemon,
            battles
        ] = pokemon_to_battles[i];
        auto battle_result_entries =
            thread_pool::ThreadPool::getCPUWorkInstance()->
            createAndRunTasks<
                BattleResultEntry,
                std::vector<BattleEntry>,
                BattleEntry
            >(battle_all, battles);
        pokemon_to_battle_result_entries.push_back(
            std::pair(pokemon, battle_result_entries)
        );
    }
}

inline void get_player_pokemon_with_top_3_moves(
    const std::unordered_map<
        std::string,
        SerebiiPokemon
    >& pokemon_name_to_serebii_pokemon,
    const std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<
                BattleResultEntry
            >
        >
    >& pokemon_to_battle_result_entries,
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<CustomPokemon>,
        PokemonPairHash
    >& player_pokemon_to_forms
) {
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::unordered_set<Move>
        >
    > pokemon_and_top_moves{};
    for (const auto& [
             pokemon,
             battle_results
         ] : pokemon_to_battle_result_entries
    ) {
        std::unordered_map<
            std::unordered_set<Move>,
            uint,
            MoveSetHash
        > moves_to_times_used_to_win{};
        for (const auto& [
                 _,
                 _,
                 won,
                 player_moves,
                 opponent_moves
             ] : battle_results
        ) {
            if (won) {
                std::unordered_set<Move> move_set{};
                for (const auto& move :
                     player_moves | std::views::keys
                ) {
                    move_set.emplace(move);
                }
                moves_to_times_used_to_win[std::move(move_set)]++;
            }
        }
        std::vector<
            std::pair<std::unordered_set<Move>, uint>
        > sorted_move_set_counts{};
        sorted_move_set_counts.append_range(moves_to_times_used_to_win);
        std::sort(
            sorted_move_set_counts.begin(),
            sorted_move_set_counts.end(),
            [](const auto& move_set1, const auto& move_set2) {
                return move_set1.second > move_set2.second;
            }
        );
        std::unordered_set<Move> top_3_moves{};
        size_t i = 0;
        while (top_3_moves.size() < 3 &&
            i < sorted_move_set_counts.size()
        ) {
            for (const auto& move :
                 sorted_move_set_counts[i].first
            ) {
                top_3_moves.insert(move);
            }
            i++;
        }
        pokemon_and_top_moves.push_back(
            std::pair(pokemon, std::move(top_3_moves))
        );
    }

    player_pokemon_to_forms =
        get_all_pokemon_to_analyze(pokemon_name_to_serebii_pokemon);
    for (const auto& [
             pokemon,
             best_moves
         ] : pokemon_and_top_moves
    ) {
        if (best_moves.size() == 0) {
            player_pokemon_to_forms.erase(pokemon);
            continue;
        }
        for (auto& player_pokemon : player_pokemon_to_forms[pokemon]) {
            const auto all_moves = player_pokemon.moves;
            player_pokemon.moves.clear();
            for (const auto& move : best_moves) {
                auto it = std::ranges::find_if(
                    all_moves,
                    [move](const auto& m) {
                        return m->move == move;
                    }
                );
                if (it != all_moves.end()) {
                    player_pokemon.moves.push_back(*it);
                }
            }
        }
    }
}

inline void do_round_robin_with_best_moves(
    const std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleEntry>
        >
    >& pokemon_to_battles,
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleResultEntry>
        >
    >& pokemon_to_battle_result_entries
) {
    do_battles(
        pokemon_to_battles,
        pokemon_to_battle_result_entries
    );
}

inline void round_robin(
    const std::unordered_map<
        std::string,
        SerebiiPokemon
    >& pokemon_name_to_serebii_pokemon
) {
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<CustomPokemon>,
        PokemonPairHash
    > pokemon_to_forms =
        get_all_pokemon_to_analyze(pokemon_name_to_serebii_pokemon);
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleEntry>
        >
    > pokemon_to_battles{};
    get_battle_entries(pokemon_to_forms, pokemon_to_battles);

    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleResultEntry>
        >
    > pokemon_to_battle_result_entries{};

    do_battles(
        pokemon_to_battles,
        pokemon_to_battle_result_entries
    );

    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<CustomPokemon>,
        PokemonPairHash
    > player_pokemon_to_forms{};
    get_player_pokemon_with_top_3_moves(
        pokemon_name_to_serebii_pokemon,
        pokemon_to_battle_result_entries,
        player_pokemon_to_forms
    );

    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleEntry>
        >
    > best_move_pokemon_to_battles{};
    get_battle_entries(
        player_pokemon_to_forms,
        pokemon_to_forms,
        best_move_pokemon_to_battles
    );
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleResultEntry>
        >
    > best_moves_pokemon_to_battle_result_entries{};
    do_round_robin_with_best_moves(
        best_move_pokemon_to_battles,
        best_moves_pokemon_to_battle_result_entries
    );

    for (auto& battle_results :
         best_moves_pokemon_to_battle_result_entries | std::views::values
    ) {
        for (auto& battle_result : battle_results) {
            std::sort(
                battle_result.opponent_moves.begin(),
                battle_result.opponent_moves.end(),
                [](const auto& a, const auto& b) {
                    return a.second > b.second;
                }
            );
        }
    }
    for (auto& battle_results :
         best_moves_pokemon_to_battle_result_entries | std::views::values
    ) {
        const auto cmp = [](const auto& a, const auto& b) {
            const auto aps = a.player_moves.size();
            const auto aos = a.opponent_moves.size();
            const int a_diff = aos - aps;
            const auto bps = b.player_moves.size();
            const auto bos = b.opponent_moves.size();
            const int b_diff = bos - bps;
            if (a_diff > 0) {
                // opponent attacked more
                if (b_diff <= 0) {
                    // opponent attacked less
                    return false;
                }
                // b_diff > 0; opponent attacked more
                if (aps != bps) {
                    return aps < bps;
                }
                if (aos > 0 && bos > 0) {
                    return a.opponent_moves[0].second <
                        b.opponent_moves[0].second;
                }
                if (aps > 0 && bps > 0) {
                    return a.player_moves[0].second >
                        b.player_moves[0].second;
                }
            }
            if (a_diff == 0) {
                if (b_diff <= 0) {
                    // opponent attacked less
                    return false;
                }
                // b_diff can't be 0 when a_diff is
                // b_diff > 0; opponent attacked more
                return true;
            }
            // a_diff < 0; opponent attacked less
            if (b_diff >= 0) {
                // opponent attacked more
                return true;
            }
            // b_diff < 0
            if (aps != bps) {
                return aps < bps;
            }
            if (aos > 0 && bos > 0) {
                return a.opponent_moves[0].second <
                    b.opponent_moves[0].second;
            }
            if (aps > 0 && bps > 0) {
                return a.player_moves[0].second >
                    b.player_moves[0].second;
            }
            return false;
        };

        std::sort(
            battle_results.begin(),
            battle_results.end(),
            cmp
        );
    }

    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<
            std::pair<
                std::pair<
                    std::vector<std::pair<Stat, int>>,
                    bool>,
                std::unordered_set<const CustomPokemon*>
            >
        >,
        PokemonPairHash
    > stat_and_ability_to_evs_and_losses_beaten{};
    for (const auto& [
             pokemon,
             battle_results
         ] : best_moves_pokemon_to_battle_result_entries
    ) {
        std::vector<CustomPokemon*> losses_to_beat{};
        for (const auto& battle_result : battle_results) {
            if (battle_result.won) {
                continue;
            }
            losses_to_beat.emplace_back(battle_result.opponent);
        }

        for (const auto& [
                 player,
                 opponent,
                 won,
                 player_moves,
                 opponent_moves
             ] : battle_results) {
            if (won) {
                continue;
            }
            // TODO where the EV to losses defeated map will be created
            // (0.6 * nature) increase per 4 EVs
            // (stat * 1.1) for ability
            BattleState battle_state{*player, *opponent};

            if (player_moves.size() == opponent_moves.size()) {
                // be faster
                // speed
                const auto p_speed =
                    battle_state.get_player_state().get_speed(Weather::CLEAR);
                const auto o_speed =
                    battle_state.get_opponent_state().get_speed(Weather::CLEAR);
                if (p_speed <= o_speed) {
                    const auto speed_diff = o_speed - p_speed + 1;
                    const int evs = std::ceil(speed_diff / 0.6) * 4;
                    const int nature_evs = std::ceil(evs / 1.1 / 4) * 4;
                    auto p = *player;
                    auto new_speed = p.stats[static_cast<int>(Stat::SPEED)] +
                        speed_diff;
                    p.stats[static_cast<int>(Stat::SPEED)] = new_speed;
                    if (battle(p, *opponent).won) {
                        std::unordered_set<const CustomPokemon*> losses_beaten
                            {};
                        std::vector<BattleEntry> battles{};

                        for (auto o : losses_to_beat) {
                            battles.emplace_back(
                                BattleEntry{p, *o}
                            );
                        }
                        auto battle_result_entries =
                            thread_pool::ThreadPool::getCPUWorkInstance()->
                            createAndRunTasks<
                                BattleResultEntry,
                                std::vector<BattleEntry>,
                                BattleEntry
                            >(battle_all, battles);
                        for (const auto& results :
                             battle_result_entries
                        ) {
                            if (results.won) {
                                losses_beaten.insert(results.opponent);
                            }
                        }
                        if (losses_beaten.size() > 0) {
                            stat_and_ability_to_evs_and_losses_beaten
                                [pokemon].emplace_back(
                                    std::pair{
                                        std::pair{
                                            std::vector{
                                                std::pair{Stat::SPEED, evs}
                                            },
                                            false
                                        },
                                        losses_beaten
                                    }
                                );
                            if (nature_evs != evs) {
                                stat_and_ability_to_evs_and_losses_beaten
                                    [pokemon].emplace_back(
                                        std::pair{
                                            std::pair{
                                                std::vector{
                                                    std::pair{Stat::SPEED, evs}
                                                },
                                                true
                                            },
                                            losses_beaten
                                        }
                                    );
                            }
                        }
                    }
                    // be faster and survive first hit
                    // hp
                    // def
                    printf("");
                }

                // survive the first hit
                // hp
                // def

                // survive the first hit and do enough damage
                // atk
            } else {
                if (player_moves.size() > opponent_moves.size()) {}
            }
        }
    }

    printf("");
}

#endif //GEN_IV_BATTLE_HALL_ANALYZER_ROUND_ROBIN_H
