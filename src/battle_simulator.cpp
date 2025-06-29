#include "battle_simulator.h"

#include <cmath>
#include <set>

#include "custom_pokemon.h"
#include  "serebii_pokemon_data_source.h"

/*
 All move sets:
    ATTACKER_STAT_BOOST_MOVES,
    MOVES_AFFECTED_BY_KINGS_ROCK,
    MOVES_THAT_BADLY_POISON,
    MOVES_THAT_BOOST_ATTACKERS_ATTACK,
    MOVES_THAT_BOOST_ATTACKERS_CRIT_RATE,
    MOVES_THAT_BOOST_ATTACKERS_DEFENSE,
    MOVES_THAT_BOOST_ATTACKERS_EVASION,
    MOVES_THAT_BOOST_ATTACKERS_SPECIAL_ATTACK,
    MOVES_THAT_BOOST_ATTACKERS_SPECIAL_DEFENSE,
    MOVES_THAT_BOOST_ATTACKERS_SPEED,
    MOVES_THAT_BREAK_PROTECT,
    MOVES_THAT_BURN,
    MOVES_THAT_BYPASS_PROTECT,
    MOVES_THAT_CAN_BE_REFLECTED_BY_MIRROR_COAT,
    MOVES_THAT_CAN_BE_SNATCHED,
    MOVES_THAT_CHANGE_WEATHER,
    MOVES_THAT_CONFUSE_DEFENDER,
    MOVES_THAT_CONTINUE,
    MOVES_THAT_DEAL_DOUBLE_AFTER_MINIMIZE,
    MOVES_THAT_FLINCH,
    MOVES_THAT_FORCE_USER_OUT,
    MOVES_THAT_FREEZE,
    MOVES_THAT_GO_INTO_AIR,
    MOVES_THAT_GO_UNDER_GROUND,
    MOVES_THAT_GO_UNDER_WATER,
    MOVES_THAT_HAVE_FIXED_DAMAGE,
    MOVES_THAT_HEAL_ATTACKER,
    MOVES_THAT_HIT_DEFENDER_IN_AIR,
    MOVES_THAT_HIT_DEFENDER_UNDER_GROUND,
    MOVES_THAT_HIT_DEFENDER_UNDER_WATER,
    MOVES_THAT_LOWER_ATTACKERS_ATTACK,
    MOVES_THAT_LOWER_ATTACKERS_DEFENSE,
    MOVES_THAT_LOWER_ATTACKERS_SPECIAL_ATTACK,
    MOVES_THAT_LOWER_ATTACKERS_SPECIAL_DEFENSE,
    MOVES_THAT_LOWER_ATTACKERS_SPEED,
    MOVES_THAT_LOWER_DEFENDER_ACCURACY,
    MOVES_THAT_LOWER_DEFENDER_ATTACK,
    MOVES_THAT_LOWER_DEFENDER_DEFENSE,
    MOVES_THAT_LOWER_DEFENDER_EVASION,
    MOVES_THAT_LOWER_DEFENDER_SPECIAL_ATTACK,
    MOVES_THAT_LOWER_DEFENDER_SPECIAL_DEFENSE,
    MOVES_THAT_LOWER_DEFENDER_SPEED,
    MOVES_THAT_MAKE_ATTACKER_FAINT,
    MOVES_THAT_MAKE_CONTACT,
    MOVES_THAT_MAKE_DEFENDER_SLEEP,
    MOVES_THAT_MAY_BYPASS_ACCURACY,
    MOVES_THAT_OMNI_BOOST_ATTACKER,
    MOVES_THAT_PARALYZE,
    MOVES_THAT_POISON,
    MOVES_THAT_RAISE_DEFENDER_ATTACK,
    MOVES_THAT_RAISE_DEFENDER_SPECIAL_ATTACK,
    MOVES_THAT_REQUIRE_CHARGING_TURN,
    MOVES_THAT_REQUIRE_RECHARGE_TURN,
    MOVES_THAT_VANISH_USER,
    MOVES_WITH_INCREASED_CRIT_CHANCE,
    MOVES_WITH_RECOIL,
    MULTI_HIT_MOVES,
    MULTI_TURN_MOVES,
    ONE_HIT_KO_MOVES,
    OTHER_MOVES,
    POWER_MOVES,
    PRIORITY_MOVES_MINUS_1,
    PRIORITY_MOVES_MINUS_3,
    PRIORITY_MOVES_MINUS_4,
    PRIORITY_MOVES_MINUS_5,
    PRIORITY_MOVES_MINUS_6,
    PRIORITY_MOVES_MINUS_7,
    PRIORITY_MOVES_PLUS_1,
    PRIORITY_MOVES_PLUS_2,
    PRIORITY_MOVES_PLUS_3,
    PRIORITY_MOVES_PLUS_4,
    PROTECTION_MOVES,
    UNUSABLE_MOVES_DURING_GRAVITY
 */

/*
 Move sets that should be in the sim:
    ATTACKER_STAT_BOOST_MOVES,
    MOVES_THAT_BADLY_POISON, maybe
    MOVES_THAT_BOOST_ATTACKERS_ATTACK,
    MOVES_THAT_BOOST_ATTACKERS_DEFENSE,
    MOVES_THAT_BOOST_ATTACKERS_SPECIAL_ATTACK,
    MOVES_THAT_BOOST_ATTACKERS_SPECIAL_DEFENSE,
    MOVES_THAT_BOOST_ATTACKERS_SPEED,
    MOVES_THAT_BREAK_PROTECT,
    MOVES_THAT_BURN, maybe
    MOVES_THAT_BYPASS_PROTECT,
    MOVES_THAT_CAN_BE_REFLECTED_BY_MIRROR_COAT,
    MOVES_THAT_CAN_BE_SNATCHED,
    MOVES_THAT_CHANGE_WEATHER,
    MOVES_THAT_CONTINUE,
    MOVES_THAT_GO_INTO_AIR,
    MOVES_THAT_GO_UNDER_GROUND,
    MOVES_THAT_GO_UNDER_WATER,
    MOVES_THAT_HAVE_FIXED_DAMAGE,
    MOVES_THAT_HEAL_ATTACKER,
    MOVES_THAT_HIT_DEFENDER_IN_AIR,
    MOVES_THAT_HIT_DEFENDER_UNDER_GROUND,
    MOVES_THAT_HIT_DEFENDER_UNDER_WATER,
    MOVES_THAT_LOWER_ATTACKERS_ATTACK,
    MOVES_THAT_LOWER_ATTACKERS_DEFENSE,
    MOVES_THAT_LOWER_ATTACKERS_SPECIAL_ATTACK,
    MOVES_THAT_LOWER_ATTACKERS_SPECIAL_DEFENSE,
    MOVES_THAT_LOWER_ATTACKERS_SPEED,
    MOVES_THAT_LOWER_DEFENDER_ATTACK,
    MOVES_THAT_LOWER_DEFENDER_DEFENSE,
    MOVES_THAT_LOWER_DEFENDER_SPECIAL_ATTACK,
    MOVES_THAT_LOWER_DEFENDER_SPECIAL_DEFENSE,
    MOVES_THAT_LOWER_DEFENDER_SPEED,
    MOVES_THAT_MAKE_ATTACKER_FAINT,
    MOVES_THAT_MAKE_CONTACT,
    MOVES_THAT_OMNI_BOOST_ATTACKER,
    MOVES_THAT_POISON,
    MOVES_THAT_RAISE_DEFENDER_ATTACK,
    MOVES_THAT_RAISE_DEFENDER_SPECIAL_ATTACK,
    MOVES_THAT_REQUIRE_CHARGING_TURN,
    MOVES_THAT_REQUIRE_RECHARGE_TURN,
    MOVES_THAT_VANISH_USER,
    MOVES_WITH_RECOIL,
    MULTI_HIT_MOVES,
    MULTI_TURN_MOVES,
    OTHER_MOVES,
    POWER_MOVES,
    PRIORITY_MOVES_MINUS_1,
    PRIORITY_MOVES_MINUS_3,
    PRIORITY_MOVES_MINUS_4,
    PRIORITY_MOVES_MINUS_5,
    PRIORITY_MOVES_MINUS_6,
    PRIORITY_MOVES_MINUS_7,
    PRIORITY_MOVES_PLUS_1,
    PRIORITY_MOVES_PLUS_2,
    PRIORITY_MOVES_PLUS_3,
    PRIORITY_MOVES_PLUS_4,
    PROTECTION_MOVES,
 */

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
    const std::set<PokemonType>& defender_types,
    const PokemonType& attack_type
) {
    static constexpr int number_of_types = static_cast<int>(PokemonType::COUNT);
    auto key = number_of_types * number_of_types * static_cast<int>(
        attack_type);
    if (auto it = defender_types.begin();
        it != defender_types.end()
    ) {
        key += number_of_types * static_cast<int>(*it);
        ++it;
        if (it != defender_types.end()) {
            key += static_cast<int>(*it);
        }
    }
    static constexpr int cache_size = number_of_types * number_of_types *
        number_of_types;
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
    const bool is_player;
    const CustomPokemon& pokemon;

    const uint8_t level = pokemon.level;
    const std::set<PokemonType> types = pokemon.types;
    Item item = pokemon.item;

    int max_health =
        pokemon.stats.at(static_cast<int>(Stat::HEALTH));
    std::array<uint16_t, 6> current_stats = {
        pokemon.stats.at(static_cast<int>(Stat::HEALTH)),
        pokemon.stats.at(static_cast<int>(Stat::ATTACK)),
        pokemon.stats.at(static_cast<int>(Stat::DEFENSE)),
        pokemon.stats.at(static_cast<int>(Stat::SPECIAL_ATTACK)),
        pokemon.stats.at(static_cast<int>(Stat::SPECIAL_DEFENSE)),
        pokemon.stats.at(static_cast<int>(Stat::SPEED))
    };
    std::array<uint16_t, 6> stat_stages = {
        0, 0, 0, 0, 0, 0
    };


    bool burned = false;
    bool recharging = false;
};

int get_damage(
    const PokemonState& attacker_state,
    const uint16_t attack,
    const std::set<PokemonType>& defender_types,
    const uint16_t defense,
    const MoveInfo* move
) {
    int damage = ((2 * attacker_state.level) / 5) + 2;
    damage = damage * move->power * attack / defense;
    damage = damage / 50;

    damage = damage + 2;
    const auto move_type = move->type;
    if (const auto attacker_types = attacker_state.types;
        attacker_types.contains(move_type)
    ) {
        damage = static_cast<int>(damage * 1.5);
    }
    const auto effectiveness =
        get_effectiveness(defender_types, move_type);
    damage = static_cast<int>(damage * effectiveness);

    if (attacker_state.burned &&
        move->category == Category::PHYSICAL &&
        !MOVES_THAT_HAVE_FIXED_DAMAGE.contains(move->move)
    ) {
        damage = damage / 2;
    }
    return damage;
}

struct BestMove {
    const MoveInfo* move = nullptr;
    int damage = 0;
};

BestMove get_best_move_against_defender(
    const std::vector<const MoveInfo*>& attacker_moves,
    PokemonState& attacker_state,
    const PokemonState& defender_state
) {
    BestMove best_move{};
    if (attacker_state.recharging) {
        attacker_state.recharging = false;
        return best_move;
    }
    const auto attack =
        attacker_state.current_stats.at(static_cast<int>(Stat::ATTACK));
    const auto defense =
        defender_state.current_stats.at(static_cast<int>(Stat::DEFENSE));
    const auto special_attack =
        attacker_state.current_stats.at(static_cast<int>(Stat::SPECIAL_ATTACK));
    const auto special_defense =
        defender_state.current_stats.at(
            static_cast<int>(Stat::SPECIAL_DEFENSE)
        );
    for (const auto& move : attacker_moves) {
        const auto category = move->category;
        if (category == Category::STATUS) {
            continue;
        }
        const bool is_special = category == Category::SPECIAL;
        const auto damage =
            is_special
                ? get_damage(
                    attacker_state,
                    special_attack,
                    defender_state.pokemon.types,
                    special_defense,
                    move
                )
                : get_damage(
                    attacker_state,
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
    return attacker_state.current_stats.at(static_cast<int>(Stat::SPEED)) >
        defender_state.current_stats.at(static_cast<int>(Stat::SPEED));
}

int calculate_state_boost(const uint16_t stat, const uint8_t stage) {
    // TODO other modifiers?
    switch (stage) {
    case -6:
        return 2 * stat / 8;
    case -5:
        return 2 * stat / 7;
    case -4:
        return 2 * stat / 6;
    case -3:
        return 2 * stat / 5;
    case -2:
        return 2 * stat / 4;
    case -1:
        return 2 * stat / 3;
    case 0:
        return stat;
    case 1:
        return 3 * stat / 2;
    case 2:
        return 4 * stat / 2;
    case 3:
        return 5 * stat / 2;
    case 4:
        return 6 * stat / 2;
    case 5:
        return 7 * stat / 2;
    case 6:
        return 8 * stat / 2;
    default:
        throw std::runtime_error("Bad stage");
    }
    throw std::runtime_error("Bad stage");
}

void boost_stat(PokemonState& pokemon_state, const Stat stat) {
    const int index = static_cast<int>(stat);
    const auto stage = pokemon_state.stat_stages.at(index);
    pokemon_state.stat_stages[index] = std::min(stage + 1, 6);
    pokemon_state.current_stats[index] = calculate_state_boost(
        pokemon_state.pokemon.stats.at(index),
        pokemon_state.stat_stages[index]
    );
}

void apply_post_attack_effects(
    PokemonState& attacker_state,
    const BestMove& attacker_move,
    PokemonState& defender_state
) {
    if (attacker_move.move == nullptr) {
        return;
    }
    const auto move = attacker_move.move->move;
    if (MOVES_THAT_REQUIRE_RECHARGE_TURN.contains(move)) {
        attacker_state.recharging = true;
    }

    // Burn
    const bool is_fling = move == Move::Fling;
    if (!defender_state.types.contains(PokemonType::FIRE)) {
        const bool flung_flame_orb =
            (is_fling && attacker_state.item == Item::FlameOrb);
        if (MOVES_THAT_BURN.contains(move) ||
            flung_flame_orb
        ) {
            if (attacker_state.is_player) {
                if (flung_flame_orb || move == Move::WillOWisp) {
                    defender_state.burned = true;
                }
            } else {
                defender_state.burned = true;
            }
        }
    }

    // Omni boost
    if (!attacker_state.is_player &&
        MOVES_THAT_OMNI_BOOST_ATTACKER.contains(move)
    ) {
        boost_stat(attacker_state, Stat::ATTACK);
        boost_stat(attacker_state, Stat::DEFENSE);
        boost_stat(attacker_state, Stat::SPECIAL_ATTACK);
        boost_stat(attacker_state, Stat::SPECIAL_DEFENSE);
        boost_stat(attacker_state, Stat::SPEED);
    }

    if
    (is_fling) {
        // TODO other items
        attacker_state.item = Item::None;
    }
}

void execute_move(
    PokemonState& attacker_state,
    const BestMove& attacker_move,
    PokemonState& defender_state
) {
    defender_state.current_stats[static_cast<int>(Stat::HEALTH)] -=
        attacker_move.damage;
    apply_post_attack_effects(
        attacker_state,
        attacker_move,
        defender_state
    );
}

void apply_end_of_turn_effects(
    PokemonState& state
) {
    if (state.burned) {
        state.current_stats[static_cast<int>(Stat::HEALTH)] -=
            state.max_health / 8;
    }
}

void apply_end_of_turn_effects(
    PokemonState& attacker_state,
    PokemonState& defender_state
) {
    apply_end_of_turn_effects(attacker_state);
    apply_end_of_turn_effects(defender_state);
}

void battle(const CustomPokemon& player, const CustomPokemon& opponent) {
    auto player_state = PokemonState{
        .is_player = true,
        .pokemon = player
    };
    auto opponent_state = PokemonState{
        .is_player = false,
        .pokemon = opponent
    };

    while (player_state.current_stats.at(static_cast<int>(Stat::HEALTH)) > 0 &&
        opponent_state.current_stats.at(static_cast<int>(Stat::HEALTH)) > 0
    ) {
        pre_turn_state_update(
            player_state,
            opponent_state
        );
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

        std::array<Move, 2> moves = {};
        if (player_move.move) {
            moves[0] = player_move.move->move;
        } else {
            moves[0] = Move::Count;
        }
        if (opponent_move.move) {
            moves[1] = opponent_move.move->move;
        } else {
            moves[1] = Move::Count;
        }
        for (const auto move : moves) {
            if (move != Move::Count &&
                !MOVES_THAT_REQUIRE_RECHARGE_TURN.contains(move) &&
                (ATTACKER_STAT_BOOST_MOVES.contains(move) ||
                    MOVES_THAT_BADLY_POISON.contains(move) ||
                    MOVES_THAT_BOOST_ATTACKERS_ATTACK.contains(move) ||
                    MOVES_THAT_BOOST_ATTACKERS_DEFENSE.contains(move) ||
                    MOVES_THAT_BOOST_ATTACKERS_SPECIAL_ATTACK.contains(move) ||
                    MOVES_THAT_BOOST_ATTACKERS_SPECIAL_DEFENSE.contains(move) ||
                    MOVES_THAT_BOOST_ATTACKERS_SPEED.contains(move) ||
                    MOVES_THAT_BREAK_PROTECT.contains(move) ||
                    MOVES_THAT_BYPASS_PROTECT.contains(move) ||
                    MOVES_THAT_CAN_BE_REFLECTED_BY_MIRROR_COAT.contains(move) ||
                    MOVES_THAT_CAN_BE_SNATCHED.contains(move) ||
                    MOVES_THAT_CHANGE_WEATHER.contains(move) ||
                    MOVES_THAT_CONTINUE.contains(move) ||
                    MOVES_THAT_GO_INTO_AIR.contains(move) ||
                    MOVES_THAT_GO_UNDER_GROUND.contains(move) ||
                    MOVES_THAT_GO_UNDER_WATER.contains(move) ||
                    MOVES_THAT_HAVE_FIXED_DAMAGE.contains(move) ||
                    MOVES_THAT_HEAL_ATTACKER.contains(move) ||
                    MOVES_THAT_HIT_DEFENDER_IN_AIR.contains(move) ||
                    MOVES_THAT_HIT_DEFENDER_UNDER_GROUND.contains(move) ||
                    MOVES_THAT_HIT_DEFENDER_UNDER_WATER.contains(move) ||
                    MOVES_THAT_LOWER_ATTACKERS_ATTACK.contains(move) ||
                    MOVES_THAT_LOWER_ATTACKERS_DEFENSE.contains(move) ||
                    MOVES_THAT_LOWER_ATTACKERS_SPECIAL_ATTACK.contains(move) ||
                    MOVES_THAT_LOWER_ATTACKERS_SPECIAL_DEFENSE.contains(move) ||
                    MOVES_THAT_LOWER_ATTACKERS_SPEED.contains(move) ||
                    MOVES_THAT_LOWER_DEFENDER_ATTACK.contains(move) ||
                    MOVES_THAT_LOWER_DEFENDER_DEFENSE.contains(move) ||
                    MOVES_THAT_LOWER_DEFENDER_SPECIAL_ATTACK.contains(move) ||
                    MOVES_THAT_LOWER_DEFENDER_SPECIAL_DEFENSE.contains(move) ||
                    MOVES_THAT_LOWER_DEFENDER_SPEED.contains(move) ||
                    MOVES_THAT_MAKE_ATTACKER_FAINT.contains(move) ||
                    MOVES_THAT_MAKE_CONTACT.contains(move) ||
                    MOVES_THAT_POISON.contains(move) ||
                    MOVES_THAT_RAISE_DEFENDER_ATTACK.contains(move) ||
                    MOVES_THAT_RAISE_DEFENDER_SPECIAL_ATTACK.contains(move) ||
                    MOVES_THAT_REQUIRE_CHARGING_TURN.contains(move) ||
                    MOVES_THAT_REQUIRE_RECHARGE_TURN.contains(move) ||
                    MOVES_THAT_VANISH_USER.contains(move) ||
                    MOVES_WITH_RECOIL.contains(move) ||
                    MULTI_HIT_MOVES.contains(move) ||
                    MULTI_TURN_MOVES.contains(move) ||
                    OTHER_MOVES.contains(move) ||
                    PRIORITY_MOVES_MINUS_1.contains(move) ||
                    PRIORITY_MOVES_MINUS_3.contains(move) ||
                    PRIORITY_MOVES_MINUS_4.contains(move) ||
                    PRIORITY_MOVES_MINUS_5.contains(move) ||
                    PRIORITY_MOVES_MINUS_6.contains(move) ||
                    PRIORITY_MOVES_MINUS_7.contains(move) ||
                    PRIORITY_MOVES_PLUS_1.contains(move) ||
                    PRIORITY_MOVES_PLUS_2.contains(move) ||
                    PRIORITY_MOVES_PLUS_3.contains(move) ||
                    PRIORITY_MOVES_PLUS_4.contains(move) ||
                    PROTECTION_MOVES.contains(move))
            ) {
                printf("");
            }
        }
        if (player_goes_first) {
            execute_move(
                player_state,
                player_move,
                opponent_state
            );
            if (player_state.current_stats.at(static_cast<int>(Stat::HEALTH)) <=
                0 ||
                opponent_state.current_stats.at(static_cast<int>(Stat::HEALTH))
                <= 0
            ) {
                break;
            }
            execute_move(
                opponent_state,
                opponent_move,
                player_state
            );
            if (player_state.current_stats.at(static_cast<int>(Stat::HEALTH)) <=
                0 ||
                opponent_state.current_stats.at(static_cast<int>(Stat::HEALTH))
                <= 0
            ) {
                break;
            }
        } else {
            execute_move(
                opponent_state,
                opponent_move,
                player_state
            );
            if (player_state.current_stats.at(static_cast<int>(Stat::HEALTH)) <=
                0 ||
                opponent_state.current_stats.at(static_cast<int>(Stat::HEALTH))
                <= 0
            ) {
                break;
            }
            execute_move(
                player_state,
                player_move,
                opponent_state
            );
            if (player_state.current_stats.at(static_cast<int>(Stat::HEALTH)) <=
                0 ||
                opponent_state.current_stats.at(static_cast<int>(Stat::HEALTH))
                <= 0
            ) {
                break;
            }
        }
        apply_end_of_turn_effects(
            player_state,
            opponent_state
        );
        if (player_move.damage == 0 &&
            opponent_move.damage == 0 &&
            !player_state.recharging &&
            !opponent_state.recharging
        ) {
            break;
        }
    }
}
