#ifndef GEN_IV_BATTLE_HALL_ANALYZER_ROUND_ROBIN_H
#define GEN_IV_BATTLE_HALL_ANALYZER_ROUND_ROBIN_H
#include <cmath>
#include <future>
#include <unordered_map>
#include <vector>

#include "BattleState.h"
#include "config.h"
#include "custom_pokemon.h"
#include "EVOptimizer.h"
#include "Pokemon.h"
#include "serebii_pokemon_data_source.h"
#include "thread_pool.h"

struct EVSet;
inline const std::unordered_set skipped = {
    Pokemon::Bulbasaur,
    Pokemon::Ivysaur,
    Pokemon::Charmander,
    Pokemon::Charmeleon,
    Pokemon::Squirtle,
    Pokemon::Wartortle,
    Pokemon::Caterpie,
    Pokemon::Metapod,
    Pokemon::Weedle,
    Pokemon::Kakuna,
    Pokemon::Pidgey,
    Pokemon::Pidgeotto,
    Pokemon::Rattata,
    Pokemon::Spearow,
    Pokemon::Ekans,
    Pokemon::Pikachu,
    Pokemon::Sandshrew,
    Pokemon::NidoranF,
    Pokemon::Nidorina,
    Pokemon::NidoranM,
    Pokemon::Nidorino,
    Pokemon::Clefairy,
    Pokemon::Vulpix,
    Pokemon::Jigglypuff,
    Pokemon::Zubat,
    Pokemon::Golbat,
    Pokemon::Oddish,
    Pokemon::Gloom,
    Pokemon::Paras,
    Pokemon::Venonat,
    Pokemon::Diglett,
    Pokemon::Meowth,
    Pokemon::Psyduck,
    Pokemon::Mankey,
    Pokemon::Growlithe,
    Pokemon::Poliwag,
    Pokemon::Poliwhirl,
    Pokemon::Abra,
    Pokemon::Kadabra,
    Pokemon::Machop,
    Pokemon::Machoke,
    Pokemon::Bellsprout,
    Pokemon::Weepinbell,
    Pokemon::Tentacool,
    Pokemon::Geodude,
    Pokemon::Graveler,
    Pokemon::Ponyta,
    Pokemon::Slowpoke,
    Pokemon::Magnemite,
    Pokemon::Magneton,
    Pokemon::Doduo,
    Pokemon::Seel,
    Pokemon::Grimer,
    Pokemon::Shellder,
    Pokemon::Gastly,
    Pokemon::Haunter,
    Pokemon::Onix,
    Pokemon::Drowzee,
    Pokemon::Krabby,
    Pokemon::Voltorb,
    Pokemon::Exeggcute,
    Pokemon::Cubone,
    Pokemon::Lickitung,
    Pokemon::Koffing,
    Pokemon::Rhyhorn,
    Pokemon::Rhydon,
    Pokemon::Chansey,
    Pokemon::Tangela,
    Pokemon::Horsea,
    Pokemon::Seadra,
    Pokemon::Goldeen,
    Pokemon::Staryu,
    Pokemon::Scyther,
    Pokemon::Electabuzz,
    Pokemon::Magmar,
    Pokemon::Magikarp,
    Pokemon::Eevee,
    Pokemon::Porygon,
    Pokemon::Omanyte,
    Pokemon::Kabuto,
    Pokemon::Dratini,
    Pokemon::Chikorita,
    Pokemon::Bayleef,
    Pokemon::Cyndaquil,
    Pokemon::Quilava,
    Pokemon::Totodile,
    Pokemon::Croconaw,
    Pokemon::Sentret,
    Pokemon::Hoothoot,
    Pokemon::Ledyba,
    Pokemon::Spinarak,
    Pokemon::Chinchou,
    Pokemon::Pichu,
    Pokemon::Cleffa,
    Pokemon::Igglybuff,
    Pokemon::Togepi,
    Pokemon::Togetic,
    Pokemon::Natu,
    Pokemon::Mareep,
    Pokemon::Flaaffy,
    Pokemon::Marill,
    Pokemon::Hoppip,
    Pokemon::Skiploom,
    Pokemon::Aipom,
    Pokemon::Sunkern,
    Pokemon::Yanma,
    Pokemon::Wooper,
    Pokemon::Murkrow,
    Pokemon::Misdreavus,
    Pokemon::Pineco,
    Pokemon::Gligar,
    Pokemon::Snubbull,
    Pokemon::Sneasel,
    Pokemon::Teddiursa,
    Pokemon::Slugma,
    Pokemon::Swinub,
    Pokemon::Remoraid,
    Pokemon::Houndour,
    Pokemon::Phanpy,
    Pokemon::Porygon2,
    Pokemon::Tyrogue,
    Pokemon::Smoochum,
    Pokemon::Elekid,
    Pokemon::Magby,
    Pokemon::Larvitar,
    Pokemon::Treecko,
    Pokemon::Grovyle,
    Pokemon::Torchic,
    Pokemon::Combusken,
    Pokemon::Mudkip,
    Pokemon::Marshtomp,
    Pokemon::Poochyena,
    Pokemon::Zigzagoon,
    Pokemon::Wurmple,
    Pokemon::Silcoon,
    Pokemon::Cascoon,
    Pokemon::Lotad,
    Pokemon::Lombre,
    Pokemon::Seedot,
    Pokemon::Nuzleaf,
    Pokemon::Taillow,
    Pokemon::Wingull,
    Pokemon::Ralts,
    Pokemon::Kirlia,
    Pokemon::Surskit,
    Pokemon::Shroomish,
    Pokemon::Slakoth,
    Pokemon::Nincada,
    Pokemon::Whismur,
    Pokemon::Loudred,
    Pokemon::Makuhita,
    Pokemon::Azumarill,
    Pokemon::Nosepass,
    Pokemon::Skitty,
    Pokemon::Aron,
    Pokemon::Lairon,
    Pokemon::Meditite,
    Pokemon::Electrike,
    Pokemon::Roselia,
    Pokemon::Gulpin,
    Pokemon::Carvanha,
    Pokemon::Wailmer,
    Pokemon::Numel,
    Pokemon::Spoink,
    Pokemon::Trapinch,
    Pokemon::Vibrava,
    Pokemon::Cacnea,
    Pokemon::Swablu,
    Pokemon::Barboach,
    Pokemon::Corphish,
    Pokemon::Baltoy,
    Pokemon::Lileep,
    Pokemon::Anorith,
    Pokemon::Feebas,
    Pokemon::Shuppet,
    Pokemon::Duskull,
    Pokemon::Dusclops,
    Pokemon::Wynaut,
    Pokemon::Snorunt,
    Pokemon::Spheal,
    Pokemon::Sealeo,
    Pokemon::Clamperl,
    Pokemon::Bagon,
    Pokemon::Beldum,
    Pokemon::Turtwig,
    Pokemon::Grotle,
    Pokemon::Chimchar,
    Pokemon::Monferno,
    Pokemon::Piplup,
    Pokemon::Prinplup,
    Pokemon::Starly,
    Pokemon::Staravia,
    Pokemon::Bidoof,
    Pokemon::Kricketot,
    Pokemon::Shinx,
    Pokemon::Luxio,
    Pokemon::Budew,
    Pokemon::Cranidos,
    Pokemon::Shieldon,
    Pokemon::Burmy,
    Pokemon::Combee,
    Pokemon::Buizel,
    Pokemon::Cherubi,
    Pokemon::Shellos,
    Pokemon::Drifloon,
    Pokemon::Buneary,
    Pokemon::Glameow,
    Pokemon::Chingling,
    Pokemon::Stunky,
    Pokemon::Bronzor,
    Pokemon::Bonsly,
    Pokemon::MimeJr,
    Pokemon::Happiny,
    Pokemon::Gible,
    Pokemon::Munchlax,
    Pokemon::Riolu,
    Pokemon::Hippopotas,
    Pokemon::Skorupi,
    Pokemon::Croagunk,
    Pokemon::Finneon,
    Pokemon::Mantyke,
    Pokemon::Snover
};

inline const std::unordered_set banned = {
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
    uint64_t forms = 0;
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
                if (!banned.contains(pokemon.name) &&
                    !skipped.contains(pokemon.name)
                ) {
                    pokemon_to_forms[
                        std::make_pair(pokemon.name, pokemon.ability)
                    ].emplace_back(pokemon);
                    forms++;
                }
            }
        }
        // if (pokemon_to_forms.size() > 70) {
        //     break;
        // }
    }
    std::cout << "Forms: " << forms << std::endl;

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

inline void change_stats(CustomPokemon* pokemon) {
    if (const auto& name = pokemon->name;
        name == Pokemon::Kingdra
    ) {
        pokemon->stats[static_cast<int>(Stat::ATTACK)] = 171;
        pokemon->stats[static_cast<int>(Stat::DEFENSE)] = 125;
        // pokemon.stats[static_cast<int>(Stat::SPECIAL_ATTACK)] = 107;
        pokemon->stats[static_cast<int>(Stat::SPECIAL_DEFENSE)] = 107;
        pokemon->stats[static_cast<int>(Stat::SPEED)] = 114;
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

struct MoveInfoSetHash {
    std::size_t operator()(
        const std::unordered_set<const MoveInfo*>& move_set
    ) const {
        std::vector sorted_moves(move_set.begin(), move_set.end());
        std::sort(sorted_moves.begin(), sorted_moves.end());

        std::size_t hash = 0;
        int i = 0;
        for (const auto& move : sorted_moves) {
            const int shift = i *
                std::bit_width(static_cast<uint>(Move::Count));
            hash |= static_cast<std::size_t>(move->move) << shift;
            i++;
        }
        return hash;
    }
};

inline
std::vector<
    std::pair<
        std::pair<Pokemon, Ability>,
        std::vector<BattleEntry>
    >
> get_battle_entries(
    std::vector<CustomPokemon*>& players,
    std::vector<CustomPokemon*>& opponents
) {
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleEntry>
        >
    > pokemon_to_battles{};
    pokemon_to_battles.reserve(
        players.size() + opponents.size()
    );
    for (auto [
             player_index,
             player_pokemon
         ] : std::views::enumerate(players)
    ) {
        auto player_name = player_pokemon->name;
        if constexpr (TEAM_ONLY && !team.contains(player_name)) {
            continue;
        }
        if (TEAM_ONLY) {
            change_stats(player_pokemon);
        }
        std::vector<BattleEntry> battles_for_player{};
        for (const auto [
                 opponent_index,
                 opponent_pokemon
             ] : std::views::enumerate(opponents)
        ) {
            battles_for_player.emplace_back(
                BattleEntry{
                    .player_index = static_cast<size_t>(player_index),
                    .player = *player_pokemon,
                    .opponent_index = static_cast<size_t>(opponent_index),
                    .opponent = *opponent_pokemon,
                }
            );
        }
        pokemon_to_battles.push_back(
            std::make_pair(
                std::make_pair(
                    player_name,
                    player_pokemon->ability
                ),
                battles_for_player
            )
        );
    }
    return pokemon_to_battles;
}

inline
std::vector<
    std::pair<
        std::pair<Pokemon, Ability>,
        std::vector<BattleResultEntry>
    >
> do_battles(
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleEntry>
        >
    >& pokemon_to_battles
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

    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleResultEntry>
        >
    > pokemon_to_battle_result_entries{};
    for (size_t i = 0; i < pokemon_to_battles.size(); ++i) {
        auto& [
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
            std::pair(pokemon, std::move(battle_result_entries))
        );
    }
    return pokemon_to_battle_result_entries;
}

inline std::unordered_map<
    std::pair<Pokemon, Ability>,
    std::vector<CustomPokemon>,
    PokemonPairHash
> get_player_pokemon_with_top_3_moves(
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
    >& pokemon_to_battle_result_entries
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
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<CustomPokemon>,
        PokemonPairHash
    > player_pokemon_to_forms =
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
    return player_pokemon_to_forms;
}

inline std::vector<CustomPokemon*> get_players(
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<CustomPokemon>,
        PokemonPairHash
    >& pokemon_to_forms
) {
    std::vector<CustomPokemon*> players = {};
    for (auto& player_pokemon_forms :
         pokemon_to_forms | std::views::values
    ) {
        for (auto& player_pokemon : player_pokemon_forms) {
            players.emplace_back(&player_pokemon);
        }
    }
    return players;
}

inline double get_score(
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        uint64_t,
        PokemonPairHash
    >& times_opponents_beaten,
    const std::unordered_set<
        std::pair<Pokemon, Ability>,
        PokemonPairHash
    >& opponents_beaten,
    const std::unordered_set<
        std::pair<Pokemon, Ability>,
        PokemonPairHash
    >& opponents_beat_by_others
) {
    double score = 0;
    for (const auto& o_pair : opponents_beaten) {
        if (opponents_beat_by_others.contains(o_pair)) {
            score -= 1000000000;
            continue;
        }
        if (times_opponents_beaten[o_pair] != 0) {
            score += 1.0 / static_cast<double>(times_opponents_beaten[o_pair]);
        } else {
            score += 1;
        }
    }
    return score;
}

inline
std::unordered_map<
    std::pair<Pokemon, Ability>,
    std::pair<
        std::unordered_set<const MoveInfo*>,
        std::vector<std::pair<Pokemon, Ability>>
    >,
    PokemonPairHash
> print_best_six(
    std::vector<CustomPokemon*>& opponents,
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleResultEntry>
        >
    >& pokemon_to_battle_result_entries
) {
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::pair<
            int,
            std::unordered_map<
                std::unordered_set<const MoveInfo*>,
                int,
                MoveInfoSetHash
            >
        >,
        PokemonPairHash
    > pokemon_to_wins_and_moves_with_win_counts{};
    for (const auto& [
             player_pokemon,
             battle_result_entries
         ] : pokemon_to_battle_result_entries
    ) {
        for (const auto& [
                 player_index,
                 opponent_index,
                 won,
                 player_moves,
                 opponent_moves
             ] : battle_result_entries
        ) {
            if (won) {
                auto& [
                    wins,
                    move_counts
                ] = pokemon_to_wins_and_moves_with_win_counts[player_pokemon];
                wins++;
                std::unordered_set<const MoveInfo*> moves{};
                for (const auto& move : player_moves) {
                    moves.insert(
                        &MOVE_INFO_MAP.at(static_cast<int>(std::get<0>(move)))
                    );
                }
                move_counts[moves]++;
            }
        }
    }

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
        const uint8_t number_of_moves = 4;
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
            Move::Counter,
            Move::MirrorCoat
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
        for (const auto& [
                 pokemon_and_ability,
                 battle_results
             ] : pokemon_to_battle_result_entries
        ) {
            for (const auto& [
                     player_index,
                     opponent_index,
                     won,
                     player_moves,
                     opponent_moves
                 ] : battle_results
            ) {
                if (!won) {
                    continue;
                }
                const auto& opponent = opponents[opponent_index];
                pokemon_to_beaten_opponents[pokemon_and_ability].emplace_back(
                    opponent->name,
                    opponent->ability
                );
            }
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
    for (const auto& pokemon : opponents) {
        opponents_to_total_times_beaten[
            {pokemon->name, pokemon->ability}
        ] = 0;
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
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::pair<
            std::unordered_set<const MoveInfo*>,
            std::vector<std::pair<Pokemon, Ability>>
        >,
        PokemonPairHash
    > pokemon_team_member_to_moves_and_loss_list{};
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        uint16_t,
        PokemonPairHash
    > opponents_to_times_beaten{};
    for (const auto& pokemon : opponents) {
        opponents_to_times_beaten[
            {pokemon->name, pokemon->ability}
        ] = 0;
    }

    std::unordered_set<
        std::pair<Pokemon, Ability>,
        PokemonPairHash
    > opponents_beat_by_others{};
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
            bool found = false;
            for (const auto& [pair,
                _
                ]: pokemon_team_member_to_moves_and_loss_list
                ) {
                if (pair.first == pokemon.first) {
                    found = true;
                }
            }
            if (found) {
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
                    opponents_beaten,
                    opponents_beat_by_others
                );
            } else if (number_beaten == max_beaten) {
                double score = get_score(
                    opponents_to_total_times_beaten,
                    opponents_beaten,
                    opponents_beat_by_others
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

        const auto& opponents_beaten =
            pokemon_to_opponents_beaten.second;
        for (const auto& o : opponents_beaten) {
            opponents_beat_by_others.insert(o);
        }
        std::vector<std::pair<Pokemon, Ability>> opponents_lost_to{};
        opponents_lost_to.reserve(opponents.size());
        for (const auto& opponent : opponents) {
            if (auto p = std::pair{opponent->name, opponent->ability};
                !opponents_beaten.contains(p)
            ) {
                opponents_lost_to.emplace_back(p);
            }
        }
        pokemon_team_member_to_moves_and_loss_list[
            pokemon_to_opponents_beaten.first
        ] = (
            std::pair(
                move_set,
                opponents_lost_to
            )
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
        for (const auto& [opponent, opponent_ability] : losses) {
            std::cout
                << "    "
                << POKEMON_TO_STRING.at(opponent)
                << "  "
                << ABILITY_TO_STRING.at(opponent_ability)
                << '\n';
        }
    }
    return pokemon_team_member_to_moves_and_loss_list;
}

inline std::vector<
    std::pair<std::pair<Pokemon, Ability>,
              std::vector<BattleResultEntry>>
> do_round_robin(
    std::vector<CustomPokemon*> players,
    std::vector<CustomPokemon*> opponents
) {
    // The CustomPokemon in the BattleEntry are unique copies
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleEntry>
        >
    > pokemon_to_battles = get_battle_entries(players, opponents);

    // BattleResultEntry only holds indices to CustomPokemon
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleResultEntry>
        >
    > pokemon_to_battle_result_entries = do_battles(pokemon_to_battles);
    return pokemon_to_battle_result_entries;
}

inline
std::vector<
    std::pair<
        std::pair<Pokemon, Ability>,
        std::vector<BattleEntry>
    >
> get_battle_entries(
    std::vector<CustomPokemon>& players,
    std::vector<CustomPokemon>& opponents
) {
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleEntry>
        >
    > pokemon_to_battles{};
    pokemon_to_battles.reserve(
        players.size() + opponents.size()
    );
    for (auto [
             player_index,
             player_pokemon
         ] : std::views::enumerate(players)
    ) {
        auto player_name = player_pokemon.name;
        if constexpr (TEAM_ONLY && !team.contains(player_name)) {
            continue;
        }
        if (TEAM_ONLY) {
            change_stats(&player_pokemon);
        }
        std::vector<BattleEntry> battles_for_player{};
        for (const auto [
                 opponent_index,
                 opponent_pokemon
             ] : std::views::enumerate(opponents)
        ) {
            battles_for_player.emplace_back(
                BattleEntry{
                    .player_index = static_cast<size_t>(player_index),
                    .player = player_pokemon,
                    .opponent_index = static_cast<size_t>(opponent_index),
                    .opponent = opponent_pokemon,
                }
            );
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
    return pokemon_to_battles;
}

inline std::vector<
    std::pair<std::pair<Pokemon, Ability>,
              std::vector<BattleResultEntry>>
> do_round_robin(
    std::vector<CustomPokemon> players,
    std::vector<CustomPokemon> opponents
) {
    // The CustomPokemon in the BattleEntry are unique copies
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleEntry>
        >
    > pokemon_to_battles = get_battle_entries(players, opponents);

    // BattleResultEntry only holds indices to CustomPokemon
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleResultEntry>
        >
    > pokemon_to_battle_result_entries = do_battles(pokemon_to_battles);
    return pokemon_to_battle_result_entries;
}

inline void round_robin(
    const std::unordered_map<
        std::string,
        SerebiiPokemon
    >& pokemon_name_to_serebii_pokemon
) {
    // These CustomPokemon must stay in memory
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<CustomPokemon>,
        PokemonPairHash
    > pokemon_to_forms =
        get_all_pokemon_to_analyze(pokemon_name_to_serebii_pokemon);

    // These CustomPokemon* rely on pokemon_to_forms staying in memory
    std::vector<CustomPokemon*> players = get_players(pokemon_to_forms);
    std::vector<CustomPokemon*> opponents = get_players(pokemon_to_forms);

    // BattleResultEntry only holds indices to CustomPokemon
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleResultEntry>
        >
    > pokemon_to_battle_result_entries =
        do_round_robin(players, opponents);

    // These CustomPokemon must stay in memory
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<CustomPokemon>,
        PokemonPairHash
    > player_pokemon_to_forms = get_player_pokemon_with_top_3_moves(
        pokemon_name_to_serebii_pokemon,
        pokemon_to_battle_result_entries
    );

    // These CustomPokemon* rely on player_pokemon_to_forms staying in memory
    std::vector<CustomPokemon*> better_players =
        get_players(player_pokemon_to_forms);

    // BattleResultEntry only holds indices to CustomPokemon
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleResultEntry>
        >
    > best_moves_pokemon_to_battle_result_entries =
        do_round_robin(better_players, opponents);

    std::cout << "Done battling" << std::endl;

    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::pair<
            std::unordered_set<const MoveInfo*>,
            std::vector<std::pair<Pokemon, Ability>>
        >,
        PokemonPairHash
    > team = print_best_six(
        opponents,
        best_moves_pokemon_to_battle_result_entries
    );

    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::unordered_map<
            EVSet,
            std::vector<size_t>,
            EVSetHash
        >,
        PokemonPairHash
    > pokemon_and_ability_to_evs_and_losses_beaten{};
    for (const auto& [
             pair,
             battle_results
         ] : best_moves_pokemon_to_battle_result_entries
    ) {
        if (!team.contains(pair)) {
            continue;
        }
        pokemon_and_ability_to_evs_and_losses_beaten.try_emplace(pair);
        // Get all the losses for the pokemon
        std::unordered_map<
            std::pair<Pokemon, Ability>,
            std::vector<BattleResultEntry>,
            PokemonPairHash
        > pokemon_to_losses{};
        for (const auto& battle_result : battle_results) {
            const auto& [
                original_player_index,
                original_opponent_index,
                won,
                player_moves,
                opponent_moves
            ] = battle_result;
            if (won) {
                continue;
            }
            const auto& original_player =
                better_players[original_player_index];
            pokemon_to_losses[
                std::make_pair(
                    original_player->name,
                    original_player->ability
                )
            ].emplace_back(battle_result);
        }
        for (auto& [
                 p,
                 losses
             ] : pokemon_to_losses
        ) {
            assert(p == pair);
            pokemon_and_ability_to_evs_and_losses_beaten[p] = get_ev_sets(
                better_players,
                opponents,
                losses
            );
        }
        // (0.6 * nature) increase per 4 EVs
        // (stat * 1.1) for ability
    }

    printf("");
}


#endif //GEN_IV_BATTLE_HALL_ANALYZER_ROUND_ROBIN_H
