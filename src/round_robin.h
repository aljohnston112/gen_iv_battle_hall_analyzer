#ifndef GEN_IV_BATTLE_HALL_ANALYZER_ROUND_ROBIN_H
#define GEN_IV_BATTLE_HALL_ANALYZER_ROUND_ROBIN_H
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
    Pokemon::Wobbuffet
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
    }
    return pokemon_to_forms;
}

struct BattleEntry {
    CustomPokemon player;
    CustomPokemon opponent;
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

inline std::vector<
    std::pair<
        std::pair<Pokemon, Ability>,
        std::vector<BattleEntry>
    >
> get_battle_entries(
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<CustomPokemon>,
        PokemonPairHash
    > pokemon_to_forms
) {
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleEntry>
        >
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
            std::vector<BattleEntry> battles_for_player{};
            for (const auto& opponent_pokemon_forms :
                 pokemon_to_forms | std::views::values
            ) {
                for (const auto& opponent_pokemon : opponent_pokemon_forms) {
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
    return pokemon_to_battles;
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

inline std::vector<
    std::pair<
        std::pair<Pokemon, Ability>,
        std::vector<BattleEntry>
    >
> get_battle_entries(
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<CustomPokemon>,
        PokemonPairHash
    > player_pokemon_to_forms,
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<CustomPokemon>,
        PokemonPairHash
    > pokemon_to_forms
) {
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleEntry>
        >
    > pokemon_to_battles{};
    pokemon_to_battles.reserve(
        pokemon_to_forms.size() * pokemon_to_forms.size()
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
            for (const auto& opponent_pokemon_forms :
                 pokemon_to_forms | std::views::values
            ) {
                for (const auto& opponent_pokemon : opponent_pokemon_forms) {
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
    return pokemon_to_battles;
}

inline void battle_all(
    const BattleEntry& battle_entry,
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

inline void do_battles(
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleEntry>
        >
    >& pokemon_to_battles,
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<BattleResultEntry>,
        PokemonPairHash
    >& pokemon_to_battle_result_entries
) {
    pokemon_to_battle_result_entries = {};
    std::vector<std::pair<Pokemon, Ability>> all_pokemon{};
    for (const auto& [
             pokemon,
             battles
         ] : pokemon_to_battles
    ) {
        pokemon_to_battle_result_entries.try_emplace(pokemon, battles.size());
        all_pokemon.push_back(pokemon);
    }

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
        const auto& pokemon_and_battles = pokemon_to_battles[i];
        const auto& pokemon = pokemon_and_battles.first;
        const auto& battles = pokemon_and_battles.second;
        auto& results =
            pokemon_to_battle_result_entries.at(pokemon);
        auto battle_result_entries =
                thread_pool::ThreadPool::getCPUWorkInstance()->
                createAndRunTasks<
                    BattleResultEntry,
                    std::vector<BattleEntry>,
                    BattleEntry
                >(battle_all, battles);
        pokemon_to_battle_result_entries.at(pokemon) = std::move(results);
    }
}

inline void do_round_robin_with_best_moves(
    const std::unordered_map<
        std::string,
        SerebiiPokemon
    >& pokemon_name_to_serebii_pokemon,
    const std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<CustomPokemon>,
        PokemonPairHash
    >& pokemon_to_forms,
    std::unordered_map<
        std::pair<Pokemon, Ability>, std::vector<BattleResultEntry>,
        PokemonPairHash
    >& pokemon_to_battle_result_entries
) {
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::unordered_set<const MoveInfo*>
        >
    > pokemon_and_top_moves{};
    for (const auto& [
             pokemon,
             battle_results
         ] : pokemon_to_battle_result_entries
    ) {
        std::unordered_map<
            std::unordered_set<const MoveInfo*>,
            uint,
            MoveSetHash
        > moves_to_times_used_to_win{};
        for (const auto& [
                 _,
                 won,
                 player_moves,
                 opponent_moves
             ] : battle_results
        ) {
            if (won) {
                std::unordered_set<const MoveInfo*> move_set{};
                for (const auto& move :
                     player_moves | std::views::keys
                ) {
                    move_set.emplace(move);
                }
                moves_to_times_used_to_win[move_set]++;
            }
        }
        std::vector<
            std::pair<std::unordered_set<const MoveInfo*>, uint>
        > sorted_move_set_counts{};
        sorted_move_set_counts.append_range(moves_to_times_used_to_win);
        std::sort(
            sorted_move_set_counts.begin(),
            sorted_move_set_counts.end(),
            [](const auto& move_set1, const auto& move_set2) {
                return move_set1.second > move_set2.second;
            }
        );
        std::unordered_set<const MoveInfo*> top_3_moves{};
        size_t i = 0;
        while (top_3_moves.size() < 3 &&
            i < sorted_move_set_counts.size()
        ) {
            for (const auto& move :
                 sorted_move_set_counts[i].first
            ) {
                top_3_moves.emplace(move);
            }
            i++;
        }
        pokemon_and_top_moves.push_back(
            std::pair(pokemon, top_3_moves)
        );
    }

    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<CustomPokemon>,
        PokemonPairHash
    > player_pokemon_to_forms =
        get_all_pokemon_to_analyze(pokemon_name_to_serebii_pokemon);
    for (const auto& [
             pokemon,
             moves
         ] : pokemon_and_top_moves
    ) {
        for (auto player_pokemon : player_pokemon_to_forms[pokemon]) {
            const auto all_moves = player_pokemon.moves;
            player_pokemon.moves.clear();
            for (const auto& move : moves) {
                if (std::ranges::find(all_moves, move) != all_moves.end()) {
                    player_pokemon.moves.push_back(move);
                }
            }
        }
    }

    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleEntry>
        >
    > pokemon_to_battles = get_battle_entries(
        player_pokemon_to_forms,
        pokemon_to_forms
    );

    do_battles(
        pokemon_to_battles,
        pokemon_to_battle_result_entries
    );
}

inline void do_round_robin(
    const std::unordered_map<
        std::string,
        SerebiiPokemon
    >& pokemon_name_to_serebii_pokemon,
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<BattleResultEntry>,
        PokemonPairHash
    >& pokemon_to_battle_result_entries
) {
    const std::unordered_map<
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
    > pokemon_to_battles = get_battle_entries(pokemon_to_forms);
    do_battles(
        pokemon_to_battles,
        pokemon_to_battle_result_entries
    );

    do_round_robin_with_best_moves(
        pokemon_name_to_serebii_pokemon,
        pokemon_to_forms,
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
        std::vector<BattleResultEntry>,
        PokemonPairHash
    > pokemon_to_battle_result_entries;
    do_round_robin(
        pokemon_name_to_serebii_pokemon,
        pokemon_to_battle_result_entries
    );

    printf("");
}

#endif //GEN_IV_BATTLE_HALL_ANALYZER_ROUND_ROBIN_H
