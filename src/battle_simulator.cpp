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


const std::unordered_set IGNORED_MOVES = {
    Move::Snore,
    Move::SolarBeam,
};

std::unordered_map<
    Category,
    std::array<MoveInfo*, static_cast<int>(PokemonType::COUNT)>
> gather_moves(
    const CustomPokemon& pokemon,
    const uint16_t defender_health,
    const bool is_player
) {
    // TODO handle solar beam with or without power herb
    //      No use in using if can ko
    std::unordered_map<
        Category,
        std::array<MoveInfo*, static_cast<int>(PokemonType::COUNT)>
    > moves;
    const uint8_t min_accuracy = is_player ? 100 : 0;
    for (auto move : pokemon.moves) {
        if (move.power == 0 ||
            move.accuracy < min_accuracy ||
            IGNORED_MOVES.contains(move.move)
        ) {
            continue;
        }

        // if (move.power == -1) {
        //     printf(move.name.c_str());
        //     printf("\n");
        //     exit(1);
        //     continue;
        // }
        auto& type_map = moves[move.category];
        if (const auto current_move = type_map[static_cast<int>(move.type)];
            current_move != nullptr && move.power > current_move->power
        ) {
            type_map[static_cast<int>(move.type)] = &move;
        }
    }
    return moves;
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
    uint damage = std::floor((2 * attacker_level) / 5) + 2;
    damage = std::floor(damage * move->power * attack / defense);
    damage = std::floor(damage / 50);
    return damage;
}

MoveInfo* get_best_move_against_defender(
    std::unordered_map<
        Category,
        std::array<MoveInfo*, static_cast<int>(PokemonType::COUNT)>
    > attacker_moves,
    const PokemonState& attacker_state,
    const PokemonState& defender_state
) {
    MoveInfo* best_move = nullptr;
    uint best_damage = 0;

    const uint8_t attacker_level = attacker_state.level;
    auto attack = attacker_state.attack;
    auto defense = defender_state.defense;
    if (attacker_moves.contains(Category::PHYSICAL)) {
        for (const auto& move_array = attacker_moves.at(Category::PHYSICAL);
             const auto move : move_array
        ) {
            if (move == nullptr) {
                continue;
            }
            if (const auto damage =
                    get_damage(attacker_level, attack, defense, move);
                damage > best_damage
            ) {
                best_damage = damage;
                best_move = move;
            }
        }
    }
    attack = attacker_state.special_attack;
    defense = defender_state.special_defense;
    if (attacker_moves.contains(Category::SPECIAL)) {
        for (const auto& move_array = attacker_moves.at(Category::SPECIAL);
             const auto move : move_array
        ) {
            if (move == nullptr) {
                continue;
            }
            if (const auto damage =
                    get_damage(attacker_level, attack, defense, move);
                damage > best_damage
            ) {
                best_damage = damage;
                best_move = move;
            }
        }
    }
    return best_move;
}

void battle(const CustomPokemon& player, const CustomPokemon& opponent) {
    const auto player_stats = player.stats;
    const auto player_hp = player_stats.at(Stat::HEALTH);
    const auto player_attack = player_stats.at(Stat::ATTACK);
    const auto player_defense = player_stats.at(Stat::DEFENSE);
    const auto player_special_attack =
        player_stats.at(Stat::SPECIAL_ATTACK);
    const auto player_special_defense =
        player_stats.at(Stat::SPECIAL_DEFENSE);
    const auto player_speed = player_stats.at(Stat::SPEED);
    const auto player_state = PokemonState{
        .pokemon = player,
        .level = player.level,
        .health = player_hp,
        .attack = player_attack,
        .defense = player_defense,
        .special_attack = player_special_attack,
        .special_defense = player_special_defense,
        .speed = player_speed
    };

    const auto opponent_stats = opponent.stats;
    const auto opponent_hp = opponent_stats.at(Stat::HEALTH);
    const auto opponent_attack = opponent_stats.at(Stat::ATTACK);
    const auto opponent_defense = opponent_stats.at(Stat::DEFENSE);
    const auto opponent_special_attack =
        opponent_stats.at(Stat::SPECIAL_ATTACK);
    const auto opponent_special_defense =
        opponent_stats.at(Stat::SPECIAL_DEFENSE);
    const auto opponent_speed = opponent_stats.at(Stat::SPEED);
    const auto opponent_state = PokemonState{
        .pokemon = opponent,
        .level = opponent.level,
        .health = opponent_hp,
        .attack = opponent_attack,
        .defense = opponent_defense,
        .special_attack = opponent_special_attack,
        .special_defense = opponent_special_defense,
        .speed = opponent_speed
    };

    const auto player_moves = gather_moves(
        player,
        opponent_state.health,
        true
    );
    const auto player_move = get_best_move_against_defender(
        player_moves,
        player_state,
        opponent_state
    );

    const auto opponent_moves = gather_moves(
        opponent,
        player_state.health,
        false
    );
    const auto opponent_move = get_best_move_against_defender(
        opponent_moves,
        opponent_state,
        player_state
    );
}
