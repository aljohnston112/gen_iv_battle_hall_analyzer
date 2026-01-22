#ifndef GEN_IV_BATTLE_HALL_ANALYZER_EVOPTIMIZER_H
#define GEN_IV_BATTLE_HALL_ANALYZER_EVOPTIMIZER_H

#include <future>
#include <debug/vector>

#include "EVSet.h"
#include "BattleState.h"
#include "custom_pokemon.h"
#include "thread_pool.h"

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
            std::ceil(
                ((current_stat + diff) / 1.1) - current_stat
            ) / (LEVEL / 100.0)
        ) * 4;
    }
}


inline std::pair<bool, bool> get_attack_evs(
    const std::vector<CustomPokemon*>& players,
    const std::vector<CustomPokemon*>& opponents,
    std::unordered_map<
        EVSet,
        std::vector<size_t>,
        EVSetHash
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
            original_player_index,
            &current_player,
            opponent_index,
            opponent
        };
        if (const auto new_battle_results =
                new_battle_state.battle_loop();
            new_battle_results.won
        ) {
            won.first = true;
            evs[
                EVSet{
                    .health = evs_used.health,
                    .attack = 252,
                    .defense = evs_used.defense,
                    .special_attack = evs_used.special_attack,
                    .special_defense = evs_used.special_defense,
                    .speed = evs_used.speed,
                    .nature_stat = evs_used.nature_stat
                }
            ].emplace_back(opponent_index);
        }
        if (!nature_used) {
            current_player.stats[
                static_cast<int>(Stat::ATTACK)
            ] = original_player_attack + (252 / 4) * (LEVEL / 100.0) * 1.1;
            BattleState new_battle_state{
                original_player_index,
                &current_player,
                opponent_index,
                opponent
            };
            if (const auto new_battle_results =
                    new_battle_state.battle_loop();
                new_battle_results.won
            ) {
                won.second = true;
                evs[
                    EVSet{
                        .health = evs_used.health,
                        .attack = 252,
                        .defense = evs_used.defense,
                        .special_attack = evs_used.special_attack,
                        .special_defense = evs_used.special_defense,
                        .speed = evs_used.speed,
                        .nature_stat = Stat::ATTACK
                    }
                ].emplace_back(opponent_index);
            }
        }
    }
    return won;
}

inline std::pair<bool, bool> get_defense_evs(
    const std::vector<CustomPokemon*>& players,
    const std::vector<CustomPokemon*>& opponents,
    std::unordered_map<
        EVSet,
        std::vector<size_t>,
        EVSetHash
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
            original_player_index,
            &current_player,
            opponent_index,
            opponent
        };
        if (const auto new_battle_results =
                new_battle_state.battle_loop();
            new_battle_results.won
        ) {
            won.first = true;
            evs[
                EVSet{
                    .health = evs_used.health,
                    .attack = evs_used.attack,
                    .defense = 252,
                    .special_attack = evs_used.special_attack,
                    .special_defense = evs_used.special_defense,
                    .speed = evs_used.speed,
                    .nature_stat = evs_used.nature_stat
                }
            ].emplace_back(opponent_index);
        }
        if (!nature_used) {
            current_player.stats[
                static_cast<int>(Stat::DEFENSE)
            ] = original_player_attack + (252 / 4) * (LEVEL / 100.0) * 1.1;
            BattleState new_battle_state{
                original_player_index,
                &current_player,
                opponent_index,
                opponent
            };
            if (const auto new_battle_results =
                    new_battle_state.battle_loop();
                new_battle_results.won
            ) {
                won.second = true;
                evs[
                    EVSet{
                        .health = evs_used.health,
                        .attack = evs_used.attack,
                        .defense = 252,
                        .special_attack = evs_used.special_attack,
                        .special_defense = evs_used.special_defense,
                        .speed = evs_used.speed,
                        .nature_stat = Stat::DEFENSE
                    }
                ].emplace_back(opponent_index);
            }
        }
    }
    return won;
}

inline std::pair<bool, bool> get_special_attack_evs(
    const std::vector<CustomPokemon*>& players,
    const std::vector<CustomPokemon*>& opponents,
    std::unordered_map<
        EVSet,
        std::vector<size_t>,
        EVSetHash
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
            original_player_index,
            &current_player,
            opponent_index,
            opponent
        };
        if (const auto new_battle_results =
                new_battle_state.battle_loop();
            new_battle_results.won
        ) {
            won.first = true;
            evs[
                EVSet{
                    .health = evs_used.health,
                    .attack = evs_used.attack,
                    .defense = evs_used.defense,
                    .special_attack = 252,
                    .special_defense = evs_used.special_defense,
                    .speed = evs_used.speed,
                    .nature_stat = evs_used.nature_stat
                }].emplace_back(opponent_index);
        }
        if (!nature_used) {
            current_player.stats[
                static_cast<int>(Stat::SPECIAL_ATTACK)
            ] = original_player_attack + (252 / 4) * (LEVEL / 100.0) * 1.1;
            BattleState new_battle_state{
                original_player_index,
                &current_player,
                opponent_index,
                opponent
            };
            if (const auto new_battle_results =
                    new_battle_state.battle_loop();
                new_battle_results.won
            ) {
                won.second = true;
                evs[
                    EVSet{
                        .health = evs_used.health,
                        .attack = evs_used.attack,
                        .defense = evs_used.defense,
                        .special_attack = 252,
                        .special_defense = evs_used.special_defense,
                        .speed = evs_used.speed,
                        .nature_stat = Stat::SPECIAL_ATTACK
                    }
                ].emplace_back(opponent_index);
            }
        }
    }
    return won;
}

inline std::pair<bool, bool> get_special_defense_evs(
    const std::vector<CustomPokemon*>& players,
    const std::vector<CustomPokemon*>& opponents,
    std::unordered_map<
        EVSet,
        std::vector<size_t>,
        EVSetHash
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
            original_player_index,
            &current_player,
            opponent_index,
            opponent
        };
        if (const auto new_battle_results =
                new_battle_state.battle_loop();
            new_battle_results.won
        ) {
            won.first = true;
            evs[
                EVSet{
                    .health = evs_used.health,
                    .attack = evs_used.attack,
                    .defense = evs_used.defense,
                    .special_attack = evs_used.special_attack,
                    .special_defense = 252,
                    .speed = evs_used.speed,
                    .nature_stat = evs_used.nature_stat
                }].emplace_back(opponent_index);
        }
        if (!nature_used) {
            current_player.stats[
                static_cast<int>(Stat::SPECIAL_DEFENSE)
            ] = (original_player_attack + ((252 / 4) * (LEVEL / 100.0))) * 1.1;
            BattleState new_battle_state{
                original_player_index,
                &current_player,
                opponent_index,
                opponent
            };
            if (const auto new_battle_results =
                    new_battle_state.battle_loop();
                new_battle_results.won
            ) {
                won.second = true;
                evs[
                    EVSet{
                        .health = evs_used.health,
                        .attack = evs_used.attack,
                        .defense = evs_used.defense,
                        .special_attack = evs_used.special_attack,
                        .special_defense = 252,
                        .speed = evs_used.speed,
                        .nature_stat = Stat::SPECIAL_DEFENSE
                    }
                ].emplace_back(opponent_index);
            }
        }
    }
    return won;
}

inline std::pair<bool, bool> get_speed_evs(
    const std::vector<CustomPokemon*>& players,
    const std::vector<CustomPokemon*>& opponents,
    std::unordered_map<
        EVSet,
        std::vector<size_t>,
        EVSetHash
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
                original_player_index,
                &current_player,
                opponent_index,
                opponent
            };
            if (const auto new_battle_results =
                    new_battle_state.battle_loop();
                new_battle_results.won
            ) {
                won.first = true;
                evs[
                    EVSet{
                        .health = evs_used.health,
                        .attack = evs_used.attack,
                        .defense = evs_used.defense,
                        .special_attack = evs_used.special_attack,
                        .special_defense = evs_used.special_defense,
                        .speed = speed_evs,
                        .nature_stat = evs_used.nature_stat
                    }
                ].emplace_back(opponent_index);
            }
        }
        if (!nature_used && speed_nature_evs <= evs_left) {
            current_player.stats[
                    static_cast<int>(Stat::SPEED)
                ] = original_player_speed +
                ((speed_nature_evs / 4) * (LEVEL / 100.0)) * 1.1;
            BattleState new_battle_state{
                original_player_index,
                &current_player,
                opponent_index,
                opponent
            };
            if (const auto new_battle_results =
                    new_battle_state.battle_loop();
                new_battle_results.won
            ) {
                won.second = true;
                evs[
                    EVSet{
                        .health = evs_used.health,
                        .attack = evs_used.attack,
                        .defense = evs_used.defense,
                        .special_attack = evs_used.special_attack,
                        .special_defense = evs_used.special_defense,
                        .speed = speed_nature_evs,
                        .nature_stat = Stat::SPEED
                    }
                ].emplace_back(opponent_index);
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
    std::unordered_map<
        EVSet,
        std::vector<size_t>,
        EVSetHash
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
            original_player_index,
            &current_player,
            opponent_index,
            opponent
        };
        // if (original_player->name == Pokemon::Illumise &&
        //     original_player->ability == Ability::TintedLens &&
        //     opponent->name == Pokemon::Venonat &&
        //     opponent->ability == Ability::Compoundeyes
        // ) {
        //     volatile int a;
        // }
        const auto new_battle_results =
            new_battle_state.battle_loop();
        if (new_battle_results.won) {
            won.first = true;
            evs[
                EVSet{
                    .health = hp_evs,
                    .attack = evs_used.attack,
                    .defense = evs_used.defense,
                    .special_attack = evs_used.special_attack,
                    .special_defense = evs_used.special_defense,
                    .speed = evs_used.speed,
                    .nature_stat = evs_used.nature_stat
                }
            ].emplace_back(opponent_index);

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

inline void do_battles_with_updated_evs(
    const std::vector<CustomPokemon*>& opponents,
    const std::pair<Pokemon, Ability>& pokemon,
    const size_t player_index,
    const CustomPokemon* player_pokemon,
    const EVSet& evs,
    const std::vector<size_t>& losses_to_beat,
    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<
            std::pair<
                EVSet,
                std::unordered_set<size_t>
            >
        >,
        PokemonPairHash
    >& pokemon_and_ability_to_evs_and_losses_beaten
) {
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
        pokemon_and_ability_to_evs_and_losses_beaten
            [pokemon].emplace_back(
                std::pair{
                    evs,
                    losses_beaten
                }
            );
    }
}

struct EvOptimizerInput {
    const std::vector<CustomPokemon*>& players;
    const std::vector<CustomPokemon*>& opponents;
    BattleResultEntry& battle_result;
    const StatPermutationState& state;
};

inline void set_up_stats_based_on_ev_set(
    const EVSet& ev_set,
    const std::array<
        uint16_t,
        static_cast<int>(Stat::NO_STAT)
    >& original_player_stats,
    CustomPokemon& player
) {
    assert(ev_set.health >= 0 && ev_set.health < 253);
    assert(ev_set.attack >= 0 && ev_set.attack < 253);
    assert(ev_set.defense >= 0 && ev_set.defense < 253);
    assert(ev_set.special_attack >= 0 && ev_set.special_attack < 253);
    assert(ev_set.special_defense >= 0 && ev_set.special_defense < 253);
    assert(ev_set.speed >= 0 && ev_set.speed < 253);

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
            ] + (ev_set.attack / 4) * (LEVEL / 100.0)) * 1.1;
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
            ] + (ev_set.defense / 4) * (LEVEL / 100.0)) * 1.1;
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
            ] + (ev_set.special_attack / 4) * (LEVEL / 100.0)) * 1.1;
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
            ] + (ev_set.special_defense / 4) * (LEVEL / 100.0)) * 1.1;
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
}

inline void optimize_evs_for_battle(
    EvOptimizerInput& input,
    std::promise<
        std::unordered_map<
            EVSet,
            std::vector<size_t>,
            EVSetHash
        >
    >&& ev_sets_promise
) {
    std::unordered_map<
        EVSet,
        std::vector<size_t>,
        EVSetHash
    > ev_sets_to_beaten{};

    const auto& players = input.players;
    const auto& opponents = input.opponents;
    BattleResultEntry& battle_result = input.battle_result;
    const StatPermutationState& state = input.state;
    size_t player_index = battle_result.player_index;
    auto player = *players[player_index];
    auto players_for_updated_player = std::vector<CustomPokemon*>{
        players.size()
    };
    players_for_updated_player.reserve(players.size());
    players_for_updated_player.insert(
        players_for_updated_player.begin() + player_index,
        &player
    );

    auto original_player = players[player_index];
    const auto original_player_stats =
        original_player->stats;

    std::unordered_map<
        EVSet,
        std::vector<size_t>,
        EVSetHash
    > current_pokemon_evs_and_losses_beaten{};
    current_pokemon_evs_and_losses_beaten[
        EVSet{
            .health = 0,
            .attack = 0,
            .defense = 0,
            .special_attack = 0,
            .special_defense = 0,
            .speed = 0,
            .nature_stat = Stat::NO_STAT
        }
    ].clear();
    int current_stat_index = 0;
    std::pair won = {false, false};
    while (current_stat_index < static_cast<int>(Stat::NO_STAT) &&
        (!won.first || !won.second)
    ) {
        std::unordered_map<
            EVSet,
            std::vector<size_t>,
            EVSetHash
        > temp = std::move(current_pokemon_evs_and_losses_beaten);
        current_pokemon_evs_and_losses_beaten.clear();
        current_pokemon_evs_and_losses_beaten.reserve(12 * temp.size());
        for (const auto& [
                 ev_set,
                 opponents_beaten
             ] : temp
        ) {
            const uint16_t evs_left = std::min(
                508 - get_number_of_evs(ev_set),
                252
            );
            set_up_stats_based_on_ev_set(
                ev_set,
                original_player_stats,
                player
            );

            const Stat nature_stat = ev_set.nature_stat;
            bool nature_used = nature_stat != Stat::NO_STAT;
            if (evs_left > 0 &&
                player.name != Pokemon::Shedinja
            ) {
                switch (state[current_stat_index]) {
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
                        current_pokemon_evs_and_losses_beaten[
                            std::move(ev_max)
                        ].clear();
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
                        current_pokemon_evs_and_losses_beaten[
                            std::move(ev_max)
                        ].clear();
                    }
                    if (!won.second) {
                        EVSet ev_max_nature = ev_set;
                        ev_max_nature.attack = evs_left;
                        ev_max_nature.nature_stat = Stat::ATTACK;
                        current_pokemon_evs_and_losses_beaten[
                            std::move(ev_max_nature)
                        ].clear();
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
                        current_pokemon_evs_and_losses_beaten[
                            std::move(ev_max)
                        ].clear();
                    }
                    if (!won.second) {
                        EVSet ev_max_nature = ev_set;
                        ev_max_nature.defense = evs_left;
                        ev_max_nature.nature_stat = Stat::DEFENSE;
                        current_pokemon_evs_and_losses_beaten[
                            std::move(ev_max_nature)
                        ].clear();
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
                        current_pokemon_evs_and_losses_beaten[
                            std::move(ev_max)
                        ].clear();
                    }
                    if (!won.second) {
                        EVSet ev_max_nature = ev_set;
                        ev_max_nature.special_attack = evs_left;
                        ev_max_nature.nature_stat =
                            Stat::SPECIAL_ATTACK;
                        current_pokemon_evs_and_losses_beaten[
                            std::move(ev_max_nature)
                        ].clear();
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
                        current_pokemon_evs_and_losses_beaten[
                            std::move(ev_max)
                        ].clear();
                    }
                    if (!won.second) {
                        EVSet ev_max_nature = ev_set;
                        ev_max_nature.special_defense = evs_left;
                        ev_max_nature.nature_stat =
                            Stat::SPECIAL_DEFENSE;
                        current_pokemon_evs_and_losses_beaten[
                            std::move(ev_max_nature)
                        ].clear();
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
                        current_pokemon_evs_and_losses_beaten[
                            std::move(ev_max)
                        ].clear();
                    }
                    if (!won.second) {
                        EVSet ev_max_nature = ev_set;
                        ev_max_nature.speed = evs_left;
                        ev_max_nature.nature_stat = Stat::SPEED;
                        current_pokemon_evs_and_losses_beaten[
                            std::move(ev_max_nature)
                        ].clear();
                    }
                    break;
                case Stat::NO_STAT:
                    throw std::runtime_error(
                        "No stat is an invalid stat"
                    );
                }
            }
        }
        current_stat_index++;
        player.stats = original_player->stats;
    }
    ev_sets_promise.set_value(ev_sets_to_beaten);
}

inline std::unordered_map<EVSet, std::vector<size_t>, EVSetHash>
get_ev_sets(
    const std::vector<CustomPokemon*>& players,
    const std::vector<CustomPokemon*>& opponents,
    std::vector<BattleResultEntry>& losses
) {
    std::vector<EvOptimizerInput> ev_optimizer_inputs{};
    StatPermutationState state = create_initial_stat_permutation_state();
    do {
        for (auto& battle_result : losses) {
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
        runTasksAndFlattenMaps<
            std::unordered_map<
                EVSet,
                std::vector<size_t>,
                EVSetHash
            >,
            std::vector<EvOptimizerInput>,
            EvOptimizerInput&>(
            optimize_evs_for_battle,
            ev_optimizer_inputs
        );

    std::unordered_map<
        std::pair<Pokemon, Ability>,
        std::vector<
            std::pair<
                EVSet,
                std::unordered_set<size_t>
            >
        >,
        PokemonPairHash
    > pokemon_and_ability_to_evs_and_losses_beaten{};

    std::vector<size_t> loss_indices{};
    for (const auto& [
             pi,
             oi,
             w,
             pm,
             om
         ] : losses
    ) {
        loss_indices.emplace_back(oi);
    }
    size_t player_index = losses[0].player_index;
    auto player = *players[player_index];
    auto players_for_updated_player = std::vector<CustomPokemon*>{
        players.size()
    };
    players_for_updated_player.reserve(players.size());
    players_for_updated_player.insert(
        players_for_updated_player.begin() + player_index,
        &player
    );

    auto original_player = players[player_index];
    const auto original_player_stats =
        original_player->stats;
    for (const auto& [
             ev_set,
             _
         ] : ev_sets
    ) {
        set_up_stats_based_on_ev_set(
            ev_set,
            original_player_stats,
            player
        );
        do_battles_with_updated_evs(
            opponents,
            {player.name, player.ability},
            player_index,
            &player,
            ev_set,
            loss_indices,
            pokemon_and_ability_to_evs_and_losses_beaten
        );
    }
    return ev_sets;
}

#endif //GEN_IV_BATTLE_HALL_ANALYZER_EVOPTIMIZER_H
