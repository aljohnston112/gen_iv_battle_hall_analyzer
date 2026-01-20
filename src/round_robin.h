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

struct BattleEntry {
    size_t player_index;
    CustomPokemon player;
    size_t opponent_index;
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

inline void get_battle_entries(
    const std::vector<CustomPokemon*>& players,
    const std::vector<CustomPokemon*>& opponents,
    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleEntry>
        >
    >& pokemon_to_battles
) {
    pokemon_to_battles.reserve(
        players.size() + opponents.size()
    );
    for (auto [
             player_index,
             player_pokemon
         ] : std::views::enumerate(players)
    ) {
        auto player_name = player_pokemon->name;
        if (TEAM_ONLY && !team.contains(player_name)) {
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
}

inline void battle_all(
    BattleEntry& battle_entry,
    std::promise<BattleResultEntry>&& promise
) {
    promise.set_value(
        battle(
            battle_entry.player_index,
            &battle_entry.player,
            battle_entry.opponent_index,
            &battle_entry.opponent
        )
    );
}

inline void do_battles(
    std::vector<
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
    std::vector<
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

inline void get_evs_needed_for_diff(
    const int current_stat,
    const int diff,
    int& evs,
    int& nature_evs
) {
    evs = std::ceil(diff / (LEVEL / 100.0)) * 4;
    if (current_stat * 1.1 >= current_stat + (evs / 4)) {
        nature_evs = 0;
    } else {
        nature_evs = std::ceil(
            std::ceil(((current_stat + diff) / 1.1) - current_stat) / (LEVEL /
                100.0)
        ) * 4;
    }
}

inline void do_battles_with_updated_evs(
    const std::vector<CustomPokemon*>& opponents,
    const std::pair<Pokemon, Ability>& pokemon,
    const size_t player_index,
    CustomPokemon* player_pokemon,
    std::vector<std::tuple<Stat, int, bool>>& evs,
    const size_t opponent_index,
    CustomPokemon* const opponent,
    const std::vector<size_t>& losses_to_beat,
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<
            std::pair<
                std::vector<std::tuple<Stat, int, bool>>,
                std::unordered_set<size_t>
            >
        >,
        PokemonPairHash
    >& stat_and_ability_to_evs_and_losses_beaten
) {
    if (evs.empty()) {
        return;
    }
    if (battle(player_index, player_pokemon, opponent_index, opponent).won) {
        std::vector<BattleEntry> battles{};
        for (const auto o_index : losses_to_beat) {
            battles.emplace_back(
                BattleEntry{
                    player_index,
                    *player_pokemon,
                    o_index,
                    *opponents[o_index]
                }
            );
        }
        const auto battle_result_entries =
            thread_pool::ThreadPool::getCPUWorkInstance()->
            createAndRunTasks<
                BattleResultEntry,
                std::vector<BattleEntry>,
                BattleEntry
            >(battle_all, battles);
        std::unordered_set<size_t> losses_beaten{};
        for (auto& results :
             battle_result_entries
        ) {
            if (results.won) {
                losses_beaten.insert(results.opponent_index);
            }
        }
        if (losses_beaten.size() > 0) {
            if (!evs.empty()) {
                stat_and_ability_to_evs_and_losses_beaten
                    [pokemon].emplace_back(
                        std::pair{
                            evs,
                            losses_beaten
                        }
                    );
            }
        }
    }
}

struct EVSet {
    int health;
    int attack;
    int defense;
    int special_attack;
    int special_defense;
    int speed;
    Stat nature_stat;

    bool is_less_than(const EVSet& b) const {
        return (nature_stat == b.nature_stat || nature_stat == Stat::NO_STAT) &&
            health <= b.health &&
            attack <= b.attack &&
            defense <= b.defense &&
            special_attack <= b.special_attack &&
            special_defense <= b.special_defense &&
            speed <= b.speed;
    }
};

struct EVSetHash {
    std::size_t operator()(const EVSet& p) const {
        return static_cast<int>(p.health) << 0 |
            static_cast<uint64_t>(p.attack) << 8 |
            static_cast<uint64_t>(p.defense) << 16 |
            static_cast<uint64_t>(p.special_attack) << 24 |
            static_cast<uint64_t>(p.special_defense) << 32 |
            static_cast<uint64_t>(p.speed) << 40 |
            static_cast<uint64_t>(p.nature_stat) << 48;
    }
};

typedef std::array<Stat, static_cast<int>(Stat::NO_STAT)> StatPermutationState;

inline StatPermutationState create_initial_stat_permutation_state() {
    StatPermutationState state{};
    for (int i = 0; i < static_cast<int>(Stat::NO_STAT); i++) {
        state[i] = static_cast<Stat>(i);
    }
    return state;
}

inline bool next_stat_permutation(
    StatPermutationState& state
) {
    std::set<int> not_used{};
    constexpr int last_stat_index = static_cast<int>(Stat::NO_STAT) - 1;
    int current_index = last_stat_index;
    not_used.emplace(static_cast<int>(state[current_index--]));

    int last_size = 0;
    int current_size = not_used.size();
    while (last_size != current_size) {
        last_size = current_size;
        if (static_cast<int>(state[current_index]) == last_stat_index
        ) {
            not_used.emplace(static_cast<int>(state[current_index--]));
        }
        if (current_index == -1) {
            return false;
        }
        int next_stat = static_cast<int>(state[current_index]) + 1;
        while (!not_used.contains(next_stat) &&
            next_stat != static_cast<int>(Stat::NO_STAT)
        ) {
            next_stat++;
        }
        if (next_stat == static_cast<int>(Stat::NO_STAT)) {
            not_used.emplace(static_cast<int>(state[current_index--]));
        }
        current_size = not_used.size();
    }

    int next_stat = static_cast<int>(state[current_index]) + 1;
    while (!not_used.contains(next_stat)) {
        next_stat++;
    }
    not_used.emplace(static_cast<int>(state[current_index]));
    state[current_index++] = static_cast<Stat>(next_stat);
    not_used.erase(next_stat);
    while (not_used.size() > 0) {
        next_stat = *not_used.cbegin();
        state[current_index++] = static_cast<Stat>(next_stat);
        not_used.erase(next_stat);
    }
    return true;
}

inline std::pair<bool, bool> get_attack_evs(
    const std::vector<CustomPokemon*>& players,
    const std::vector<CustomPokemon*>& opponents,
    std::vector<
        std::pair<
            EVSet,
            const size_t>
    >& evs,
    BattleResultEntry& battle_results,
    const uint16_t evs_left,
    const bool nature_used,
    const EVSet& evs_used
) {
    std::pair won{false, false};
    auto& [
        original_player_index,
        opponent_index,
        _,
        player_moves,
        opponent_moves
    ] = battle_results;
    // TODO get exact EV
    const auto original_player =
        players[original_player_index];
    CustomPokemon current_player = *original_player;
    const auto original_player_attack =
        original_player->stats[static_cast<int>(Stat::ATTACK)];
    if (evs_left >= 252) {
        current_player.stats[
            static_cast<int>(Stat::ATTACK)
        ] = original_player_attack + (252 / 4) * 0.6;
        auto opponent = opponents[opponent_index];
        BattleState new_battle_state{
            &current_player,
            opponent
        };
        if (const auto new_battle_results =
                new_battle_state.battle_indices(
                    original_player_index,
                    opponent_index
                );
            new_battle_results.won
        ) {
            won.first = true;
            evs.emplace_back(
                EVSet{
                    .health = evs_used.health,
                    .attack = 252,
                    .defense = evs_used.defense,
                    .special_attack = evs_used.special_attack,
                    .special_defense = evs_used.special_defense,
                    .speed = evs_used.speed,
                    .nature_stat = evs_used.nature_stat
                },
                opponent_index
            );
        }
        if (!nature_used) {
            current_player.stats[
                static_cast<int>(Stat::ATTACK)
            ] = original_player_attack + (252 / 4) * (LEVEL / 100.0) * 1.1;
            BattleState new_battle_state{
                &current_player,
                opponent
            };
            if (const auto new_battle_results =
                    new_battle_state.battle_indices(
                        original_player_index,
                        opponent_index
                    );
                new_battle_results.won
            ) {
                won.second = true;
                evs.emplace_back(
                    EVSet{
                        .health = evs_used.health,
                        .attack = 252,
                        .defense = evs_used.defense,
                        .special_attack = evs_used.special_attack,
                        .special_defense = evs_used.special_defense,
                        .speed = evs_used.speed,
                        .nature_stat = Stat::ATTACK
                    },
                    opponent_index
                );
            }
        }
    }
    return won;
}

inline std::pair<bool, bool> get_defense_evs(
    const std::vector<CustomPokemon*>& players,
    const std::vector<CustomPokemon*>& opponents,
    std::vector<
        std::pair<
            EVSet,
            const size_t>
    >& evs,
    const BattleResultEntry& battle_results,
    const uint16_t evs_left,
    const bool nature_used,
    const EVSet& evs_used
) {
    std::pair won{false, false};
    const auto& [
        original_player_index,
        opponent_index,
        _,
        player_moves,
        opponent_moves
    ] = battle_results;
    // TODO get exact EV
    const auto original_player = players[original_player_index];
    const auto opponent = opponents[opponent_index];
    CustomPokemon current_player = *original_player;
    const auto original_player_attack =
        original_player->stats[static_cast<int>(Stat::DEFENSE)];
    if (evs_left >= 252) {
        current_player.stats[
            static_cast<int>(Stat::DEFENSE)
        ] = original_player_attack + (252 / 4) * (LEVEL / 100.0);
        BattleState new_battle_state{
            &current_player,
            opponent
        };
        if (const auto new_battle_results =
                new_battle_state.battle_indices(
                    original_player_index,
                    opponent_index
                );
            new_battle_results.won
        ) {
            won.first = true;
            evs.emplace_back(
                EVSet{
                    .health = evs_used.health,
                    .attack = evs_used.attack,
                    .defense = 252,
                    .special_attack = evs_used.special_attack,
                    .special_defense = evs_used.special_defense,
                    .speed = evs_used.speed,
                    .nature_stat = evs_used.nature_stat
                },
                opponent_index
            );
        }
        if (!nature_used) {
            current_player.stats[
                static_cast<int>(Stat::DEFENSE)
            ] = original_player_attack + (252 / 4) * (LEVEL / 100.0) * 1.1;
            BattleState new_battle_state{
                &current_player,
                opponent
            };
            if (const auto new_battle_results =
                    new_battle_state.battle_indices(
                        original_player_index,
                        opponent_index
                    );
                new_battle_results.won
            ) {
                won.second = true;
                evs.emplace_back(
                    EVSet{
                        .health = evs_used.health,
                        .attack = evs_used.attack,
                        .defense = 252,
                        .special_attack = evs_used.special_attack,
                        .special_defense = evs_used.special_defense,
                        .speed = evs_used.speed,
                        .nature_stat = Stat::DEFENSE
                    },
                    opponent_index
                );
            }
        }
    }
    return won;
}

inline std::pair<bool, bool> get_special_attack_evs(
    const std::vector<CustomPokemon*>& players,
    const std::vector<CustomPokemon*>& opponents,
    std::vector<
        std::pair<
            EVSet,
            const size_t>
    >& evs,
    const BattleResultEntry& battle_results,
    const uint16_t evs_left,
    const bool nature_used,
    const EVSet& evs_used
) {
    std::pair won{false, false};
    const auto& [
        original_player_index,
        opponent_index,
        _,
        player_moves,
        opponent_moves
    ] = battle_results;
    // TODO get exact EV
    const auto original_player = players[original_player_index];
    const auto opponent = opponents[opponent_index];
    CustomPokemon current_player = *original_player;
    const auto original_player_attack =
        original_player->stats[static_cast<int>(Stat::SPECIAL_ATTACK)];
    if (evs_left >= 252) {
        current_player.stats[
            static_cast<int>(Stat::SPECIAL_ATTACK)
        ] = original_player_attack + (252 / 4) * (LEVEL / 100.0);
        BattleState new_battle_state{
            &current_player,
            opponent
        };
        if (const auto new_battle_results =
                new_battle_state.battle_indices(
                    original_player_index,
                    opponent_index
                );
            new_battle_results.won
        ) {
            won.first = true;
            evs.emplace_back(
                EVSet{
                    .health = evs_used.health,
                    .attack = evs_used.attack,
                    .defense = evs_used.defense,
                    .special_attack = 252,
                    .special_defense = evs_used.special_defense,
                    .speed = evs_used.speed,
                    .nature_stat = evs_used.nature_stat
                },
                opponent_index
            );
        }
        if (!nature_used) {
            current_player.stats[
                static_cast<int>(Stat::SPECIAL_ATTACK)
            ] = original_player_attack + (252 / 4) * (LEVEL / 100.0) * 1.1;
            BattleState new_battle_state{
                &current_player,
                opponent
            };
            if (const auto new_battle_results =
                    new_battle_state.battle_indices(
                        original_player_index,
                        opponent_index
                    );
                new_battle_results.won
            ) {
                won.second = true;
                evs.emplace_back(
                    EVSet{
                        .health = evs_used.health,
                        .attack = evs_used.attack,
                        .defense = evs_used.defense,
                        .special_attack = 252,
                        .special_defense = evs_used.special_defense,
                        .speed = evs_used.speed,
                        .nature_stat = Stat::SPECIAL_ATTACK
                    },
                    opponent_index
                );
            }
        }
    }
    return won;
}

inline std::pair<bool, bool> get_special_defense_evs(
    const std::vector<CustomPokemon*>& players,
    const std::vector<CustomPokemon*>& opponents,
    std::vector<
        std::pair<
            EVSet,
            const size_t>
    >& evs,
    const BattleResultEntry& battle_results,
    const uint16_t evs_left,
    const bool nature_used,
    const EVSet& evs_used
) {
    std::pair won{false, false};
    const auto& [
        original_player_index,
        opponent_index,
        _,
        player_moves,
        opponent_moves
    ] = battle_results;
    // TODO get exact EV
    const auto original_player = players[original_player_index];
    const auto opponent = opponents[opponent_index];
    CustomPokemon current_player = *original_player;
    const auto original_player_attack =
        original_player->stats[static_cast<int>(Stat::SPECIAL_DEFENSE)];
    if (evs_left >= 252) {
        current_player.stats[
            static_cast<int>(Stat::SPECIAL_DEFENSE)
        ] = original_player_attack + ((252 / 4) * (LEVEL / 100.0));
        BattleState new_battle_state{
            &current_player,
            opponent
        };
        if (const auto new_battle_results =
                new_battle_state.battle_indices(
                    original_player_index,
                    opponent_index
                );
            new_battle_results.won
        ) {
            won.first = true;
            evs.emplace_back(
                EVSet{
                    .health = evs_used.health,
                    .attack = evs_used.attack,
                    .defense = evs_used.defense,
                    .special_attack = evs_used.special_attack,
                    .special_defense = 252,
                    .speed = evs_used.speed,
                    .nature_stat = evs_used.nature_stat
                },
                opponent_index
            );
        }
        if (!nature_used) {
            current_player.stats[
                static_cast<int>(Stat::SPECIAL_DEFENSE)
            ] = (original_player_attack + ((252 / 4) * (LEVEL / 100.0))) * 1.1;
            BattleState new_battle_state{
                &current_player,
                opponent
            };
            if (const auto new_battle_results =
                    new_battle_state.battle_indices(
                        original_player_index,
                        opponent_index
                    );
                new_battle_results.won
            ) {
                won.second = true;
                evs.emplace_back(
                    EVSet{
                        .health = evs_used.health,
                        .attack = evs_used.attack,
                        .defense = evs_used.defense,
                        .special_attack = evs_used.special_attack,
                        .special_defense = 252,
                        .speed = evs_used.speed,
                        .nature_stat = Stat::SPECIAL_DEFENSE
                    },
                    opponent_index
                );
            }
        }
    }
    return won;
}

inline std::pair<bool, bool> get_speed_evs(
    const std::vector<CustomPokemon*>& players,
    const std::vector<CustomPokemon*>& opponents,
    std::vector<
        std::pair<
            EVSet,
            const size_t>
    >& evs,
    const BattleResultEntry& battle_results,
    const uint16_t evs_left,
    const bool nature_used,
    const EVSet& evs_used
) {
    std::pair won{false, false};
    const auto& [
        original_player_index,
        opponent_index,
        _,
        player_moves,
        opponent_moves
    ] = battle_results;
    const auto original_player = players[original_player_index];
    const auto opponent = opponents[opponent_index];
    CustomPokemon current_player = *original_player;
    const auto original_player_speed =
        original_player->stats[static_cast<int>(Stat::SPEED)];
    const auto speed_diff =
        opponent->stats.at(static_cast<int>(Stat::SPEED)) -
        original_player_speed + 1;
    if (speed_diff > 0) {
        int speed_evs;
        int speed_nature_evs;
        get_evs_needed_for_diff(
            original_player_speed,
            speed_diff,
            speed_evs,
            speed_nature_evs
        );
        if (speed_evs <= evs_left) {
            current_player.stats[
                static_cast<int>(Stat::SPEED)
            ] = original_player_speed + (speed_evs / 4) * (LEVEL / 100.0);
            BattleState new_battle_state{
                &current_player,
                opponent
            };
            if (const auto new_battle_results =
                    new_battle_state.battle_indices(
                        original_player_index,
                        opponent_index
                    );
                new_battle_results.won
            ) {
                won.first = true;
                evs.emplace_back(
                    EVSet{
                        .health = evs_used.health,
                        .attack = evs_used.attack,
                        .defense = evs_used.defense,
                        .special_attack = evs_used.special_attack,
                        .special_defense = evs_used.special_defense,
                        .speed = speed_evs,
                        .nature_stat = evs_used.nature_stat
                    },
                    opponent_index
                );
            }
        }
        if (!nature_used && speed_nature_evs <= evs_left) {
            current_player.stats[
                    static_cast<int>(Stat::SPEED)
                ] = original_player_speed +
                ((speed_nature_evs / 4) * (LEVEL / 100.0)) * 1.1;
            BattleState new_battle_state{
                &current_player,
                opponent
            };
            if (const auto new_battle_results =
                    new_battle_state.battle_indices(
                        original_player_index,
                        opponent_index
                    );
                new_battle_results.won
            ) {
                won.second = true;
                evs.emplace_back(
                    EVSet{
                        .health = evs_used.health,
                        .attack = evs_used.attack,
                        .defense = evs_used.defense,
                        .special_attack = evs_used.special_attack,
                        .special_defense = evs_used.special_defense,
                        .speed = speed_nature_evs,
                        .nature_stat = Stat::SPEED
                    },
                    opponent_index
                );
            }
        }
    }
    return won;
}

inline int calculate_hp_diff_to_survive(
    const CustomPokemon& player,
    const std::vector<std::tuple<Move, int, int>>& player_moves,
    const CustomPokemon& opponent,
    const std::vector<std::tuple<Move, int, int>>& opponent_moves
) {
    const uint16_t p_hp = player.stats.at(static_cast<int>(Stat::HEALTH));
    auto damage = 0;
    for (const auto opponent_move : opponent_moves) {
        damage += std::get<2>(opponent_move);
    }
    auto hp_diff = damage - p_hp + 1;
    double factors_sum = 0;
    // Add hp to cover poison and burn damage
    int turns_poisoned_or_burned = 0;
    for (const auto [move, _, _] :
         opponent_moves
    ) {
        if (move_has_flag(
                move,
                MoveFlag::POISONS_DEFENDER
            ) ||
            move_has_flag(
                move,
                MoveFlag::BURNS_DEFENDER
            ) ||
            move == Move::TriAttack
        ) {
            turns_poisoned_or_burned++;
        } else if (turns_poisoned_or_burned > 0) {
            turns_poisoned_or_burned++;
        }
    }
    int turns_of_flame_body = 0;
    if (opponent.ability == Ability::FlameBody ||
        opponent.ability == Ability::PoisonPoint
    ) {
        for (const auto move : player_moves) {
            if (move_has_flag(
                    std::get<0>(move),
                    MoveFlag::MAKES_CONTACT)
            ) {
                turns_of_flame_body++;
            } else if (turns_of_flame_body > 0) {
                turns_of_flame_body++;
            }
        }
    }
    if (turns_of_flame_body > turns_poisoned_or_burned) {
        turns_poisoned_or_burned = turns_of_flame_body;
    }
    if (turns_poisoned_or_burned > 0) {
        if (opponent.ability == Ability::Truant) {
            turns_poisoned_or_burned +=
                (player_moves.size() - opponent_moves.size());
        }
        if (player.stats.at(static_cast<int>(Stat::SPEED)) >
            opponent.stats.at(static_cast<int>(Stat::SPEED))
        ) {
            turns_poisoned_or_burned++;
        }
        factors_sum += turns_poisoned_or_burned / 8.0;
    }

    int turns_badly_poisoned = 0;
    for (const auto [move, _, _] :
         opponent_moves
    ) {
        if (move_has_flag(move, MoveFlag::BADLY_POISONS)) {
            turns_badly_poisoned++;
        } else if (turns_badly_poisoned > 0) {
            turns_badly_poisoned++;
        }
    }
    assert(turns_badly_poisoned < 6);
    if (turns_badly_poisoned > 0) {
        int den = 15;
        int sub = 0;
        for (int i = 0; i < turns_badly_poisoned; i++) {
            den -= sub;
            sub += (i + 2);
        }
        factors_sum += den / 16.0;
    }

    // Weather
    if (opponent.ability == Ability::SandStream ||
        opponent.ability == Ability::SnowWarning
    ) {
        const int turns = std::max(opponent_moves.size(), player_moves.size());
        factors_sum += turns / 16.0;
    }

    // Rough Skin
    if (opponent.ability == Ability::RoughSkin ||
        (opponent_moves.size() > 0 &&
            std::get<0>(opponent_moves[0]) == Move::Transform &&
            player.ability == Ability::RoughSkin)
    ) {
        int rough_skinned = 0;
        for (const auto& move : player_moves) {
            if (move_has_flag(std::get<0>(move), MoveFlag::MAKES_CONTACT)) {
                rough_skinned++;
            }
        }
        factors_sum += rough_skinned / 8.0;
    }

    // Aftermath
    if (opponent.ability == Ability::Aftermath ||
        (opponent_moves.size() > 0 &&
            std::get<0>(opponent_moves[0]) == Move::Transform &&
            player.ability == Ability::Aftermath
        )
    ) {
        factors_sum += 1 / 4.0;
    }
    assert(factors_sum < 1.0);
    hp_diff += ((1.0 / (1.0 - factors_sum)) - 1.0) * (damage + 1);

    // Recoil
    for (const auto& move : player_moves) {
        if (std::get<0>(move) == Move::FlareBlitz ||
            std::get<0>(move) == Move::WoodHammer ||
            std::get<0>(move) == Move::BraveBird ||
            std::get<0>(move) == Move::VoltTackle
        ) {
            hp_diff += std::get<1>(move) / 2;
        } else if (std::get<0>(move) == Move::TakeDown ||
            std::get<0>(move) == Move::DoubleEdge ||
            std::get<0>(move) == Move::Submission
        ) {
            hp_diff += std::get<1>(move) / 3;
        } else if (std::get<0>(move) == Move::BellyDrum) {
            hp_diff += (damage + 1.0);
        } else if (std::get<0>(move) == Move::HeadSmash) {
            hp_diff += std::get<1>(move);
        }
    }

    if (opponent.ability == Ability::LiquidOoze) {
        for (const auto& move : player_moves) {
            if (std::get<0>(move) == Move::GigaDrain ||
                std::get<0>(move) == Move::DrainPunch ||
                std::get<0>(move) == Move::Absorb ||
                std::get<0>(move) == Move::LeechLife ||
                std::get<0>(move) == Move::MegaDrain
            ) {
                hp_diff += std::get<1>(move) / 2.0;
            }
        }
    }
    return std::ceil(hp_diff);
}

constexpr auto max_stat_diff = ((252 / 4) * (LEVEL / 100.0));
constexpr auto max_stat_diff_nature =
    ((252 / 4) * (LEVEL / 100.0)) * 1.1;

inline std::pair<bool, bool> get_health_evs(
    const std::vector<CustomPokemon*>& players,
    const std::vector<CustomPokemon*>& opponents,
    std::vector<
        std::pair<
            EVSet,
            const size_t>
    >& evs,
    const BattleResultEntry& battle_results,
    const uint16_t evs_left,
    const bool nature_used,
    const EVSet& evs_used
) {
    const auto& [
        original_player_index,
        opponent_index,
        _,
        player_moves,
        opponent_moves
    ] = battle_results;
    const auto original_player = players[original_player_index];
    const auto opponent = opponents[opponent_index];
    const auto p_hp =
        original_player->stats[static_cast<int>(Stat::HEALTH)];
    const auto hp_diff =
        calculate_hp_diff_to_survive(
            *original_player,
            player_moves,
            *opponent,
            opponent_moves
        );
    assert(hp_diff > 0);
    int hp_evs;
    int hp_nature_evs;
    get_evs_needed_for_diff(
        p_hp,
        hp_diff,
        hp_evs,
        hp_nature_evs
    );
    assert(hp_nature_evs < hp_evs || (hp_evs == hp_nature_evs && hp_evs == 8));
    std::pair won{false, false};
    if (hp_evs <= evs_left) {
        CustomPokemon current_player = *original_player;
        current_player.stats[
            static_cast<int>(Stat::HEALTH)
        ] = (p_hp + hp_diff);
        BattleState new_battle_state{
            &current_player,
            opponent
        };
        if (original_player->name == Pokemon::Illumise &&
            original_player->ability == Ability::TintedLens &&
            opponent->name == Pokemon::Venonat &&
            opponent->ability == Ability::Compoundeyes
        ) {
            volatile int a;
        }
        const auto new_battle_results =
            new_battle_state.battle_indices(
                original_player_index,
                opponent_index
            );
        if (new_battle_results.won) {
            won.first = true;
            evs.emplace_back(
                EVSet{
                    .health = hp_evs,
                    .attack = evs_used.attack,
                    .defense = evs_used.defense,
                    .special_attack = evs_used.special_attack,
                    .special_defense = evs_used.special_defense,
                    .speed = evs_used.speed,
                    .nature_stat = evs_used.nature_stat
                },
                opponent_index
            );
            return won;
        }
        // Opponent moves may have changed
        const auto new_hp_diff = calculate_hp_diff_to_survive(
            current_player,
            new_battle_results.player_moves,
            *opponent,
            new_battle_results.opponent_moves
        );
        assert(new_hp_diff > 0 ||
            evs_used.defense > 0 ||
            evs_used.special_defense > 0
        );
        if (new_hp_diff != hp_diff) {
            int new_hp_evs;
            int new_hp_nature_evs;
            get_evs_needed_for_diff(
                p_hp,
                new_hp_diff,
                new_hp_evs,
                new_hp_nature_evs
            );
            assert(new_hp_diff < 0);
            if (new_hp_evs <= evs_left) {
                // Opponent used different moves
                return get_health_evs(
                    players,
                    opponents,
                    evs,
                    new_battle_results,
                    evs_left,
                    nature_used,
                    evs_used
                );
            }
        }
        return won;
    }
    return won;
}

inline uint16_t get_number_of_evs(const EVSet& evs) {
    uint16_t evs_used = 0;
    evs_used += evs.health;
    evs_used += evs.attack;
    evs_used += evs.defense;
    evs_used += evs.special_attack;
    evs_used += evs.special_defense;
    evs_used += evs.speed;
    return evs_used;
}

struct EvOptimizerInput {
    const std::vector<CustomPokemon*>& players;
    const std::vector<CustomPokemon*>& opponents;
    BattleResultEntry& battle_result;
    const StatPermutationState& state;
};

inline void optimize_evs_for_battle(
    EvOptimizerInput& input,
    std::promise<
        std::vector<
            std::pair<
                EVSet,
                const size_t>
        >
    >&& ev_sets_promise
) {
    const auto& players = input.players;
    const auto& opponents = input.opponents;
    BattleResultEntry& battle_result = input.battle_result;
    const StatPermutationState& state = input.state;
    std::vector<
        std::pair<
            EVSet,
            const size_t>
    > ev_sets_to_beaten{};
    std::vector<
        std::pair<
            EVSet,
            const CustomPokemon*>
    > current_pokemon_evs_and_losses_beaten{};
    current_pokemon_evs_and_losses_beaten.emplace_back(
        EVSet{
            .health = 0,
            .attack = 0,
            .defense = 0,
            .special_attack = 0,
            .special_defense = 0,
            .speed = 0,
            .nature_stat = Stat::NO_STAT
        },
        nullptr
    );
    int current_index = 0;
    std::pair won = {false, false};
    auto player = *players[battle_result.player_index];
    auto players_for_updated_player = std::vector<CustomPokemon*>{players.size()};
    players_for_updated_player.reserve(players.size());
    players_for_updated_player.insert(
        players_for_updated_player.begin() + battle_result.player_index,
        &player
    );
    auto original_player = players[battle_result.player_index];
    const auto original_player_stats =
        original_player->stats;
    while (current_index < static_cast<int>(Stat::NO_STAT) &&
        (!won.first || !won.second)
    ) {
        std::vector<
            std::pair<
                EVSet,
                const CustomPokemon*>
        > temp = std::move(current_pokemon_evs_and_losses_beaten);
        current_pokemon_evs_and_losses_beaten.clear();
        current_pokemon_evs_and_losses_beaten.reserve(12 * temp.size());
        for (const auto& [ev_set, _] : temp) {
            const uint16_t evs_left = std::min(
                508 - get_number_of_evs(ev_set),
                252
            );
            assert(ev_set.health >= 0 && ev_set.health < 253);
            assert(ev_set.health >= 0 && ev_set.attack < 253);
            assert(ev_set.health >= 0 && ev_set.defense < 253);
            assert(ev_set.health >= 0 && ev_set.special_attack < 253);
            assert(ev_set.health >= 0 && ev_set.special_defense < 253);
            assert(ev_set.health >= 0 && ev_set.speed < 253);
            if (ev_set.health > 0) {
                player.stats[
                        static_cast<int>(Stat::HEALTH)
                    ] = original_player_stats[static_cast<int>(Stat::HEALTH)] +
                    ((ev_set.health / 4) * (LEVEL / 100.0));
            }
            if (ev_set.attack > 0) {
                if (ev_set.nature_stat == Stat::ATTACK) {
                    player.stats[
                        static_cast<int>(Stat::ATTACK)
                    ] = std::floor(original_player_stats[
                        static_cast<int>(Stat::ATTACK)
                    ] + (ev_set.speed / 4) * (LEVEL / 100.0)) * 1.1;
                } else {
                    player.stats[
                        static_cast<int>(Stat::ATTACK)
                    ] = original_player_stats[
                        static_cast<int>(Stat::ATTACK)
                    ] + ((ev_set.attack / 4) * (LEVEL / 100.0));
                }
            }
            if (ev_set.defense > 0) {
                if (ev_set.nature_stat == Stat::DEFENSE) {
                    player.stats[
                        static_cast<int>(Stat::DEFENSE)
                    ] = std::floor(original_player_stats[
                        static_cast<int>(Stat::DEFENSE)
                    ] + (ev_set.speed / 4) * (LEVEL / 100.0)) * 1.1;
                } else {
                    player.stats[
                        static_cast<int>(Stat::DEFENSE)
                    ] = original_player_stats[
                        static_cast<int>(Stat::DEFENSE)
                    ] + ((ev_set.defense / 4) * (LEVEL / 100.0));
                }
            }
            if (ev_set.special_attack > 0) {
                if (ev_set.nature_stat == Stat::SPECIAL_ATTACK) {
                    player.stats[
                        static_cast<int>(Stat::SPECIAL_ATTACK)
                    ] = std::floor(original_player_stats[
                        static_cast<int>(Stat::SPECIAL_ATTACK)
                    ] + (ev_set.speed / 4) * (LEVEL / 100.0)) * 1.1;
                } else {
                    player.stats[
                        static_cast<int>(Stat::SPECIAL_ATTACK)
                    ] = original_player_stats[
                        static_cast<int>(Stat::SPECIAL_ATTACK)
                    ] + ((ev_set.special_attack / 4) * (LEVEL / 100.0));
                }
            }
            if (ev_set.special_defense > 0) {
                if (ev_set.nature_stat == Stat::SPECIAL_DEFENSE) {
                    player.stats[
                        static_cast<int>(Stat::SPECIAL_DEFENSE)
                    ] = std::floor(original_player_stats[
                        static_cast<int>(Stat::SPECIAL_DEFENSE)
                    ] + (ev_set.speed / 4) * (LEVEL / 100.0)) * 1.1;
                } else {
                    player.stats[
                        static_cast<int>(Stat::SPECIAL_DEFENSE)
                    ] = original_player_stats[
                        static_cast<int>(Stat::SPECIAL_DEFENSE)
                    ] + ((ev_set.special_defense / 4) * (LEVEL / 100.0));
                }
            }
            if (ev_set.speed > 0) {
                if (ev_set.nature_stat == Stat::SPEED) {
                    player.stats[
                        static_cast<int>(Stat::SPEED)
                    ] = std::floor(original_player_stats[
                        static_cast<int>(Stat::SPEED)
                    ] + (ev_set.speed / 4) * (LEVEL / 100.0)) * 1.1;
                } else {
                    player.stats[
                            static_cast<int>(Stat::SPEED)
                        ] = original_player_stats[static_cast<int>(Stat::SPEED)]
                        + ((ev_set.speed / 4) * (LEVEL / 100.0));
                }
            }
            assert(
                player.stats[static_cast<int>(Stat::HEALTH)] < 414 &&
                player.stats[static_cast<int>(Stat::HEALTH)] > 81
            );
            assert(
                player.stats[static_cast<int>(Stat::ATTACK)] < 284 &&
                player.stats[static_cast<int>(Stat::ATTACK)] > 8
            );
            assert(
                player.stats[static_cast<int>(Stat::DEFENSE)] < 350 &&
                player.stats[static_cast<int>(Stat::DEFENSE)] > 8
            );
            assert(
                player.stats[static_cast<int>(Stat::SPECIAL_ATTACK)] < 284 &&
                player.stats[static_cast<int>(Stat::SPECIAL_ATTACK)] > 14
            );
            assert(
                player.stats[static_cast<int>(Stat::SPECIAL_DEFENSE)] < 350 &&
                player.stats[static_cast<int>(Stat::SPECIAL_DEFENSE)] > 25
            );
            assert(
                player.stats[static_cast<int>(Stat::SPEED)] < 284 &&
                player.stats[static_cast<int>(Stat::SPEED)] > 8
            );

            const Stat nature_stat = ev_set.nature_stat;
            bool nature_used = nature_stat != Stat::NO_STAT;
            if (evs_left > 0 &&
                player.name != Pokemon::Shedinja
            ) {
                switch (state[current_index]) {
                case Stat::HEALTH:
                    won = get_health_evs(
                        players_for_updated_player,
                        opponents,
                        ev_sets_to_beaten,
                        battle_result,
                        evs_left,
                        nature_used,
                        ev_set
                    );
                    if (!won.first) {
                        EVSet ev_max = ev_set;
                        ev_max.health = evs_left;
                        current_pokemon_evs_and_losses_beaten.emplace_back(
                            std::move(ev_max), nullptr
                        );
                    }
                    break;
                case Stat::ATTACK:
                    won = get_attack_evs(
                        players_for_updated_player,
                        opponents,
                        ev_sets_to_beaten,
                        battle_result,
                        evs_left,
                        nature_used,
                        ev_set
                    );
                    if (!won.first) {
                        EVSet ev_max = ev_set;
                        ev_max.attack = evs_left;
                        ev_max.nature_stat = Stat::ATTACK;
                        current_pokemon_evs_and_losses_beaten.emplace_back(
                            std::move(ev_max), nullptr
                        );
                    }
                    if (!won.second) {
                        EVSet ev_max_nature = ev_set;
                        ev_max_nature.attack = evs_left;
                        ev_max_nature.nature_stat = Stat::ATTACK;
                        current_pokemon_evs_and_losses_beaten.emplace_back(
                            std::move(ev_max_nature), nullptr
                        );
                    }
                    break;
                case Stat::DEFENSE:
                    won = get_defense_evs(
                        players_for_updated_player,
                        opponents,
                        ev_sets_to_beaten,
                        battle_result,
                        evs_left,
                        nature_used,
                        ev_set
                    );
                    if (!won.first) {
                        EVSet ev_max = ev_set;
                        ev_max.defense = evs_left;
                        current_pokemon_evs_and_losses_beaten.emplace_back(
                            std::move(ev_max), nullptr
                        );
                    }
                    if (!won.second) {
                        EVSet ev_max_nature = ev_set;
                        ev_max_nature.defense = evs_left;
                        ev_max_nature.nature_stat = Stat::DEFENSE;
                        current_pokemon_evs_and_losses_beaten.emplace_back(
                            std::move(ev_max_nature), nullptr
                        );
                    }
                    break;
                case Stat::SPECIAL_ATTACK:
                    won = get_special_attack_evs(
                        players_for_updated_player,
                        opponents,
                        ev_sets_to_beaten,
                        battle_result,
                        evs_left,
                        nature_used,
                        ev_set
                    );
                    if (!won.first) {
                        EVSet ev_max = ev_set;
                        ev_max.special_attack = evs_left;
                        current_pokemon_evs_and_losses_beaten.emplace_back(
                            std::move(ev_max), nullptr
                        );
                    }
                    if (!won.second) {
                        EVSet ev_max_nature = ev_set;
                        ev_max_nature.special_attack = evs_left;
                        ev_max_nature.nature_stat =
                            Stat::SPECIAL_ATTACK;
                        current_pokemon_evs_and_losses_beaten.emplace_back(
                            std::move(ev_max_nature), nullptr
                        );
                    }
                    break;
                case Stat::SPECIAL_DEFENSE:
                    won = get_special_defense_evs(
                        players_for_updated_player,
                        opponents,
                        ev_sets_to_beaten,
                        battle_result,
                        evs_left,
                        nature_used,
                        ev_set
                    );
                    if (!won.first) {
                        EVSet ev_max = ev_set;
                        ev_max.special_defense = evs_left;
                        current_pokemon_evs_and_losses_beaten.emplace_back(
                            std::move(ev_max), nullptr
                        );
                    }
                    if (!won.second) {
                        EVSet ev_max_nature = ev_set;
                        ev_max_nature.special_defense = evs_left;
                        ev_max_nature.nature_stat =
                            Stat::SPECIAL_DEFENSE;
                        current_pokemon_evs_and_losses_beaten.emplace_back(
                            std::move(ev_max_nature), nullptr
                        );
                    }
                    break;
                case Stat::SPEED:
                    won = get_speed_evs(
                        players_for_updated_player,
                        opponents,
                        ev_sets_to_beaten,
                        battle_result,
                        evs_left,
                        nature_used,
                        ev_set
                    );
                    if (!won.first) {
                        EVSet ev_max = ev_set;
                        ev_max.speed = evs_left;
                        current_pokemon_evs_and_losses_beaten.emplace_back(
                            std::move(ev_max), nullptr
                        );
                    }
                    if (!won.second) {
                        EVSet ev_max_nature = ev_set;
                        ev_max_nature.speed = evs_left;
                        ev_max_nature.nature_stat = Stat::SPEED;
                        current_pokemon_evs_and_losses_beaten.emplace_back(
                            std::move(ev_max_nature), nullptr
                        );
                    }
                    break;
                case Stat::NO_STAT:
                    throw std::runtime_error(
                        "No stat is an invalid stat"
                    );
                }
            }
        }
        current_index++;
        player.stats = original_player->stats;
    }
    ev_sets_promise.set_value(ev_sets_to_beaten);
}

inline void get_stat_spreads(
    const std::vector<CustomPokemon*>& players,
    const std::vector<CustomPokemon*>& opponents,
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<
            std::pair<
                EVSet,
                const size_t>
        >,
        PokemonPairHash
    >& pokemon_and_ability_to_evs_and_losses_beaten,
    const std::pair<Pokemon, Ability>& pokemon,
    std::vector<BattleResultEntry>& battle_results
) {
    if (!pokemon_and_ability_to_evs_and_losses_beaten.contains(pokemon)) {
        pokemon_and_ability_to_evs_and_losses_beaten.try_emplace(pokemon);
    }

    std::vector<EvOptimizerInput> ev_optimizer_inputs{};
    StatPermutationState state = create_initial_stat_permutation_state();
    do {
        for (auto& battle_result : battle_results) {
            ev_optimizer_inputs.push_back(
                {
                    .players = players,
                    .opponents = opponents,
                    .battle_result = battle_result,
                    .state = state
                }
            );
        }
    }
    while (next_stat_permutation(state));

    auto ev_sets =
        thread_pool::ThreadPool::getCPUWorkInstance()->
        runTasksAndFlatten<
            std::pair<
                EVSet,
                const size_t>,
            std::vector<EvOptimizerInput>,
            EvOptimizerInput&>(
            optimize_evs_for_battle,
            ev_optimizer_inputs
        );

    pokemon_and_ability_to_evs_and_losses_beaten
        .at(pokemon)
        .append_range(ev_sets);
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

    std::vector<CustomPokemon*> players{};
    std::unordered_map<CustomPokemon*, uint16_t> player_indices;
    for (auto& player_pokemon_forms :
         pokemon_to_forms | std::views::values
    ) {
        for (auto& player_pokemon : player_pokemon_forms) {
            player_indices[&player_pokemon] = players.size();
            players.emplace_back(&player_pokemon);
        }
    }

    std::vector<CustomPokemon*> opponents{};
    std::unordered_map<CustomPokemon*, uint16_t> opponent_indices;
    for (auto& opponent_pokemon_forms :
         pokemon_to_forms | std::views::values
    ) {
        for (auto& opponent_pokemon : opponent_pokemon_forms) {
            opponent_indices[&opponent_pokemon] = opponents.size();
            opponents.emplace_back(&opponent_pokemon);
        }
    }

    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleEntry>
        >
    > pokemon_to_battles{};
    get_battle_entries(players, opponents, pokemon_to_battles);

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
    std::vector<CustomPokemon*> better_players{};
    std::unordered_map<CustomPokemon*, uint16_t> better_player_indices;
    for (auto& player_pokemon_forms :
         player_pokemon_to_forms | std::views::values
    ) {
        for (auto& player_pokemon : player_pokemon_forms) {
            better_player_indices[&player_pokemon] = better_players.size();
            better_players.emplace_back(&player_pokemon);
        }
    }

    std::vector<
        std::pair<
            std::pair<Pokemon, Ability>,
            std::vector<BattleEntry>
        >
    > best_move_pokemon_to_battles{};
    get_battle_entries(
        better_players,
        opponents,
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

    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<
            std::pair<
                EVSet,
                const size_t>
        >,
        PokemonPairHash
    > pokemon_and_ability_to_evs_and_losses_beaten{};
    for (const auto& battle_results :
         best_moves_pokemon_to_battle_result_entries | std::views::values
    ) {
        std::unordered_map<
            std::pair<Pokemon, Ability>,
            std::vector<BattleResultEntry>,
            PokemonPairHash
        > pokemon_to_battle_losses{};
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
            pokemon_to_battle_losses[
                std::make_pair(
                    original_player->name,
                    original_player->ability
                )
            ].emplace_back(battle_result);
        }
        for (auto& [
                 pair,
                 losses
             ] : pokemon_to_battle_losses
        ) {
            get_stat_spreads(
                better_players,
                opponents,
                pokemon_and_ability_to_evs_and_losses_beaten,
                pair,
                losses
            );
        }
        // (0.6 * nature) increase per 4 EVs
        // (stat * 1.1) for ability
    }

    constexpr size_t BITSET_SIZE =
        static_cast<size_t>(Pokemon::Count) *
        static_cast<size_t>(Ability::Disabled);
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<
            std::pair<
                EVSet,
                std::bitset<BITSET_SIZE>
            >
        >,
        PokemonPairHash
    > pokemon_and_ability_to_evs_to_all_losses_beaten{};
    for (const auto& [
             p,
             ev_sets
         ] : pokemon_and_ability_to_evs_and_losses_beaten) {
        std::cout << ev_sets.size() << std::endl;
        for (const auto& [
                 ev_set,
                 beaten_opponent
             ] : ev_sets
        ) {
            std::bitset<BITSET_SIZE> beaten_opponents{};
            beaten_opponents.set(beaten_opponent);
            for (const auto& [
                     ev_set2,
                     beaten_opponent2
                 ] : ev_sets
            ) {
                if (ev_set2.is_less_than(ev_set)) {
                    beaten_opponents.set(beaten_opponent2);
                }
            }
            pokemon_and_ability_to_evs_to_all_losses_beaten[p].emplace_back(
                ev_set,
                beaten_opponents
            );
        }
    }


    printf("");
}

#endif //GEN_IV_BATTLE_HALL_ANALYZER_ROUND_ROBIN_H
