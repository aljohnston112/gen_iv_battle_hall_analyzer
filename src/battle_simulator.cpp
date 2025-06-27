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


struct PokemonTypeSetHash {
    std::size_t operator()(
        const std::unordered_set<PokemonType>& types
    ) const noexcept {
        std::vector<std::size_t> hashes;
        for (const PokemonType type : types) {
            hashes.push_back(std::hash<PokemonType>{}(type));
        }
        std::sort(hashes.begin(), hashes.end());
        std::size_t hash = 0;
        for (const auto h : hashes) {
            hash ^= h + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

struct PokemonTypeSetEqual {
    bool operator()(
        const std::unordered_set<PokemonType>& a,
        const std::unordered_set<PokemonType>& b
    ) const noexcept {
        return a == b;
    }
};

double get_effectiveness(
    const std::unordered_set<PokemonType>& defender_types,
    const PokemonType& attack_type
) {
    static std::unordered_map<
        std::unordered_set<PokemonType>,
        std::unordered_map<PokemonType, double>,
        PokemonTypeSetHash,
        PokemonTypeSetEqual
    > cache;
    if (const auto it = cache.find(defender_types);
        it != cache.end()
    ) {
        if (const auto it_2 = it->second.find(attack_type);
            it_2 != it->second.end()
        ) {
            return it_2->second;
        }
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
    cache[defender_types][attack_type] = multiplier;
    return multiplier;
}


struct PokemonState {
    const CustomPokemon& pokemon;
    const uint8_t level;
    uint16_t health;
    uint16_t attack;
    uint16_t defense;
    uint16_t special_attack;
    uint16_t special_defense;
    uint16_t speed;
};

uint get_damage(
    const uint8_t attacker_level,
    const uint16_t attack,
    const uint16_t defense,
    const MoveInfo* move
) {
    uint damage = ((2 * attacker_level) / 5) + 2;
    damage = damage * move->power * attack / defense;
    damage = damage / 50;
    return damage;
}

const MoveInfo* get_best_move_against_defender(
    const std::vector<const MoveInfo*>& attacker_moves,
    const PokemonState& attacker_state,
    const PokemonState& defender_state
) {
    const MoveInfo* best_move = nullptr;
    uint best_damage = 0;

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
                    special_attack,
                    special_defense,
                    move
                )
                : get_damage(
                    attacker_level,
                    attack,
                    defense,
                    move
                );
        if (damage > best_damage) {
            best_damage = damage;
            best_move = move;
        }
    }
    return best_move;
}

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
    const auto player_state = PokemonState{
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
    const auto opponent_state = PokemonState{
        .pokemon = opponent,
        .level = opponent.level,
        .health = opponent_health,
        .attack = opponent_attack,
        .defense = opponent_defense,
        .special_attack = opponent_special_attack,
        .special_defense = opponent_special_defense,
        .speed = opponent_speed
    };

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
}
