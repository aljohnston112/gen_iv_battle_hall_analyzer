#include "battle_simulator.h"

#include <cmath>

#include "custom_pokemon.h"
#include  "serebii_pokemon_data_source.h"

constexpr double not_effective = 0.0;
constexpr double not_very_effective = 0.5;
constexpr double super_effective = 2.0;

const std::unordered_map<
    PokemonType,
    std::unordered_map<PokemonType, double>
> defender_type_map = {
    {
        PokemonType::NORMAL,
        {
            {PokemonType::FIGHTING, super_effective},
            {PokemonType::GHOST, not_effective},
        }
    },
    {
        PokemonType::FIRE,
        {
            {PokemonType::FIRE, not_very_effective},
            {PokemonType::WATER, super_effective},
            {PokemonType::GRASS, not_very_effective},
            {PokemonType::ICE, not_very_effective},
            {PokemonType::GROUND, super_effective},
            {PokemonType::BUG, not_very_effective},
            {PokemonType::ROCK, super_effective},
            {PokemonType::STEEL, not_very_effective},
        }
    },
    {
        PokemonType::WATER,
        {
            {PokemonType::FIRE, not_very_effective},
            {PokemonType::WATER, not_very_effective},
            {PokemonType::ELECTRIC, super_effective},
            {PokemonType::GRASS, super_effective},
            {PokemonType::ICE, not_very_effective},
            {PokemonType::STEEL, not_very_effective},
        }
    },
    {
        PokemonType::ELECTRIC,
        {
            {PokemonType::ELECTRIC, not_very_effective},
            {PokemonType::GROUND, super_effective},
            {PokemonType::FLYING, not_very_effective},
            {PokemonType::STEEL, not_very_effective},
        }
    },
    {
        PokemonType::GRASS,
        {
            {PokemonType::FIRE, super_effective},
            {PokemonType::WATER, not_very_effective},
            {PokemonType::ELECTRIC, not_very_effective},
            {PokemonType::GRASS, not_very_effective},
            {PokemonType::ICE, super_effective},
            {PokemonType::POISON, super_effective},
            {PokemonType::GROUND, not_very_effective},
            {PokemonType::FLYING, super_effective},
            {PokemonType::BUG, super_effective},
        }
    },
    {
        PokemonType::ICE,
        {
            {PokemonType::FIRE, super_effective},
            {PokemonType::ICE, not_very_effective},
            {PokemonType::FIGHTING, super_effective},
            {PokemonType::ROCK, super_effective},
            {PokemonType::STEEL, super_effective},
        }
    },
    {
        PokemonType::FIGHTING,
        {
            {PokemonType::FLYING, super_effective},
            {PokemonType::PSYCHIC, super_effective},
            {PokemonType::BUG, not_very_effective},
            {PokemonType::ROCK, not_very_effective},
            {PokemonType::DARK, not_very_effective},
        }
    },
    {
        PokemonType::POISON,
        {
            {PokemonType::GRASS, not_very_effective},
            {PokemonType::FIGHTING, not_very_effective},
            {PokemonType::POISON, not_very_effective},
            {PokemonType::GROUND, super_effective},
            {PokemonType::PSYCHIC, super_effective},
            {PokemonType::BUG, not_very_effective},
        }
    },
    {
        PokemonType::GROUND,
        {
            {PokemonType::WATER, super_effective},
            {PokemonType::ELECTRIC, not_effective},
            {PokemonType::GRASS, super_effective},
            {PokemonType::ICE, super_effective},
            {PokemonType::POISON, not_very_effective},
            {PokemonType::ROCK, not_very_effective},
        }
    },
    {
        PokemonType::FLYING,
        {
            {PokemonType::ELECTRIC, super_effective},
            {PokemonType::GRASS, not_very_effective},
            {PokemonType::ICE, super_effective},
            {PokemonType::FIGHTING, not_very_effective},
            {PokemonType::GROUND, not_effective},
            {PokemonType::BUG, not_very_effective},
            {PokemonType::ROCK, super_effective},
        }
    },
    {
        PokemonType::PSYCHIC,
        {
            {PokemonType::FIGHTING, not_very_effective},
            {PokemonType::PSYCHIC, not_very_effective},
            {PokemonType::BUG, super_effective},
            {PokemonType::GHOST, super_effective},
            {PokemonType::DARK, super_effective},
        }
    },
    {
        PokemonType::BUG,
        {
            {PokemonType::FIRE, super_effective},
            {PokemonType::GRASS, not_very_effective},
            {PokemonType::FIGHTING, not_very_effective},
            {PokemonType::GROUND, not_very_effective},
            {PokemonType::FLYING, super_effective},
            {PokemonType::ROCK, super_effective},
        }
    },
    {
        PokemonType::ROCK,
        {
            {PokemonType::NORMAL, not_very_effective},
            {PokemonType::FIRE, not_very_effective},
            {PokemonType::WATER, super_effective},
            {PokemonType::GRASS, super_effective},
            {PokemonType::FIGHTING, super_effective},
            {PokemonType::POISON, not_very_effective},
            {PokemonType::GROUND, super_effective},
            {PokemonType::FLYING, not_very_effective},
            {PokemonType::STEEL, super_effective},
        }
    },
    {
        PokemonType::GHOST,
        {
            {PokemonType::NORMAL, not_effective},
            {PokemonType::FIGHTING, not_effective},
            {PokemonType::POISON, not_very_effective},
            {PokemonType::BUG, not_very_effective},
            {PokemonType::GHOST, super_effective},
            {PokemonType::DARK, super_effective},
        }
    },
    {
        PokemonType::DRAGON,
        {
            {PokemonType::FIRE, not_very_effective},
            {PokemonType::WATER, not_very_effective},
            {PokemonType::ELECTRIC, not_very_effective},
            {PokemonType::GRASS, not_very_effective},
            {PokemonType::ICE, super_effective},
            {PokemonType::DRAGON, super_effective},
        }
    },
    {
        PokemonType::DARK,
        {
            {PokemonType::FIGHTING, super_effective},
            {PokemonType::PSYCHIC, not_effective},
            {PokemonType::BUG, super_effective},
            {PokemonType::GHOST, not_very_effective},
            {PokemonType::DARK, not_very_effective},
        }
    },
    {
        PokemonType::STEEL,
        {
            {PokemonType::NORMAL, not_very_effective},
            {PokemonType::FIRE, super_effective},
            {PokemonType::GRASS, not_very_effective},
            {PokemonType::ICE, not_very_effective},
            {PokemonType::FIGHTING, super_effective},
            {PokemonType::POISON, not_effective},
            {PokemonType::GROUND, super_effective},
            {PokemonType::FLYING, not_very_effective},
            {PokemonType::PSYCHIC, not_very_effective},
            {PokemonType::BUG, not_very_effective},
            {PokemonType::ROCK, not_very_effective},
            {PokemonType::GHOST, not_very_effective},
            {PokemonType::DRAGON, not_very_effective},
            {PokemonType::DARK, not_very_effective},
            {PokemonType::STEEL, not_very_effective},
        }
    },
};

double get_effectiveness(
    const std::vector<PokemonType>& defender_types,
    const PokemonType& attack_type
) {
    static constexpr int number_of_types = static_cast<int>(PokemonType::COUNT);
    auto key = number_of_types * static_cast<int>(defender_types[0]);
    if (defender_types.size() > 1) {
        key += static_cast<int>(defender_types[1]);
    }
    static constexpr int cache_size = number_of_types * number_of_types;
    static std::array<double, cache_size> cache = [] {
        std::array<double, cache_size> arr{};
        arr.fill(-1.0);
        return arr;
    }();
    if (const auto value = cache[key];
        value != -1.0
    ) {
        return value;
    }

    double multiplier = 1.0;
    for (const auto defender_type : defender_types) {
        if (const auto defender_it = defender_type_map.find(defender_type);
            defender_it != defender_type_map.end()
        ) {
            const auto& inner_map = defender_it->second;
            if (const auto attacker_it = inner_map.find(attack_type);
                attacker_it != inner_map.end()
            ) {
                multiplier *= attacker_it->second;
            }
        }
    }
    cache[key] = multiplier;
    return multiplier;
}


struct PokemonState {
    const CustomPokemon& pokemon;
    const uint8_t level;
    int health;
    uint16_t attack;
    uint16_t defense;
    uint16_t special_attack;
    uint16_t special_defense;
    uint16_t speed;
};

int get_damage(
    const uint8_t attacker_level,
    const std::vector<PokemonType>& attacker_types,
    const uint16_t attack,
    const std::vector<PokemonType>& defender_types,
    const uint16_t defense,
    const MoveInfo* move
) {
    int damage = ((2 * attacker_level) / 5) + 2;
    damage = damage * move->power * attack / defense;
    damage = damage / 50;

    damage = damage + 2;
    const auto move_type = move->type;
    if (attacker_types[0] == move_type ||
        (attacker_types.size() > 1 && attacker_types[1] == move_type)
    ) {
        damage = static_cast<int>(damage * 1.5);
    }
    damage = static_cast<int>(damage * get_effectiveness(
        defender_types, move_type));

    return damage;
}

struct BestMove {
    const MoveInfo* move = nullptr;
    int damage = 0;
};

BestMove get_best_move_against_defender(
    const std::vector<const MoveInfo*>& attacker_moves,
    const PokemonState& attacker_state,
    const PokemonState& defender_state
) {
    BestMove best_move{};
    const uint8_t attacker_level = attacker_state.level;
    const auto attack = attacker_state.attack;
    const auto defense = defender_state.defense;
    const auto special_attack = attacker_state.special_attack;
    const auto special_defense = defender_state.special_defense;
    for (const auto& move : attacker_moves) {
        const auto category = move->category;
        if (category == Category::STATUS) {
            continue;
        }
        const bool is_special = category == Category::SPECIAL;
        const auto damage =
            is_special
                ? get_damage(
                    attacker_level,
                    attacker_state.pokemon.types,
                    special_attack,
                    defender_state.pokemon.types,
                    special_defense,
                    move
                )
                : get_damage(
                    attacker_level,
                    attacker_state.pokemon.types,
                    attack,
                    defender_state.pokemon.types,
                    defense,
                    move
                );
        if (damage > best_move.damage) {
            best_move.damage = damage;
            best_move.move = move;
        }
    }
    return best_move;
}

void pre_turn_state_update(
    PokemonState& attacker_state,
    PokemonState& defender_state
) {}

bool is_player_first(
    const PokemonState& attacker_state,
    const BestMove attacker_move,
    const PokemonState& defender_state,
    const BestMove defender_move
) {
    return attacker_state.speed > defender_state.speed;
}

void apply_post_attack_effects(
    PokemonState& attacker_state,
    const BestMove& attacker_move,
    PokemonState& defender_state
) {}

void execute_move(
    PokemonState& attacker_state,
    const BestMove& attacker_move,
    PokemonState& defender_state
) {
    defender_state.health -= attacker_move.damage;
    apply_post_attack_effects(
        attacker_state,
        attacker_move,
        defender_state
    );
}

void apply_end_of_turn_effects(
    PokemonState& attacker_state,
    PokemonState& defender_state
) {}

void battle(const CustomPokemon& player, const CustomPokemon& opponent) {
    const auto& player_stats = player.stats;
    const auto player_health =
        player_stats[static_cast<int>(Stat::HEALTH)];
    const auto player_attack =
        player_stats[static_cast<int>(Stat::ATTACK)];
    const auto player_defense =
        player_stats[static_cast<int>(Stat::DEFENSE)];
    const auto player_special_attack =
        player_stats[static_cast<int>(Stat::SPECIAL_ATTACK)];
    const auto player_special_defense =
        player_stats[static_cast<int>(Stat::SPECIAL_DEFENSE)];
    const auto player_speed =
        player_stats[static_cast<int>(Stat::SPEED)];
    auto player_state = PokemonState{
        .pokemon = player,
        .level = player.level,
        .health = player_health,
        .attack = player_attack,
        .defense = player_defense,
        .special_attack = player_special_attack,
        .special_defense = player_special_defense,
        .speed = player_speed
    };

    const auto& opponent_stats = opponent.stats;
    const auto opponent_health =
        opponent_stats[static_cast<int>(Stat::HEALTH)];
    const auto opponent_attack =
        opponent_stats[static_cast<int>(Stat::ATTACK)];
    const auto opponent_defense =
        opponent_stats[static_cast<int>(Stat::DEFENSE)];
    const auto opponent_special_attack =
        opponent_stats[static_cast<int>(Stat::SPECIAL_ATTACK)];
    const auto opponent_special_defense =
        opponent_stats[static_cast<int>(Stat::SPECIAL_DEFENSE)];
    const auto opponent_speed =
        opponent_stats[static_cast<int>(Stat::SPEED)];
    auto opponent_state = PokemonState{
        .pokemon = opponent,
        .level = opponent.level,
        .health = opponent_health,
        .attack = opponent_attack,
        .defense = opponent_defense,
        .special_attack = opponent_special_attack,
        .special_defense = opponent_special_defense,
        .speed = opponent_speed
    };

    while (player_state.health > 0 && opponent_state.health > 0) {
        pre_turn_state_update(player_state, opponent_state);
        const auto player_move = get_best_move_against_defender(
            player.moves,
            player_state,
            opponent_state
        );
        const auto opponent_move = get_best_move_against_defender(
            opponent.moves,
            opponent_state,
            player_state
        );
        const bool player_goes_first = is_player_first(
            player_state,
            player_move,
            opponent_state,
            opponent_move
        );
        if (player_move.damage == 0 && opponent_move.damage == 0) {
            break;
        }

        if (player_goes_first) {
            execute_move(
                player_state,
                player_move,
                opponent_state
            );
            if (player_state.health <= 0 || opponent_state.health <= 0) {
                break;
            }
            execute_move(
                opponent_state,
                opponent_move,
                player_state
            );
            if (player_state.health <= 0 || opponent_state.health <= 0) {
                break;
            }
        } else {
            execute_move(
                opponent_state,
                opponent_move,
                player_state
            );
            if (player_state.health <= 0 || opponent_state.health <= 0) {
                break;
            }
            execute_move(
                player_state,
                player_move,
                opponent_state
            );
            if (player_state.health <= 0 || opponent_state.health <= 0) {
                break;
            }
        }
        apply_end_of_turn_effects(
            player_state,
            opponent_state
        );
    }
}
