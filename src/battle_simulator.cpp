#include "battle_simulator.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <set>

#include "config.h"
#include "custom_pokemon.h"
#include "Items.h"
#include "SimulationState.h"
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

/*
Ignored:
BrightPowder,
FocusBand,
KingsRock,
LaxIncense,
LightClay,
RazorClaw,
RazorFang,
ScopeLens,
Stick,
ZoomLens,
 */

// 20% boost in power
static const std::unordered_map<Item, PokemonType> ITEM_TO_TYPE = {
    {Item::BlackBelt, PokemonType::FIGHTING},
    {Item::BlackGlasses, PokemonType::DARK},
    {Item::Charcoal, PokemonType::FIRE},
    {Item::DragonFang, PokemonType::DRAGON},
    {Item::HardStone, PokemonType::ROCK},
    {Item::Magnet, PokemonType::ELECTRIC},
    {Item::MetalCoat, PokemonType::STEEL},
    {Item::MiracleSeed, PokemonType::GRASS},
    {Item::MysticWater, PokemonType::WATER},
    {Item::NeverMeltIce, PokemonType::ICE},
    {Item::PoisonBarb, PokemonType::POISON},
    {Item::SharpBeak, PokemonType::FLYING},
    {Item::SilkScarf, PokemonType::NORMAL},
    {Item::SilverPowder, PokemonType::BUG},
    {Item::SoftSand, PokemonType::GROUND},
    {Item::SpellTag, PokemonType::GHOST},
    {Item::TwistedSpoon, PokemonType::PSYCHIC},
};

/*
    BigRoot, TODO 30% more HP from Leech Seed, Ingrain and Aqua Ring
    HeatRock, TODO sun is eight turns instead of five turns
    IcyRock, TODO hail is eight turns instead of five turns
    IronBall, TODO Even if the holder has Klutz or is affected by Embargo,
                   its Speed is still halved by the Iron Ball.
                   Due to being grounded, the holder becomes susceptible
                   to Arena Trap; the Spikes, and Toxic Spikes,
                   even if it is Flying-type, has the Ability Levitate,
                   or is under the effect of Telekinesis or Magnet Rise.
                   An Iron Ball does not prevent Magnet Rise or Telekinesis
                   from being used successfully.
    QuickPowder, TODO doubles ditto's speed till it transforms.
    RockyHelmet, TODO multi-hit moves cause damage for each hit
    ShellBell, TODO If the holder uses a multistrike move,
                    it recovers HP after the last strike,
                    considering the damage from all of the strikes at once.
                    The Shell Bell will not activate if the move only hits
                    a Pokémon's substitute. Ignores heal block
    SmoothRock, TODO sandstorm is eight turns instead of five turns
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

int apply_speed_items(const PokemonState& state, int speed) {
    if (const auto item = state.get_item();
        item == Item::ChoiceScarf
    ) {
        speed = std::floor(speed * 1.5);
    } else if (item == Item::IronBall) {
        speed = speed / 2;
    }
    return speed;
}

bool is_player_first(
    const PokemonState& player_state,
    const MoveInfo* player_move,
    const PokemonState& opponent_state,
    const MoveInfo* opponent_move
) {
    if (opponent_state.get_item() == Item::QuickClaw) {
        return false;
    }
    const int player_priority = get_move_priority(player_move);
    const int opponent_priority = get_move_priority(opponent_move);
    if (player_priority != opponent_priority) {
        return player_priority > opponent_priority;
    }
    int player_speed = player_state.get_speed();
    player_speed = apply_speed_items(player_state, player_speed);
    int opponent_speed = opponent_state.get_speed();
    opponent_speed = apply_speed_items(opponent_state, opponent_speed);
    return player_speed > opponent_speed;
}

void apply_end_of_turn_effects(
    PokemonState& state
) {
    /* Order of effects
    Trapping moves
    Leech Seed
    Poison/burn damage
    Abilities
    Weather damage/healing
    Status healing from abilities
    Aqua Ring/ingrain recovery
    Leftovers/Black Sludge recovery
    */
    if (state.get_item() == Item::BlackSludge &&
        (state.types[0] == PokemonType::POISON ||
            state.types[1] == PokemonType::POISON)
    ) {
        state.heal(std::max(1, state.max_health / 16));
    }
    if (state.get_ability() != Ability::MagicGuard) {
        if (state.trapped_counter > 0) {
            apply_damage(state, state.max_health / 8);
        }
        if (state.get_status() == Status::BADLY_POISONED) {
            state.turns_badly_poisoned =
                std::min(15, state.turns_badly_poisoned + 1);
            apply_damage(
                state,
                state.turns_badly_poisoned * (state.max_health / 16)
            );
        }
        if (state.get_status() == Status::BURN ||
            state.get_status() == Status::POISON
        ) {
            apply_damage(state, state.max_health / 8);
        }
        if (state.get_item() == Item::StickyBarb) {
            apply_damage(state, state.max_health / 8);
        }
        if (state.get_item() == Item::BlackSludge &&
            (state.types[0] != PokemonType::POISON &&
                state.types[1] != PokemonType::POISON)
        ) {
            apply_damage(state, std::max(1, state.max_health / 8));
        }
    }
    if (state.get_item() == Item::FlameOrb &&
        (state.types[0] != PokemonType::FIRE &&
            state.types[1] != PokemonType::FIRE)
    ) {
        try_apply_status(state, Status::BURN);
    }
    if (state.trapped_counter > 0) {
        state.trapped_counter--;
    }
    if (state.get_item() == Item::Leftovers) {
        heal(state, std::max(1, state.max_health / 16));
    }
}

void apply_end_of_turn_effects(
    PokemonState& player_state,
    PokemonState& opponent_state
) {
    apply_end_of_turn_effects(player_state);
    apply_end_of_turn_effects(opponent_state);
}

bool move_has_flag(const Move move, const MoveFlag move_flag) {
    return MOVE_FLAGS[static_cast<int>(move)].test(
        static_cast<size_t>(move_flag)
    );
}

void clear_negative_stat_changes(PokemonState& pokemon_state) {
    int i = 0;
    for (const auto stat_stage : pokemon_state.stat_stages) {
        if (stat_stage < 0) {
            change_stat_modifier(
                pokemon_state,
                static_cast<Stat>(i),
                -stat_stage
            );
        }
        i++;
    }
}

void apply_post_move_effects(
    BattleState& battle_state,
    PokemonState& attacker_state,
    const BestMove& attacker_move,
    PokemonState& defender_state,
    const BestMove& defender_move
) {
    if (attacker_move.move == nullptr) {
        return;
    }
    const auto move = attacker_move.move->move;

    // Protect
    if (move_has_flag(move, MoveFlag::BREAKS_PROTECT)) {
        defender_state.is_protected = false;
    }

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

            if (attacker_state.get_item() == Item::BigRoot) {
                health_gained = std::floor(attacker_move.damage * 0.65);
            }

            health_gained = std::max(1, health_gained);
            if (defender_state.get_ability() == Ability::LiquidOoze) {
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
        if (attacker_state.get_item() == Item::ShellBell) {
            heal(
                attacker_state,
                attacker_move.damage / 8
            );
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
        if (defender_state.get_status() == Status::SLEEP) {
            defender_state.get_status() = Status::NONE;
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
        if (BERRIES.contains(defender_state.get_item()) &&
            defender_state.get_ability() != Ability::StickyHold
        ) {
            const Item backup = attacker_state.get_item();
            attacker_state.get_item() = defender_state.get_item();
            defender_state.get_item() = Item::None;
            try_apply_berry(attacker_state, true);
            attacker_state.get_item() = backup;
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
            apply_damage(attacker_state, attacker_move.damage / 2);
        } else if (
            move == Move::BraveBird ||
            move == Move::DoubleEdge ||
            move == Move::FlareBlitz ||
            move == Move::VoltTackle ||
            move == Move::WoodHammer
        ) {
            apply_damage(attacker_state, attacker_move.damage / 3);
        } else if (
            move == Move::Submission ||
            move == Move::TakeDown
        ) {
            apply_damage(attacker_state, attacker_move.damage / 4);
        } else if (move == Move::BellyDrum) {
            apply_damage(
                attacker_state,
                attacker_state.max_health / 2
            );
            change_stat_modifier(attacker_state, Stat::ATTACK, 6);
        }
    }

    // Contact moves
    if (move_has_flag(move, MoveFlag::MAKES_CONTACT)) {
        const auto defender_ability = defender_state.get_ability();
        if (attacker_state.is_player) {
            if (defender_state.get_status() == Status::NONE) {
                if (defender_ability == Ability::EffectSpore ||
                    defender_ability == Ability::PoisonPoint
                ) {
                    defender_state.get_status() = Status::POISON;
                } else if (defender_ability == Ability::FlameBody) {
                    defender_state.get_status() = Status::BURN;
                }
            }
        }
        if (defender_ability == Ability::RoughSkin) {
            apply_damage(
                attacker_state,
                attacker_state.max_health / 8
            );
        }
        if (defender_state.get_item() == Item::RockyHelmet) {
            apply_damage(
                attacker_state,
                attacker_state.max_health / 6
            );
        }
        if (attacker_state.get_item() == Item::None &&
            defender_state.get_item() == Item::StickyBarb
        ) {
            attacker_state.get_item() = Item::StickyBarb;
            defender_state.get_item() = Item::None;
        }
    }

    // Life orb
    if (attacker_state.get_ability() != Ability::MagicGuard &&
        attacker_state.get_item() == Item::LifeOrb &&
        attacker_move.damage > 0 &&
        move_has_flag(attacker_move.move->move, MoveFlag::HAS_POWER)
    ) {
        apply_damage(
            attacker_state,
            std::max(
                1,
                attacker_state.current_stats[HEALTH_INDEX] / 10
            )
        );
    }

    // Burn application
    const bool is_fling = move == Move::Fling;
    if (defender_state.types[0] != PokemonType::FIRE &&
        defender_state.types[1] != PokemonType::FIRE
    ) {
        const bool flung_flame_orb =
            (is_fling && attacker_state.get_item() == Item::FlameOrb);
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

    // Poison application
    if (defender_state.types[0] != PokemonType::POISON &&
        defender_state.types[1] != PokemonType::POISON &&
        defender_state.types[0] != PokemonType::STEEL &&
        defender_state.types[1] != PokemonType::STEEL
    ) {
        const bool flung_poison_barb =
            (is_fling && attacker_state.get_item() == Item::PoisonBarb);
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

    // Bad poison application
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

    if (move_has_flag(move, MoveFlag::LOWERS_ATTACKERS_ATTACK)) {
        change_stat_modifier(attacker_state, Stat::ATTACK, -1);
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

    if (move_has_flag(move, MoveFlag::LOWERS_ATTACKERS_SPEED)) {
        change_stat_modifier(attacker_state, Stat::SPEED, -1);
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
        if (attacker_state.get_item() == Item::KingsRock ||
            attacker_state.get_item() == Item::RazorFang
        ) {
            defender_state.flinched = true;
        } else if (attacker_state.get_item() == Item::LightBall) {
            try_apply_status(defender_state, Status::PARALYZED);
        } else if (attacker_state.get_item() == Item::MentalHerb) {
            defender_state.infatuated = true;
        } else if (attacker_state.get_item() == Item::WhiteHerb) {
            clear_negative_stat_changes(defender_state);
        }
        attacker_state.get_item() = Item::None;
    }
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

    const auto status = attacker_state.get_status();
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
    } else if ((attacker_move->move == Move::Avalanche ||
            attacker_move->move == Move::Revenge
        ) && attacker_state.was_hit
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
    if (attacker_state.get_item() == Item::Metronome) {
        const double multiplier = 1.0 + std::max(
            0.0,
            0.1 * attacker_state.metronome
        );
        power = std::floor(power * multiplier);
    } else if (attacker_state.get_item() == Item::MuscleBand &&
        attacker_move->category == Category::PHYSICAL
    ) {
        power = std::floor(power * 1.1);
    } else if (attacker_state.get_item() == Item::WiseGlasses &&
        attacker_move->category == Category::SPECIAL
    ) {
        power = std::floor(power * 1.1);
    } else if (attacker_state.pokemon.name == Pokemon::Pikachu &&
        attacker_state.get_item() == Item::LightBall
    ) {
        power = power * 2;
    }

    // Field location
    const bool attacker_faster = is_player_first(
        attacker_state,
        attacker_move,
        defender_state,
        nullptr
    );
    if (const auto defender_field_location = defender_state.field_location;
        defender_field_location != FieldLocation::ON_FIELD && attacker_faster
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
    auto defender_types = defender_state.types;
    if (defender_state.get_item() == Item::IronBall &&
        attacker_move->type == PokemonType::GROUND
    ) {
        if (defender_types[0] == PokemonType::FLYING) {
            defender_types = {defender_types[1]};
        } else if (defender_types[1] == PokemonType::FLYING) {
            defender_types = {defender_types[0]};
        }
    }
    auto effectiveness =
        get_effectiveness(defender_types, move_type);
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
    if (attacker_state.get_item() == Item::ExpertBelt && effectiveness >= 2) {
        damage = std::floor(damage * 1.2);
    }

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
    BestMove& last_picked_move
) {
    if (!is_after_being_attacked &&
        (attacker_state.field_location != FieldLocation::ON_FIELD ||
            attacker_state.multi_turn_move_counter > 0)
    ) {
        return last_picked_move;
    }

    const bool attacker_faster = is_player_first(
        attacker_state,
        nullptr,
        defender_state,
        nullptr
    );

    BestMove best_move{};
    if (!is_after_being_attacked && attacker_state.recharging) {
        attacker_state.recharging = false;
        return best_move;
    }

    auto attack =
        attacker_state.current_stats[ATTACK_INDEX];
    if (attacker_state.get_item() == Item::ChoiceBand) {
        attack = std::floor(attack * 1.5);
    }
    const auto defense =
        defender_state.current_stats[DEFENSE_INDEX];
    auto special_attack =
        attacker_state.current_stats[SPECIAL_ATTACK_INDEX];
    if (attacker_state.get_item() == Item::ChoiceSpecs) {
        special_attack = std::floor(special_attack * 1.5);
    }
    const auto special_defense =
        defender_state.current_stats[SPECIAL_DEFENSE_INDEX];

    // TODO add priority effects
    //      should not be using lower priority moves if it means getting KO'd
    for (const auto& move : attacker_moves) {
        if (attacker_state.is_choiced &&
            move->move != last_picked_move.move->move
        ) {
            continue;
        }
        const auto category = move->category;
        if (category == Category::STATUS &&
            move->move != Move::DreamEater
        ) {
            continue;
        }
        const bool is_special = category == Category::SPECIAL;
        const auto attack_used = is_special ? special_attack : attack;
        const auto defense_used = is_special ? special_defense : defense;
        auto damage =
            get_damage_of_attacker_move(
                battle_state,
                attacker_state,
                attack_used,
                move,
                defender_state,
                defense_used,
                is_after_being_attacked
            );
        if (attacker_state.get_item() == Item::LifeOrb) {
            damage = std::floor(damage * 1.3);
        }
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
    if (best_move.damage != 0 && !attacker_state.is_choiced) {
        const auto hits =
            defender_state.current_stats[HEALTH_INDEX] / best_move.damage;
        for (const auto& move : attacker_moves) {
            if (move_has_flag(move->move, MoveFlag::BOOSTS_ATTACKER_STAT)) {
                // TODO is_first check
                if (best_move.move->category == Category::PHYSICAL) {
                    if (attacker_state.current_stats[ATTACK_INDEX] < 6) {
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
                }
                if (best_move.move->category == Category::SPECIAL) {
                    if (attacker_state.current_stats[SPECIAL_ATTACK_INDEX] <
                        6
                    ) {
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
                }
                // Agility, double team, minimize, bulk up, rock polish
                // are ignored
                // TODO check if defending is worth it
                // flags[static_cast<int>(Move::Harden)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
                // flags[static_cast<int>(Move::Withdraw)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
                // flags[static_cast<int>(Move::Amnesia)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
                // flags[static_cast<int>(Move::AcidArmor)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
                // flags[static_cast<int>(Move::TailGlow)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
                // flags[static_cast<int>(Move::CosmicPower)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
                // flags[static_cast<int>(Move::IronDefense)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
                // flags[static_cast<int>(Move::CalmMind)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
                // flags[static_cast<int>(Move::DefendOrder)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
            }
        }
    }
    if (CHOICE_ITEMS.contains(attacker_state.get_item()) &&
        !attacker_state.is_choiced &&
        best_move.move != nullptr
    ) {
        attacker_state.is_choiced = true;
        return best_move;
    }
    if (attacker_state.is_choiced) {
        assert(last_picked_move.move->move == best_move.move->move);
        last_picked_move.damage = best_move.damage;
        return last_picked_move;
    }

    if (defender_state.field_location != FieldLocation::ON_FIELD &&
        attacker_faster &&
        best_move.move == nullptr
    ) {
        // For metronome since no move was picked
        return last_picked_move;
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

    if (!battle_state.mid_turn &&
        attacker_move == Move::Snatch &&
        move_has_flag(defender_move.move->move, MoveFlag::CAN_BE_SNATCHED)
    ) {
        attacker_move_info.move = defender_move.move;
        defender_move.move = nullptr;
        defender_move.damage = 0;
        auto [move, damage] =
            get_best_move_against_defender(
                battle_state,
                {attacker_move_info.move},
                attacker_state,
                defender_state,
                false,
                defender_move
            );
        attacker_move_info.damage = damage;
    }

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
        if (attacker_state.get_item() == Item::PowerHerb) {
            attacker_state.get_item() = Item::None;
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
        const bool goes_into_air =
            move_has_flag(attacker_move, MoveFlag::GOES_INTO_AIR);
        const bool goes_under_ground =
            move_has_flag(attacker_move, MoveFlag::GOES_UNDER_GROUND);
        const bool goes_under_water =
            move_has_flag(attacker_move, MoveFlag::GOES_UNDER_WATER);
        const bool goes_into_void =
            move_has_flag(attacker_move, MoveFlag::GOES_INTO_VOID);
        if (goes_into_air ||
            goes_under_ground ||
            goes_under_water ||
            goes_into_void
        ) {
            if (attacker_state.get_item() == Item::PowerHerb) {
                attacker_state.get_item() = Item::None;
            } else {
                if (goes_into_air) {
                    attacker_state.field_location = FieldLocation::IN_AIR;
                    attacker_vanished = true;
                } else if (goes_under_ground) {
                    attacker_state.field_location = FieldLocation::UNDER_GROUND;
                    attacker_vanished = true;
                } else if (goes_under_water) {
                    attacker_state.field_location = FieldLocation::UNDER_WATER;
                    attacker_vanished = true;
                } else if (goes_into_void) {
                    attacker_state.field_location = FieldLocation::IN_THE_VOID;
                    attacker_vanished = true;
                }
            }
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
        attacker_move_info.damage = damage;
    }

    // Do the damage
    if (attacker_state.field_location == FieldLocation::ON_FIELD) {
        if (move_has_flag(attacker_move, MoveFlag::BYPASSES_PROTECT) ||
            !defender_state.is_protected
        ) {
            const bool defender_had_max_hp =
                defender_state.max_health ==
                defender_state.current_stats[HEALTH_INDEX];
            apply_damage(defender_state, attacker_move_info.damage);
            if (defender_had_max_hp &&
                defender_state.current_stats[HEALTH_INDEX] <= 0 &&
                defender_state.get_item() == Item::FocusSash
            ) {
                defender_state.current_stats[HEALTH_INDEX] = 1;
                attacker_state.get_item() = Item::None;
            }
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
                    const auto attacker_move_category =
                        attacker_move_info.move->category;
                    if (defender_move.move->move == Move::Counter) {
                        if (attacker_move_category == Category::PHYSICAL) {
                            defender_move.damage =
                                attacker_move_info.damage * 2;
                        }
                    } else if (defender_move.move->move == Move::MirrorCoat) {
                        if (attacker_move_category == Category::SPECIAL) {
                            defender_move.damage =
                                attacker_move_info.damage * 2;
                        }
                    } else {
                        // Several moves do more damage after getting hit
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
            }
            apply_post_move_effects(
                battle_state,
                attacker_state,
                attacker_move_info,
                defender_state,
                defender_move
            );
        }
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
        BestMove last_move = player_move;
        player_move = get_best_move_against_defender(
            battle_state,
            player.moves,
            player_state,
            opponent_state,
            false,
            player_move
        );
        if (last_move.move != nullptr &&

            last_move.move->move == player_move.move->move
        ) {
            player_state.metronome++;
        } else {
            player_state.metronome = 0;
        }
        last_move = opponent_move;
        opponent_move = get_best_move_against_defender(
            battle_state,
            opponent.moves,
            opponent_state,
            player_state,
            false,
            opponent_move
        );
        if (last_move.move != nullptr &&
            last_move.move->move == opponent_move.move->move
        ) {
            opponent_state.metronome++;
        } else {
            opponent_state.metronome = 0;
        }
        const bool player_goes_first = is_player_first(
            player_state,
            player_move.move,
            opponent_state,
            opponent_move.move
        );

        // For debugging unimplemented attacks
        std::array<Move, 2> moves = {};
        // if (player_move.move) {
        //     moves[0] = player_move.move->move;
        // } else {
        //     moves[0] = Move::Count;
        // }
        // if (opponent_move.move) {
        //     moves[1] = opponent_move.move->move;
        // } else {
        //     moves[1] = Move::Count;
        // }
        // for (const auto move : moves) {
        //     if (move != Move::Count &&
        //         move != Move::Eruption &&
        //         move != Move::WaterSpout &&
        //         move != Move::SuckerPunch &&
        //         move != Move::BugBite &&
        //         move != Move::TriAttack &&
        //         move != Move::FocusPunch &&
        //         move != Move::DreamEater &&
        //         move != Move::Facade &&
        //         move != Move::UTurn &&
        //         move != Move::Avalanche &&
        //         move != Move::Counter &&
        //         move != Move::MirrorCoat &&
        //         move != Move::WeatherBall &&
        //         move != Move::Brine &&
        //         move != Move::Chatter &&
        //         move != Move::Uproar &&
        //         move != Move::HiddenPower &&
        //         move != Move::BrickBreak &&
        //         move != Move::Revenge &&
        //         move != Move::LastResort &&
        //         (move_has_flag(move, MoveFlag::BOOSTS_ATTACKERS_DEFENSE) ||
        //             move_has_flag(
        //                 move, MoveFlag::BOOSTS_ATTACKERS_SPECIAL_ATTACK) ||
        //             move_has_flag(
        //                 move, MoveFlag::BOOSTS_ATTACKERS_SPECIAL_DEFENSE) ||
        //             move_has_flag(move, MoveFlag::BOOSTS_ATTACKERS_SPEED) ||
        //             move_has_flag(
        //                 move, MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT) ||
        //             move_has_flag(move, MoveFlag::CHANGES_WEATHER) ||
        //             move_has_flag(move, MoveFlag::HAS_FIXED_DAMAGE) ||
        //             move_has_flag(move, MoveFlag::LOWERS_DEFENDER_ATTACK) ||
        //             move_has_flag(move, MoveFlag::MAKES_ATTACKER_FAINT) ||
        //             move_has_flag(move, MoveFlag::RAISES_DEFENDER_ATTACK) ||
        //             move_has_flag(
        //                 move, MoveFlag::RAISES_DEFENDER_SPECIAL_ATTACK) ||
        //             move_has_flag(move, MoveFlag::HITS_MULTIPLE_TIMES) ||
        //             move_has_flag(move, MoveFlag::IS_OTHER) ||
        //             move_has_flag(move, MoveFlag::PROTECTS_USER))
        //     ) {
        //         printf("");
        //     }
        // }

        battle_state.mid_turn = false;
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
            battle_state.mid_turn = true;
            if (!opponent_state.flinched) {
                execute_move(
                    battle_state,
                    opponent_state,
                    opponent_move,
                    player_state,
                    player_move
                );
            } else {
                opponent_state.flinched = false;
            }
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
            battle_state.mid_turn = true;
            if (!player_state.flinched) {
                execute_move(
                    battle_state,
                    player_state,
                    player_move,
                    opponent_state,
                    opponent_move
                );
            } else {
                player_state.flinched = false;
            }
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
        opponent_state.get_ability() == Ability::Aftermath &&
        player_state.get_ability() != Ability::Damp
    ) {
        apply_damage(player_state, player_state.max_health / 4);
    }

    if (player_state.current_stats[HEALTH_INDEX] > 0) {
        return true;
    }
    return false;
}
