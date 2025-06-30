#include "battle_simulator.h"

#include <algorithm>
#include <cmath>
#include <set>

#include "custom_pokemon.h"
#include "Items.h"
#include  "serebii_pokemon_data_source.h"
#include "type_effectiveness.h"

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

enum class Status {
    BURN,
    POISON,
    SLEEP,
    NONE
};

enum class FieldLocation {
    IN_AIR,
    UNDER_GROUND,
    UNDER_WATER,
    IN_THE_VOID,
    ON_FIELD
};

struct PokemonState {
    const bool is_player;
    const CustomPokemon& pokemon;

    const uint8_t level = pokemon.level;
    const std::array<PokemonType, 2> types = pokemon.types;
    Item item = pokemon.item;
    Ability ability = pokemon.ability;
    FieldLocation field_location = FieldLocation::ON_FIELD;

    int max_health =
        pokemon.stats.at(static_cast<int>(Stat::HEALTH));
    std::array<int, 6> current_stats = {
        pokemon.stats.at(static_cast<int>(Stat::HEALTH)),
        pokemon.stats.at(static_cast<int>(Stat::ATTACK)),
        pokemon.stats.at(static_cast<int>(Stat::DEFENSE)),
        pokemon.stats.at(static_cast<int>(Stat::SPECIAL_ATTACK)),
        pokemon.stats.at(static_cast<int>(Stat::SPECIAL_DEFENSE)),
        pokemon.stats.at(static_cast<int>(Stat::SPEED))
    };
    std::array<int, 6> stat_stages = {
        0, 0, 0, 0, 0, 0
    };


    Status status = Status::NONE;
    bool confused = false;
    bool recharging = false;
};

int get_damage(
    const PokemonState& attacker_state,
    const uint16_t attack,
    const PokemonState& defender_state,
    const uint16_t defense,
    const MoveInfo* move,
    const bool recalculate
) {
    int damage = ((2 * attacker_state.level) / 5) + 2;
    int power = move->power;

    if (move->move == Move::Eruption) {
        power = std::min(
            1,
            static_cast<int>(
                std::floor(
                    150 * attacker_state.current_stats[
                        static_cast<int>(Stat::HEALTH)
                    ] / attacker_state.max_health
                )
            )
        );
    }

    // Field location
    const bool attacker_faster =
        attacker_state.current_stats.at(static_cast<int>(Stat::SPEED)) >
        defender_state.current_stats.at(static_cast<int>(Stat::SPEED));
    if (const auto field_location = defender_state.field_location;
        field_location != FieldLocation::ON_FIELD
    ) {
        if (!attacker_faster && !recalculate) {
            return 0;
        }
        if (field_location == FieldLocation::IN_AIR) {
            if (MOVES_THAT_HIT_DEFENDER_IN_AIR.contains(move->move)
            ) {
                if (move->move == Move::Gust ||
                    move->move == Move::Twister
                ) {
                    power *= 2;
                }
            } else {
                return 0;
            }
        } else if (field_location == FieldLocation::IN_THE_VOID) {
            return 0;
        } else if (field_location == FieldLocation::UNDER_GROUND) {
            if (MOVES_THAT_HIT_DEFENDER_UNDER_GROUND.contains(move->move)) {
                if (move->move == Move::Earthquake ||
                    move->move == Move::Magnitude
                ) {
                    power *= 2;
                }
            } else {
                return 0;
            }
        } else if (field_location == FieldLocation::UNDER_WATER) {
            if (MOVES_THAT_HIT_DEFENDER_UNDER_WATER.contains(move->move)) {
                power *= 2;
            } else {
                return 0;
            }
        }
    }

    damage = damage * power * attack / defense;
    damage = damage / 50;
    damage = damage + 2;

    // Type effectiveness
    const auto move_type = move->type;
    if (attacker_state.types[0] == move_type ||
        attacker_state.types[1] == move_type
    ) {
        damage = static_cast<int>(damage * 1.5);
    }
    const auto effectiveness =
        get_effectiveness(defender_state.types, move_type);
    damage = static_cast<int>(damage * effectiveness);

    // Random
    if (attacker_state.is_player) {
        damage = std::floor(damage * 0.85);
    }

    // Burn
    if (attacker_state.status == Status::BURN &&
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
    const PokemonState& defender_state,
    const bool recalculate,
    const BestMove& last_picked_move
) {
    if (attacker_state.field_location != FieldLocation::ON_FIELD
    ) {
        return last_picked_move;
    }

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
                    defender_state,
                    special_defense,
                    move,
                    recalculate
                )
                : get_damage(
                    attacker_state,
                    attack,
                    defender_state,
                    defense,
                    move,
                    recalculate
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

int get_move_priority(const BestMove& best_move) {
    int priority = 0;
    if (best_move.move == nullptr) {
        return 0;
    }
    if (const auto move = best_move.move->move; PRIORITY_MOVES_MINUS_7.
        contains(move)) {
        priority = -7;
    } else if (PRIORITY_MOVES_MINUS_6.contains(move)) {
        priority = -6;
    } else if (PRIORITY_MOVES_MINUS_5.contains(move)) {
        priority = -5;
    } else if (PRIORITY_MOVES_MINUS_4.contains(move)) {
        priority = -4;
    } else if (PRIORITY_MOVES_MINUS_3.contains(move)) {
        priority = -3;
    } else if (PRIORITY_MOVES_MINUS_1.contains(move)) {
        priority = -1;
    } else if (PRIORITY_MOVES_PLUS_1.contains(move)) {
        priority = 1;
    } else if (PRIORITY_MOVES_PLUS_2.contains(move)) {
        priority = 2;
    } else if (PRIORITY_MOVES_PLUS_3.contains(move)) {
        priority = 3;
    } else if (PRIORITY_MOVES_PLUS_4.contains(move)) {
        priority = 4;
    } else if (PRIORITY_MOVES_PLUS_5.contains(move)) {
        priority = 5;
    }
    return priority;
}

bool is_player_first(
    const PokemonState& player_state,
    const BestMove player_move,
    const PokemonState& opponent_state,
    const BestMove opponent_move
) {
    const int player_priority = get_move_priority(player_move);
    const int opponent_priority = get_move_priority(opponent_move);
    if (player_priority != opponent_priority) {
        return player_priority > opponent_priority;
    }
    return player_state.current_stats.at(static_cast<int>(Stat::SPEED)) >
        opponent_state.current_stats.at(static_cast<int>(Stat::SPEED));
}

int calculate_state_boost(const uint16_t stat, const int stage) {
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

void change_stat_modifier(
    PokemonState& pokemon_state,
    const Stat stat,
    const int change
) {
    const int index = static_cast<int>(stat);
    const auto stage = pokemon_state.stat_stages.at(index);
    if (change > 0) {
        pokemon_state.stat_stages[index] = std::min(stage + change, 6);
    } else {
        pokemon_state.stat_stages[index] = std::max(stage + change, -6);
    }
    pokemon_state.current_stats[index] = calculate_state_boost(
        pokemon_state.pokemon.stats.at(index),
        pokemon_state.stat_stages[index]
    );
}

void apply_berry(PokemonState& pokemon_state) {
    if (const auto berry = pokemon_state.item;
        berry == Item::ApicotBerry
    ) {
        change_stat_modifier(pokemon_state, Stat::SPECIAL_DEFENSE, 1);
    } else if (berry == Item::ChestoBerry) {
        if (pokemon_state.status == Status::SLEEP) {
            pokemon_state.status = Status::NONE;
        }
    } else if (berry == Item::GanlonBerry) {
        change_stat_modifier(pokemon_state, Stat::DEFENSE, 1);
    } else if (berry == Item::LiechiBerry) {
        change_stat_modifier(pokemon_state, Stat::ATTACK, 1);
    } else if (berry == Item::LumBerry) {
        if (pokemon_state.status != Status::NONE) {
            pokemon_state.status = Status::NONE;
        }
        if (pokemon_state.confused) {
            pokemon_state.confused = false;
        }
    } else if (berry == Item::PersimBerry) {
        if (pokemon_state.confused) {
            pokemon_state.confused = false;
        }
    } else if (berry == Item::PetayaBerry) {
        change_stat_modifier(pokemon_state, Stat::SPECIAL_ATTACK, 1);
    } else if (berry == Item::SalacBerry) {
        change_stat_modifier(pokemon_state, Stat::SPEED, 1);
    } else if (berry == Item::SitrusBerry) {
        const auto current_health =
            pokemon_state.current_stats[static_cast<int>(Stat::HEALTH)];
        const int max_health = pokemon_state.max_health;
        pokemon_state.current_stats[static_cast<int>(Stat::HEALTH)] =
            std::max(max_health, current_health + max_health / 4);
    }
    pokemon_state.item = Item::None;
}

void try_apply_berry(PokemonState pokemon_state, const bool eaten) {
    if (const auto item = pokemon_state.item;
        BERRIES.contains(item)
    ) {
        if (eaten) {
            apply_berry(pokemon_state);
        }
    }
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

    // Recoil
    if (MOVES_WITH_RECOIL.contains(move)) {
        if (move == Move::HeadSmash ||
            (attacker_state.is_player &&
                move == Move::JumpKick ||
                move == Move::HighJumpKick)
        ) {
            attacker_state.current_stats[static_cast<int>(Stat::HEALTH)] -=
                attacker_move.damage / 2;
        } else if (
            move == Move::BraveBird ||
            move == Move::DoubleEdge ||
            move == Move::FlareBlitz ||
            move == Move::VoltTackle ||
            move == Move::WoodHammer
        ) {
            attacker_state.current_stats[static_cast<int>(Stat::HEALTH)] -=
                attacker_move.damage / 3;
        } else if (
            move == Move::Submission ||
            move == Move::TakeDown
        ) {
            attacker_state.current_stats[static_cast<int>(Stat::HEALTH)] -=
                attacker_move.damage / 4;
        } else if (move == Move::BellyDrum) {
            attacker_state.current_stats[static_cast<int>(Stat::HEALTH)] -=
                attacker_state.max_health / 2;
            change_stat_modifier(attacker_state, Stat::ATTACK, 6);
        }
    }

    // Contact moves
    if (MOVES_THAT_MAKE_CONTACT.contains(move)) {
        const auto defender_ability = defender_state.ability;
        if (attacker_state.is_player) {
            if (defender_state.status == Status::NONE) {
                if (defender_ability == Ability::EffectSpore ||
                    defender_ability == Ability::PoisonPoint
                ) {
                    defender_state.status = Status::POISON;
                } else if (defender_ability == Ability::FlameBody) {
                    defender_state.status = Status::BURN;
                }
            }
        }
        if (defender_ability == Ability::RoughSkin) {
            attacker_state.current_stats[static_cast<int>(Stat::HEALTH)] -=
                attacker_state.max_health / 8;
        }
        if (attacker_state.item == Item::RockyHelmet) {
            defender_state.current_stats[static_cast<int>(Stat::HEALTH)] -=
                defender_state.max_health / 6;
        }
        if (attacker_state.item == Item::None &&
            defender_state.item == Item::StickyBarb
        ) {
            attacker_state.item = Item::StickyBarb;
            defender_state.item = Item::None;
        }
    }

    // Burn
    const bool is_fling = move == Move::Fling;
    if (defender_state.types[0] != PokemonType::FIRE &&
        defender_state.types[1] != PokemonType::FIRE
    ) {
        const bool flung_flame_orb =
            (is_fling && attacker_state.item == Item::FlameOrb);
        if (MOVES_THAT_BURN.contains(move) ||
            flung_flame_orb
        ) {
            if (attacker_state.is_player) {
                if (flung_flame_orb || move == Move::WillOWisp) {
                    if (defender_state.status == Status::NONE) {
                        defender_state.status = Status::BURN;
                    }
                }
            } else {
                if (defender_state.status == Status::NONE) {
                    defender_state.status = Status::BURN;
                }
            }
        }
    }

    // Poison
    if (defender_state.types[0] != PokemonType::POISON &&
        defender_state.types[1] != PokemonType::POISON &&
        defender_state.types[0] != PokemonType::STEEL &&
        defender_state.types[1] != PokemonType::STEEL
    ) {
        const bool flung_poison_barb =
            (is_fling && attacker_state.item == Item::PoisonBarb);
        if (MOVES_THAT_POISON.contains(move) ||
            flung_poison_barb
        ) {
            if (attacker_state.is_player) {
                if (flung_poison_barb ||
                    move == Move::PoisonGas ||
                    move == Move::PoisonPowder
                ) {
                    if (defender_state.status == Status::NONE) {
                        defender_state.status = Status::POISON;
                    }
                }
            } else {
                if (defender_state.status == Status::NONE) {
                    defender_state.status = Status::POISON;
                }
            }
        }
    }

    // Special Attack
    if (MOVES_THAT_LOWER_ATTACKERS_SPECIAL_ATTACK.contains(move)) {
        change_stat_modifier(attacker_state, Stat::SPECIAL_ATTACK, -2);
    }

    // Special Defense
    if (MOVES_THAT_LOWER_DEFENDER_SPECIAL_DEFENSE.contains(move)) {
        if (move == Move::SeedFlare ||
            move == Move::MetalSound ||
            move == Move::FakeTears
        ) {
            change_stat_modifier(defender_state, Stat::SPECIAL_DEFENSE, -2);
        }
        if (move == Move::ShadowBall ||
            move == Move::Psychic ||
            move == Move::Acid ||
            move == Move::LusterPurge ||
            move == Move::BugBuzz ||
            move == Move::FocusBlast ||
            move == Move::EnergyBall ||
            move == Move::EarthPower ||
            move == Move::FlashCannon
        ) {
            change_stat_modifier(defender_state, Stat::SPECIAL_DEFENSE, -1);
        }
    }

    // Omni boost
    if (!attacker_state.is_player &&
        MOVES_THAT_OMNI_BOOST_ATTACKER.contains(move)
    ) {
        change_stat_modifier(attacker_state, Stat::ATTACK, 1);
        change_stat_modifier(attacker_state, Stat::DEFENSE, 1);
        change_stat_modifier(attacker_state, Stat::SPECIAL_ATTACK, 1);
        change_stat_modifier(attacker_state, Stat::SPECIAL_DEFENSE, 1);
        change_stat_modifier(attacker_state, Stat::SPEED, 1);
    }

    if (is_fling) {
        // TODO other items
        attacker_state.item = Item::None;
    }
}

void execute_move(
    PokemonState& attacker_state,
    BestMove& attacker_move,
    PokemonState& defender_state,
    BestMove& defender_move
) {
    if (attacker_move.move == nullptr) {
        return;
    }

    bool attacker_vanished = false;
    if (const bool on_field =
            attacker_state.field_location == FieldLocation::ON_FIELD;
        MOVES_THAT_GO_INTO_AIR.contains(attacker_move.move->move) && on_field
    ) {
        attacker_state.field_location = FieldLocation::IN_AIR;
        attacker_vanished = true;
    } else if (MOVES_THAT_GO_UNDER_GROUND.contains(attacker_move.move->move) &&
        on_field
    ) {
        attacker_state.field_location = FieldLocation::UNDER_GROUND;
        attacker_vanished = true;
    } else if (MOVES_THAT_GO_UNDER_WATER.contains(attacker_move.move->move) &&
        on_field
    ) {
        attacker_state.field_location = FieldLocation::UNDER_WATER;
        attacker_vanished = true;
    } else if (MOVES_THAT_VANISH_USER.contains(attacker_move.move->move) &&
        on_field
    ) {
        attacker_state.field_location = FieldLocation::IN_THE_VOID;
        attacker_vanished = true;
    }
    if (!attacker_vanished &&
        attacker_state.field_location != FieldLocation::ON_FIELD
    ) {
        attacker_state.field_location = FieldLocation::ON_FIELD;
    }

    if (attacker_state.field_location != FieldLocation::ON_FIELD) {
        // Recalculate the damage of the next move of the opponent
        auto [move, damage] = get_best_move_against_defender(
            {defender_move.move},
            defender_state,
            attacker_state,
            true,
            defender_move
        );
        defender_move.damage = damage;
    }

    if (attacker_state.field_location == FieldLocation::ON_FIELD) {
        if (attacker_move.move->move == Move::SuckerPunch &&
            defender_move.move->category == Category::STATUS
        ) {
            attacker_move.damage = 0;
        }
        defender_state.current_stats[static_cast<int>(Stat::HEALTH)] -=
            attacker_move.damage;
        apply_post_attack_effects(
            attacker_state,
            attacker_move,
            defender_state
        );
    }
}

void apply_end_of_turn_effects(
    PokemonState& state
) {
    if (state.ability != Ability::MagicGuard) {
        if (state.status == Status::BURN ||
            state.status == Status::POISON
        ) {
            state.current_stats[static_cast<int>(Stat::HEALTH)] -=
                state.max_health / 8;
        }
        if (state.item == Item::StickyBarb) {
            state.current_stats[static_cast<int>(Stat::HEALTH)] -=
                state.max_health / 8;
        }
    }
}

void apply_end_of_turn_effects(
    PokemonState& player_state,
    PokemonState& opponent_state
) {
    apply_end_of_turn_effects(player_state);
    apply_end_of_turn_effects(opponent_state);
}

bool battle(const CustomPokemon& player, const CustomPokemon& opponent) {
    auto player_state = PokemonState{
        .is_player = true,
        .pokemon = player
    };
    auto opponent_state = PokemonState{
        .is_player = false,
        .pokemon = opponent
    };

    constexpr int heath_index = static_cast<int>(Stat::HEALTH);
    BestMove player_move;
    BestMove opponent_move;
    while (player_state.current_stats.at(heath_index) > 0 &&
        opponent_state.current_stats.at(heath_index) > 0
    ) {
        pre_turn_state_update(
            player_state,
            opponent_state
        );
        player_move = get_best_move_against_defender(
            player.moves,
            player_state,
            opponent_state,
            false,
            player_move
        );
        opponent_move = get_best_move_against_defender(
            opponent.moves,
            opponent_state,
            player_state,
            false,
            opponent_move
        );
        const bool player_goes_first = is_player_first(
            player_state,
            player_move,
            opponent_state,
            opponent_move
        );

        // For debugging unimplemented attacks
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
                move != Move::Eruption &&
                move != Move::SuckerPunch &&
                move != Move::BugBite &&
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
                    MOVES_THAT_HAVE_FIXED_DAMAGE.contains(move) ||
                    MOVES_THAT_HEAL_ATTACKER.contains(move) ||
                    MOVES_THAT_LOWER_ATTACKERS_ATTACK.contains(move) ||
                    MOVES_THAT_LOWER_ATTACKERS_DEFENSE.contains(move) ||
                    MOVES_THAT_LOWER_ATTACKERS_SPECIAL_DEFENSE.contains(move) ||
                    MOVES_THAT_LOWER_ATTACKERS_SPEED.contains(move) ||
                    MOVES_THAT_LOWER_DEFENDER_ATTACK.contains(move) ||
                    MOVES_THAT_LOWER_DEFENDER_DEFENSE.contains(move) ||
                    MOVES_THAT_LOWER_DEFENDER_SPECIAL_ATTACK.contains(move) ||
                    MOVES_THAT_LOWER_DEFENDER_SPEED.contains(move) ||
                    MOVES_THAT_MAKE_ATTACKER_FAINT.contains(move) ||
                    MOVES_THAT_RAISE_DEFENDER_ATTACK.contains(move) ||
                    MOVES_THAT_RAISE_DEFENDER_SPECIAL_ATTACK.contains(move) ||
                    MOVES_THAT_REQUIRE_CHARGING_TURN.contains(move) ||
                    MULTI_HIT_MOVES.contains(move) ||
                    MULTI_TURN_MOVES.contains(move) ||
                    OTHER_MOVES.contains(move) ||
                    PROTECTION_MOVES.contains(move))
            ) {
                printf("");
            }
        }
        if (player_goes_first) {
            execute_move(
                player_state,
                player_move,
                opponent_state,
                opponent_move
            );
            if (player_state.current_stats.at(heath_index) <= 0 ||
                opponent_state.current_stats.at(heath_index) <= 0
            ) {
                break;
            }
            execute_move(
                opponent_state,
                opponent_move,
                player_state,
                player_move
            );
            if (player_state.current_stats.at(heath_index) <= 0 ||
                opponent_state.current_stats.at(heath_index) <= 0
            ) {
                break;
            }
        } else {
            execute_move(
                opponent_state,
                opponent_move,
                player_state,
                player_move
            );
            if (player_state.current_stats.at(heath_index) <= 0 ||
                opponent_state.current_stats.at(heath_index) <= 0
            ) {
                break;
            }
            execute_move(
                player_state,
                player_move,
                opponent_state,
                opponent_move
            );
            if (player_state.current_stats.at(heath_index) <= 0 ||
                opponent_state.current_stats.at(heath_index) <= 0
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
    if (player_state.current_stats.at(heath_index) > 0 &&
        opponent_state.current_stats.at(heath_index) < 0 &&
        opponent_state.ability == Ability::Aftermath &&
        player_state.ability != Ability::Damp
    ) {
        player_state.current_stats[heath_index] -= player_state.max_health / 4;
    }

    if (player_state.current_stats.at(heath_index) > 0) {
        return true;
    }
    return false;
}
