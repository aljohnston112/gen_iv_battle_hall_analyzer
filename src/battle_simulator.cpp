#include "battle_simulator.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <set>

#include "config.h"
#include "custom_pokemon.h"
#include "Items.h"
#include "PokemonState.h"
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

struct BestMove {
    const MoveInfo* move = nullptr;
    int damage = 0;
};

int get_move_priority(const MoveInfo* move_info) {
    if (move_info == nullptr) {
        return 0;
    }
    const auto move = static_cast<int>(move_info->move);
    return MOVE_PRIORITIES[move];
}

void apply_end_of_turn_effects(
    PokemonState& state
) {
    if (state.ability != Ability::MagicGuard) {
        if (state.status == Status::BADLY_POISONED) {
            state.turns_badly_poisoned =
                std::min(15, state.turns_badly_poisoned + 1);
            state.current_stats[HEALTH_INDEX] -=
                state.turns_badly_poisoned * (state.max_health / 16);
        }
        if (state.status == Status::BURN ||
            state.status == Status::POISON
        ) {
            state.current_stats[HEALTH_INDEX] -= state.max_health / 8;
        }
        if (state.item == Item::StickyBarb) {
            state.current_stats[HEALTH_INDEX] -= state.max_health / 8;
        }
        if (state.trapped_counter > 0) {
            state.current_stats[HEALTH_INDEX] -= state.max_health / 8;
        }
    }
    if (state.trapped_counter > 0) {
        state.trapped_counter--;
    }
}

void apply_end_of_turn_effects(
    PokemonState& player_state,
    PokemonState& opponent_state
) {
    apply_end_of_turn_effects(player_state);
    apply_end_of_turn_effects(opponent_state);
}


int calculate_stat_based_on_stage(const uint16_t stat, const int stage) {
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
}

constexpr int MAX_STAT_STAGE = 6;
constexpr int MIN_STAT_STAGE = -6;

void change_stat_modifier(
    PokemonState& pokemon_state,
    const Stat stat,
    const int change
) {
    const int index = static_cast<int>(stat);
    const auto stage = pokemon_state.stat_stages[index];
    const int new_stage = stage + change;
    if (change > 0) {
        pokemon_state.stat_stages[index] =
            std::min(new_stage, MAX_STAT_STAGE);
    } else {
        pokemon_state.stat_stages[index] =
            std::max(new_stage, MIN_STAT_STAGE);
    }
    pokemon_state.current_stats[index] =
        calculate_stat_based_on_stage(
            pokemon_state.pokemon.stats[index],
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
            pokemon_state.current_stats[HEALTH_INDEX];
        const int max_health = pokemon_state.max_health;
        pokemon_state.current_stats[HEALTH_INDEX] =
            std::min(max_health, current_health + max_health / 4);
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

void try_apply_status(PokemonState& pokemon_state, const Status status) {
    if (pokemon_state.status == Status::NONE) {
        pokemon_state.status = status;
    }
}

bool move_has_flag(const Move move, const MoveFlag move_flag) {
    return MOVE_FLAGS[static_cast<int>(move)].test(
        static_cast<size_t>(move_flag)
    );
}

void heal(PokemonState& pokemon_state, int health_gained) {
    pokemon_state.current_stats[HEALTH_INDEX] += health_gained;
    if (pokemon_state.current_stats[HEALTH_INDEX] > pokemon_state.max_health) {
        pokemon_state.current_stats[HEALTH_INDEX] = pokemon_state.max_health;
    }
}

void apply_post_attack_effects(
    BattleState& battle_state,
    PokemonState& attacker_state,
    const BestMove& attacker_move,
    PokemonState& defender_state
) {
    if (attacker_move.move == nullptr) {
        return;
    }
    const auto move = attacker_move.move->move;

    // Healing
    if (move_has_flag(move, MoveFlag::HEALS_ATTACKER)) {
        if (attacker_move.damage > 0 &&
            (move == Move::Absorb ||
                move == Move::MegaDrain ||
                move == Move::LeechLife ||
                move == Move::GigaDrain ||
                move == Move::DrainPunch)
        ) {
            int health_gained = attacker_move.damage / 2;

            if (attacker_state.item == Item::BigRoot) {
                health_gained = std::floor(attacker_move.damage * 0.65);
            }

            health_gained = std::max(1, health_gained);
            if (defender_state.ability == Ability::LiquidOoze) {
                health_gained = -health_gained;
            }
            heal(attacker_state, health_gained);
        }
        if (move == Move::Recover ||
            move == Move::Softboiled ||
            move == Move::MilkDrink ||
            move == Move::SlackOff ||
            move == Move::HealOrder
        ) {
            heal(attacker_state, attacker_state.max_health / 2);
        }
        if (move == Move::MorningSun ||
            move == Move::Synthesis ||
            move == Move::Moonlight
        ) {
            if (battle_state.weather == Weather::SUN) {
                heal(
                    attacker_state,
                    2 * attacker_state.max_health / 3
                );
            } else if (battle_state.weather == Weather::CLEAR) {
                heal(
                    attacker_state,
                    attacker_state.max_health / 2
                );
            } else {
                heal(
                    attacker_state,
                    attacker_state.max_health / 4
                );
            }
        }
    }

    // Multiturn
    if (move_has_flag(move, MoveFlag::LASTS_MULTIPLE_TURNS)) {
        uint8_t player_turns = 0;
        uint8_t opponent_turns = 0;
        if (move == Move::Outrage) {
            player_turns = 3;
            opponent_turns = 2;
        }
        if (move == Move::Thrash ||
            move == Move::PetalDance
        ) {
            player_turns = 4;
            opponent_turns = 3;
        }

        if (attacker_state.is_player &&
            attacker_state.multi_turn_move_counter >= player_turns
        ) {
            attacker_state.multi_turn_move_counter = 0;
        } else {
            attacker_state.multi_turn_move_counter++;
        }

        if (!attacker_state.is_player &&
            attacker_state.multi_turn_move_counter >= opponent_turns
        ) {
            attacker_state.multi_turn_move_counter = 0;
        } else {
            attacker_state.multi_turn_move_counter++;
        }
    }

    if (move == Move::Uproar) {
        if (defender_state.status == Status::SLEEP) {
            defender_state.status = Status::NONE;
        }
        if (constexpr uint8_t player_turns = 5;
            attacker_state.is_player &&
            attacker_state.multi_turn_move_counter >= player_turns
        ) {
            attacker_state.multi_turn_move_counter = 0;
        } else {
            attacker_state.multi_turn_move_counter++;
        }

        if (constexpr uint8_t opponent_turns = 2;
            !attacker_state.is_player &&
            attacker_state.multi_turn_move_counter >= opponent_turns
        ) {
            attacker_state.multi_turn_move_counter = 0;
        } else {
            attacker_state.multi_turn_move_counter++;
        }
    }

    if (move == Move::BugBite) {
        if (BERRIES.contains(defender_state.item) &&
            defender_state.ability != Ability::StickyHold
        ) {
            const Item backup = attacker_state.item;
            attacker_state.item = defender_state.item;
            defender_state.item = Item::None;
            try_apply_berry(attacker_state, true);
            attacker_state.item = backup;
        }
    }

    if (move_has_flag(move, MoveFlag::REQUIRES_RECHARGE_TURN)) {
        attacker_state.recharging = true;
    }

    // Recoil
    if (move_has_flag(move, MoveFlag::HAS_RECOIL)) {
        if (move == Move::HeadSmash ||
            (attacker_state.is_player &&
                (move == Move::JumpKick ||
                    move == Move::HighJumpKick))
        ) {
            attacker_state.current_stats[HEALTH_INDEX] -=
                attacker_move.damage / 2;
        } else if (
            move == Move::BraveBird ||
            move == Move::DoubleEdge ||
            move == Move::FlareBlitz ||
            move == Move::VoltTackle ||
            move == Move::WoodHammer
        ) {
            attacker_state.current_stats[HEALTH_INDEX] -=
                attacker_move.damage / 3;
        } else if (
            move == Move::Submission ||
            move == Move::TakeDown
        ) {
            attacker_state.current_stats[HEALTH_INDEX] -=
                attacker_move.damage / 4;
        } else if (move == Move::BellyDrum) {
            attacker_state.current_stats[HEALTH_INDEX] -=
                attacker_state.max_health / 2;
            change_stat_modifier(attacker_state, Stat::ATTACK, 6);
        }
    }

    // Contact moves
    if (move_has_flag(move, MoveFlag::MAKES_CONTACT)) {
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
            attacker_state.current_stats[HEALTH_INDEX] -=
                attacker_state.max_health / 8;
        }
        if (attacker_state.item == Item::RockyHelmet) {
            defender_state.current_stats[HEALTH_INDEX] -=
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
        if (move_has_flag(move, MoveFlag::BURNS_DEFENDER) ||
            flung_flame_orb
        ) {
            if (attacker_state.is_player) {
                if (flung_flame_orb || move == Move::WillOWisp) {
                    try_apply_status(defender_state, Status::BURN);
                }
            } else {
                try_apply_status(defender_state, Status::BURN);
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
        if ((move_has_flag(move, MoveFlag::POISONS_DEFENDER)) ||
            flung_poison_barb
        ) {
            if (attacker_state.is_player) {
                if (flung_poison_barb ||
                    move == Move::PoisonGas ||
                    move == Move::PoisonPowder
                ) {
                    try_apply_status(defender_state, Status::POISON);
                }
            } else {
                try_apply_status(defender_state, Status::POISON);
            }
        }
    }

    // Bad poison
    if (move_has_flag(move, MoveFlag::BADLY_POISONS)) {
        if (defender_state.is_player) {
            try_apply_status(defender_state, Status::BADLY_POISONED);
        }
        if (move == Move::Toxic) {
            try_apply_status(defender_state, Status::BADLY_POISONED);
        }
    }

    // Other status
    if (move == Move::TriAttack && defender_state.is_player) {
        try_apply_status(defender_state, Status::POISON);
    }

    // Attack
    if (move_has_flag(move, MoveFlag::BOOSTS_ATTACKERS_ATTACK)) {
        if (move == Move::Meditate ||
            move == Move::Sharpen ||
            move == Move::Howl ||
            move == Move::BulkUp ||
            move == Move::DragonDance
        ) {
            change_stat_modifier(attacker_state, Stat::ATTACK, 1);
        }
        if (move == Move::SwordsDance) {
            change_stat_modifier(attacker_state, Stat::ATTACK, 2);
        }
        if (move == Move::BellyDrum) {
            change_stat_modifier(attacker_state, Stat::ATTACK, 6);
        }
        if (!attacker_state.is_player &&
            (move == Move::MeteorMash ||
                move == Move::MetalClaw)
        ) {
            change_stat_modifier(attacker_state, Stat::ATTACK, 1);
        }
    }

    if (move_has_flag(move, MoveFlag::LOWERS_DEFENDER_ATTACK)) {
        if (move == Move::Tickle ||
            move == Move::Growl
        ) {
            change_stat_modifier(defender_state, Stat::ATTACK, -1);
        }
        if (move == Move::Charm ||
            move == Move::Memento ||
            move == Move::Featherdance
        ) {
            change_stat_modifier(defender_state, Stat::ATTACK, -2);
        }
        if (defender_state.is_player &&
            move == Move::AuroraBeam
        ) {
            change_stat_modifier(defender_state, Stat::ATTACK, -1);
        }
    }

    // Defense
    if (move_has_flag(move, MoveFlag::BOOSTS_ATTACKERS_DEFENSE)) {
        if (move == Move::BulkUp) {
            change_stat_modifier(attacker_state, Stat::DEFENSE, 1);
        }
    }
    if (move_has_flag(move, MoveFlag::LOWERS_ATTACKERS_DEFENSE)) {
        change_stat_modifier(attacker_state, Stat::DEFENSE, -1);
    }

    if (move_has_flag(move, MoveFlag::LOWERS_DEFENDER_DEFENSE)) {
        if (defender_state.is_player &&
            (move == Move::IronTail ||
                move == Move::Crunch ||
                move == Move::RockSmash ||
                move == Move::CrushClaw)
        ) {
            change_stat_modifier(defender_state, Stat::DEFENSE, -1);
        }
        if (move == Move::TailWhip ||
            move == Move::Leer ||
            move == Move::Tickle
        ) {
            change_stat_modifier(defender_state, Stat::DEFENSE, -1);
        }
        if (move == Move::Screech) {
            change_stat_modifier(defender_state, Stat::DEFENSE, -2);
        }
    }

    // Special Attack
    if (move_has_flag(move, MoveFlag::LOWERS_ATTACKERS_SPECIAL_ATTACK)) {
        change_stat_modifier(attacker_state, Stat::SPECIAL_ATTACK, -2);
    }

    if (move_has_flag(move, MoveFlag::LOWERS_DEFENDER_SPECIAL_ATTACK)) {
        if (defender_state.is_player &&
            move == Move::MistBall
        ) {
            change_stat_modifier(defender_state, Stat::SPECIAL_ATTACK, -1);
        }
        if (move == Move::Memento) {
            change_stat_modifier(defender_state, Stat::SPECIAL_ATTACK, -2);
        }
    }

    // Special Defense
    if (move_has_flag(move, MoveFlag::LOWERS_ATTACKERS_SPECIAL_DEFENSE)) {
        change_stat_modifier(attacker_state, Stat::SPECIAL_DEFENSE, -1);
    }

    if (move_has_flag(move, MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE)) {
        if (move == Move::MetalSound ||
            move == Move::FakeTears
        ) {
            change_stat_modifier(defender_state, Stat::SPECIAL_DEFENSE, -2);
        }
        if (defender_state.is_player &&
            move == Move::SeedFlare
        ) {
            change_stat_modifier(defender_state, Stat::SPECIAL_DEFENSE, -2);
        }
        if (defender_state.is_player &&
            (move == Move::ShadowBall ||
                move == Move::Psychic ||
                move == Move::Acid ||
                move == Move::LusterPurge ||
                move == Move::BugBuzz ||
                move == Move::FocusBlast ||
                move == Move::EnergyBall ||
                move == Move::EarthPower ||
                move == Move::FlashCannon)
        ) {
            change_stat_modifier(defender_state, Stat::SPECIAL_DEFENSE, -1);
        }
    }

    // Speed
    if (move_has_flag(move, MoveFlag::BOOSTS_ATTACKERS_SPEED)) {
        if (move == Move::DragonDance) {
            change_stat_modifier(attacker_state, Stat::SPEED, 1);
        }
    }

    if (move_has_flag(move, MoveFlag::LOWERS_DEFENDER_SPEED)) {
        if (move == Move::IcyWind ||
            move == Move::RockTomb ||
            move == Move::MudShot
        ) {
            change_stat_modifier(defender_state, Stat::SPEED, -1);
        }
        if (defender_state.is_player &&
            (move == Move::Bubblebeam ||
                move == Move::Constrict ||
                move == Move::Bubble ||
                move == Move::StringShot)
        ) {
            change_stat_modifier(defender_state, Stat::SPEED, -1);
        }
        if (defender_state.is_player &&
            (move == Move::CottonSpore ||
                move == Move::ScaryFace)
        ) {
            change_stat_modifier(defender_state, Stat::SPEED, -2);
        }
    }


    // Omni boost
    if (!attacker_state.is_player &&
        move_has_flag(move, MoveFlag::OMNI_BOOSTS_ATTACKER)
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

bool is_player_first(
    const PokemonState& player_state,
    const MoveInfo* player_move,
    const PokemonState& opponent_state,
    const MoveInfo* opponent_move
) {
    const int player_priority = get_move_priority(player_move);
    const int opponent_priority = get_move_priority(opponent_move);
    if (player_priority != opponent_priority) {
        return player_priority > opponent_priority;
    }
    return player_state.current_stats[SPEED_INDEX] >
        opponent_state.current_stats[SPEED_INDEX];
}

/**
 * Gets the damage of a move.
 *
 * @param attacker_state
 * @param attacker_attack
 * @param attacker_move
 * @param defender_state
 * @param defender_defense
 * @param is_after_being_attacked If the move needs to be recalculated for any reason.
 *                       Currently, the opponent field location will result
 *                       in different damage depending on this flag.
 * @return
 */
int get_damage_of_attacker_move(
    const BattleState& battle_state,
    const PokemonState& attacker_state,
    const uint16_t attacker_attack,
    const MoveInfo* attacker_move,
    const PokemonState& defender_state,
    const uint16_t defender_defense,
    const bool is_after_being_attacked
) {
    static std::array<double, LEVEL + 1> cache = [] {
        std::array<double, LEVEL + 1> arr{};
        arr.fill(-1.0);
        return arr;
    }();
    double damage = cache[attacker_state.level];
    if (damage < 0) {
        damage = std::floor(2.0 * attacker_state.level / 5.0) + 2.0;
        cache[attacker_state.level] = damage;
    }

    // Power calculations
    double power = attacker_move->power;
    const auto attacker_move_enum = attacker_move->move;

    const auto status = attacker_state.status;
    if (attacker_move_enum == Move::Eruption ||
        attacker_move_enum == Move::WaterSpout
    ) {
        power = std::max(
            1.0,
            std::floor(
                150.0 *
                attacker_state.current_stats[HEALTH_INDEX] /
                attacker_state.max_health
            )
        );
    } else if (attacker_move_enum == Move::Facade &&
        (status == Status::POISON ||
            status == Status::PARALYZED ||
            status == Status::BURN)
    ) {
        power = power * 2;
    } else if (attacker_move->move == Move::Avalanche &&
        attacker_state.was_hit
    ) {
        power = power * 2;
    } else if (attacker_move->move == Move::WeatherBall) {
        if (battle_state.weather != Weather::CLEAR) {
            power = power * 2;
        }
    } else if (attacker_move->move == Move::Brine &&
        attacker_state.current_stats[HEALTH_INDEX] <=
        attacker_state.max_health / 2
    ) {
        power = power * 2;
    } else if (attacker_move->move == Move::HiddenPower) {
        if (attacker_state.is_player) {
            power = 30;
        } else {
            power = 70;
        }
    }

    // Field location
    const bool attacker_faster = is_player_first(
        attacker_state,
        attacker_move,
        defender_state,
        nullptr
    );
    if (const auto defender_field_location = defender_state.field_location;
        defender_field_location != FieldLocation::ON_FIELD
    ) {
        if (!attacker_faster && !is_after_being_attacked) {
            power = attacker_move->power;
        }
        if (defender_field_location == FieldLocation::IN_AIR) {
            if (move_has_flag(attacker_move_enum,
                              MoveFlag::HITS_DEFENDER_IN_AIR)
            ) {
                if (attacker_move_enum == Move::Gust ||
                    attacker_move_enum == Move::Twister
                ) {
                    power *= 2;
                }
            } else {
                return 0;
            }
        } else if (defender_field_location == FieldLocation::IN_THE_VOID) {
            return 0;
        } else if (defender_field_location == FieldLocation::UNDER_GROUND) {
            if (move_has_flag(attacker_move_enum,
                              MoveFlag::HITS_DEFENDER_UNDER_GROUND)
            ) {
                if (attacker_move_enum == Move::Earthquake ||
                    attacker_move_enum == Move::Magnitude
                ) {
                    power *= 2;
                }
            } else {
                return 0;
            }
        } else if (defender_field_location == FieldLocation::UNDER_WATER) {
            if (move_has_flag(attacker_move_enum,
                              MoveFlag::HITS_DEFENDER_UNDER_WATER)) {
                power *= 2;
            } else {
                return 0;
            }
        }
    }

    damage = std::floor(damage * power * attacker_attack / defender_defense);
    damage = std::floor(damage / 50) + 2;

    // STAB
    auto move_type = attacker_move->type;
    if (attacker_move->move == Move::WeatherBall) {
        switch (battle_state.weather) {
        case Weather::SUN:
            move_type = PokemonType::FIRE;
            break;
        case Weather::RAIN:
            move_type = PokemonType::WATER;
            break;
        case Weather::HAIL:
            move_type = PokemonType::ICE;
            break;
        case Weather::SANDSTORM:
            move_type = PokemonType::ROCK;
            break;
        default:
            move_type = PokemonType::NORMAL;
        }
    }
    if (attacker_state.types[0] == move_type ||
        attacker_state.types[1] == move_type
    ) {
        damage = std::floor(damage * 1.5);
    }

    // Type effectiveness
    auto effectiveness =
        get_effectiveness(defender_state.types, move_type);
    if (attacker_move->move == Move::HiddenPower) {
        if (attacker_state.is_player) {
            if (defender_state.types[1] == PokemonType::COUNT) {
                effectiveness = 0.5;
            } else {
                effectiveness = 0.25;
            }
        } else {
            if (defender_state.types[1] == PokemonType::COUNT) {
                effectiveness = 4;
            } else {
                effectiveness = 2;
            }
        }
    }
    damage = static_cast<int>(damage * effectiveness);

    // Random
    if (attacker_state.is_player) {
        damage = std::floor(damage * 0.85);
    }

    // Burn
    if (status == Status::BURN &&
        attacker_move->category == Category::PHYSICAL &&
        !move_has_flag(attacker_move_enum, MoveFlag::HAS_FIXED_DAMAGE)
    ) {
        damage = std::floor(damage / 2);
    }

    if (attacker_move->move != Move::BrickBreak) {
        if (defender_state.reflect &&
            attacker_move->category == Category::PHYSICAL
        ) {
            damage = std::floor(damage / 2);
        } else if (defender_state.light_screen &&
            attacker_move->category == Category::SPECIAL
        ) {
            damage = std::floor(damage / 2);
        }
    }
    return static_cast<int>(damage);
}

BestMove get_best_move_against_defender(
    const BattleState& battle_state,
    const std::vector<const MoveInfo*>& attacker_moves,
    PokemonState& attacker_state,
    const PokemonState& defender_state,
    const bool is_after_being_attacked,
    const BestMove& last_picked_move
) {
    if (attacker_state.field_location != FieldLocation::ON_FIELD ||
        attacker_state.multi_turn_move_counter > 0
    ) {
        return last_picked_move;
    }

    BestMove best_move{};
    if (attacker_state.recharging) {
        attacker_state.recharging = false;
        return best_move;
    }

    const auto attack =
        attacker_state.current_stats[ATTACK_INDEX];
    const auto defense =
        defender_state.current_stats[DEFENSE_INDEX];
    const auto special_attack =
        attacker_state.current_stats[SPECIAL_ATTACK_INDEX];
    const auto special_defense =
        defender_state.current_stats[SPECIAL_DEFENSE_INDEX];
    for (const auto& move : attacker_moves) {
        const auto category = move->category;
        if (category == Category::STATUS &&
            move->move != Move::DreamEater
        ) {
            continue;
        }
        const bool is_special = category == Category::SPECIAL;
        const auto attack_used = is_special ? special_attack : attack;
        const auto defense_used = is_special ? special_defense : defense;
        const auto damage =
            get_damage_of_attacker_move(
                battle_state,
                attacker_state,
                attack_used,
                move,
                defender_state,
                defense_used,
                is_after_being_attacked
            );
        if (move_has_flag(move->move, MoveFlag::REQUIRES_CHARGING_TURN) &&
            damage > defender_state.current_stats[HEALTH_INDEX]
        ) {
            // TODO figure out if worth using
        }
        if (damage > best_move.damage) {
            best_move.damage = damage;
            best_move.move = move;
        }
    }

    // Check if it is better to use a status move
    const auto hits =
        defender_state.current_stats[HEALTH_INDEX] / best_move.damage;
    for (const auto& move : attacker_moves) {
        if (move_has_flag(move->move, MoveFlag::BOOSTS_ATTACKER_STAT)) {
            // TODO is_first check
            if (best_move.move->category == Category::PHYSICAL) {
                if (move->move == Move::SwordsDance) {
                    if (hits > 3) {
                        best_move.move = move;
                        best_move.damage = 0;
                    }
                } else if (move->move == Move::Meditate ||
                    move->move == Move::Sharpen ||
                    move->move == Move::Howl ||
                    move->move == Move::BulkUp ||
                    move->move == Move::DragonDance
                ) {
                    if (hits > 6) {
                        best_move.move = move;
                        best_move.damage = 0;
                    }
                }
            }
            if (best_move.move->category == Category::SPECIAL) {
                if (move->move == Move::NastyPlot) {
                    if (hits > 3) {
                        best_move.move = move;
                        best_move.damage = 0;
                    }
                }
                if (move->move == Move::Growth) {
                    if (hits > 6) {
                        best_move.move = move;
                        best_move.damage = 0;
                    }
                }
            }
            // Agility, double team, minimize, bulk up are ignored
            // TODO Also let opponent see if defending is worth it
            // flags[static_cast<int>(Move::Harden)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
            // flags[static_cast<int>(Move::Withdraw)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
            // flags[static_cast<int>(Move::Amnesia)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
            // flags[static_cast<int>(Move::AcidArmor)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
            // flags[static_cast<int>(Move::TailGlow)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
            // flags[static_cast<int>(Move::CosmicPower)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
            // flags[static_cast<int>(Move::IronDefense)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
            // flags[static_cast<int>(Move::CalmMind)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
            // flags[static_cast<int>(Move::RockPolish)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
            // flags[static_cast<int>(Move::NastyPlot)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
            // flags[static_cast<int>(Move::DefendOrder)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
        }
    }
    return best_move;
}

void execute_move(
    BattleState& battle_state,
    PokemonState& attacker_state,
    BestMove& attacker_move_info,
    PokemonState& defender_state,
    BestMove& defender_move
) {
    if (attacker_move_info.move == nullptr) {
        return;
    }

    const auto attacker_move = attacker_move_info.move->move;

    if (attacker_move == Move::BrickBreak) {
        defender_state.reflect = false;
        defender_state.light_screen = false;
        auto [move, damage] =
            get_best_move_against_defender(
                battle_state,
                {attacker_move_info.move},
                attacker_state,
                defender_state,
                attacker_state.was_hit,
                attacker_move_info
            );
        attacker_move_info.damage = damage;
    }

    // Sucker punch
    if (attacker_move == Move::SuckerPunch &&
        defender_move.move != nullptr &&
        defender_move.move->category == Category::STATUS
    ) {
        return;
    }

    // Charge move
    const bool requires_charging =
        move_has_flag(attacker_move, MoveFlag::REQUIRES_CHARGING_TURN);
    if (requires_charging &&
        !attacker_state.charging
    ) {
        if (attacker_state.item == Item::PowerHerb) {
            attacker_state.item = Item::None;
        } else {
            attacker_state.charging = true;
            return;
        }
    }
    if (attacker_state.charging) {
        assert(requires_charging);
        attacker_state.charging = false;
    }
    if (attacker_move == Move::FocusPunch &&
        attacker_state.was_hit
    ) {
        return;
    }

    // Off field move
    bool attacker_vanished = false;
    if (const FieldLocation on_field = attacker_state.field_location;
        on_field == FieldLocation::ON_FIELD
    ) {
        if (move_has_flag(attacker_move, MoveFlag::GOES_INTO_AIR)) {
            attacker_state.field_location = FieldLocation::IN_AIR;
            attacker_vanished = true;
        } else if (move_has_flag(attacker_move, MoveFlag::GOES_UNDER_GROUND)) {
            attacker_state.field_location = FieldLocation::UNDER_GROUND;
            attacker_vanished = true;
        } else if (move_has_flag(attacker_move, MoveFlag::GOES_UNDER_WATER)) {
            attacker_state.field_location = FieldLocation::UNDER_WATER;
            attacker_vanished = true;
        } else if (move_has_flag(attacker_move, MoveFlag::GOES_INTO_VOID)) {
            attacker_state.field_location = FieldLocation::IN_THE_VOID;
            attacker_vanished = true;
        }
    }
    if (!attacker_vanished &&
        attacker_state.field_location != FieldLocation::ON_FIELD
    ) {
        attacker_state.field_location = FieldLocation::ON_FIELD;
    }

    if (attacker_state.field_location != FieldLocation::ON_FIELD) {
        // Defender attack may miss or do double damage
        if (defender_move.move != nullptr) {
            auto [move, damage] =
                get_best_move_against_defender(
                    battle_state,
                    {defender_move.move},
                    defender_state,
                    attacker_state,
                    true,
                    defender_move
                );
            defender_move.damage = damage;
        }
    }

    if (attacker_state.was_hit &&
        attacker_move_info.move != nullptr &&
        attacker_move_info.move->move == Move::Avalanche
    ) {
        auto [move, damage] =
            get_best_move_against_defender(
                battle_state,
                {attacker_move_info.move},
                attacker_state,
                defender_state,
                true,
                attacker_move_info
            );
        defender_move.damage = damage;
    }

    // Do the damage
    if (attacker_state.field_location == FieldLocation::ON_FIELD) {
        defender_state.current_stats[HEALTH_INDEX] -= attacker_move_info.damage;
        if (attacker_move_info.damage > 0) {
            defender_state.was_hit = true;
            if (move_has_flag(attacker_move, MoveFlag::CONTINUES)) {
                if (attacker_state.is_player) {
                    defender_state.trapped_counter = 2;
                } else {
                    defender_state.trapped_counter = 5;
                }
            }

            if (defender_move.move != nullptr) {
                const auto attacker_movecategory =
                    attacker_move_info.move->category;
                if (defender_move.move->move == Move::Counter) {
                    if (attacker_movecategory == Category::PHYSICAL) {
                        defender_move.damage = attacker_move_info.damage * 2;
                    }
                }
                if (defender_move.move->move == Move::MirrorCoat) {
                    if (attacker_movecategory == Category::SPECIAL) {
                        defender_move.damage = attacker_move_info.damage * 2;
                    }
                }
            }
        }
        apply_post_attack_effects(
            battle_state,
            attacker_state,
            attacker_move_info,
            defender_state
        );
    }
}

bool is_battle_over(
    const PokemonState& player_state,
    const PokemonState& opponent_state
) {
    return player_state.current_stats[HEALTH_INDEX] <= 0 ||
        opponent_state.current_stats[HEALTH_INDEX] <= 0;
}

bool battle(const CustomPokemon& player, const CustomPokemon& opponent) {
    BattleState battle_state{};
    auto player_state = PokemonState{
        .is_player = true,
        .pokemon = player
    };
    auto opponent_state = PokemonState{
        .is_player = false,
        .pokemon = opponent
    };

    BestMove player_move;
    BestMove opponent_move;
    while (player_state.current_stats[HEALTH_INDEX] > 0 &&
        opponent_state.current_stats[HEALTH_INDEX] > 0
    ) {
        player_move = get_best_move_against_defender(
            battle_state,
            player.moves,
            player_state,
            opponent_state,
            false,
            player_move
        );
        opponent_move = get_best_move_against_defender(
            battle_state,
            opponent.moves,
            opponent_state,
            player_state,
            false,
            opponent_move
        );
        const bool player_goes_first = is_player_first(
            player_state,
            player_move.move,
            opponent_state,
            opponent_move.move
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
                move != Move::WaterSpout &&
                move != Move::SuckerPunch &&
                move != Move::BugBite &&
                move != Move::TriAttack &&
                move != Move::FocusPunch &&
                move != Move::DreamEater &&
                move != Move::Facade &&
                move != Move::UTurn &&
                move != Move::Avalanche &&
                move != Move::Counter &&
                move != Move::MirrorCoat &&
                move != Move::WeatherBall &&
                move != Move::Brine &&
                move != Move::Chatter &&
                move != Move::Uproar &&
                move != Move::HiddenPower &&
                move != Move::BrickBreak &&
                (move_has_flag(move, MoveFlag::BOOSTS_ATTACKER_STAT) ||
                    move_has_flag(move, MoveFlag::BOOSTS_ATTACKERS_DEFENSE) ||
                    move_has_flag(
                        move, MoveFlag::BOOSTS_ATTACKERS_SPECIAL_ATTACK) ||
                    move_has_flag(
                        move, MoveFlag::BOOSTS_ATTACKERS_SPECIAL_DEFENSE) ||
                    move_has_flag(move, MoveFlag::BOOSTS_ATTACKERS_SPEED) ||
                    move_has_flag(move, MoveFlag::BREAKS_PROTECT) ||
                    move_has_flag(move, MoveFlag::BYPASSES_PROTECT) ||
                    move_has_flag(
                        move, MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT) ||
                    move_has_flag(move, MoveFlag::CAN_BE_SNATCHED) ||
                    move_has_flag(move, MoveFlag::CHANGES_WEATHER) ||
                    move_has_flag(move, MoveFlag::HAS_FIXED_DAMAGE) ||
                    move_has_flag(move, MoveFlag::LOWERS_ATTACKERS_ATTACK) ||
                    move_has_flag(move, MoveFlag::LOWERS_ATTACKERS_SPEED) ||
                    move_has_flag(move, MoveFlag::LOWERS_DEFENDER_ATTACK) ||
                    move_has_flag(move, MoveFlag::MAKES_ATTACKER_FAINT) ||
                    move_has_flag(move, MoveFlag::RAISES_DEFENDER_ATTACK) ||
                    move_has_flag(
                        move, MoveFlag::RAISES_DEFENDER_SPECIAL_ATTACK) ||
                    move_has_flag(move, MoveFlag::HITS_MULTIPLE_TIMES) ||
                    move_has_flag(move, MoveFlag::IS_OTHER) ||
                    move_has_flag(move, MoveFlag::PROTECTS_USER))
            ) {
                printf("");
            }
        }

        player_state.was_hit = false;
        opponent_state.was_hit = false;
        if (player_goes_first) {
            execute_move(
                battle_state,
                player_state,
                player_move,
                opponent_state,
                opponent_move
            );
            if (is_battle_over(player_state, opponent_state)) {
                break;
            }
            execute_move(
                battle_state,
                opponent_state,
                opponent_move,
                player_state,
                player_move
            );
            if (is_battle_over(player_state, opponent_state)) {
                break;
            }
        } else {
            execute_move(
                battle_state,
                opponent_state,
                opponent_move,
                player_state,
                player_move
            );
            if (is_battle_over(player_state, opponent_state)) {
                break;
            }
            execute_move(
                battle_state,
                player_state,
                player_move,
                opponent_state,
                opponent_move
            );
            if (is_battle_over(player_state, opponent_state)) {
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
            !opponent_state.recharging &&
            player_state.field_location == FieldLocation::ON_FIELD &&
            opponent_state.field_location == FieldLocation::ON_FIELD &&
            !player_state.charging &&
            !opponent_state.charging
        ) {
            break;
        }
    }
    if (player_state.current_stats[HEALTH_INDEX] > 0 &&
        opponent_state.current_stats[HEALTH_INDEX] < 0 &&
        opponent_state.ability == Ability::Aftermath &&
        player_state.ability != Ability::Damp
    ) {
        player_state.current_stats[HEALTH_INDEX] -= player_state.max_health / 4;
    }

    if (player_state.current_stats[HEALTH_INDEX] > 0) {
        return true;
    }
    return false;
}
