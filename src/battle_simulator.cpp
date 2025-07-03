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

void apply_end_of_turn_effects(
    PokemonState& player_state,
    PokemonState& opponent_state
) {
    player_state.apply_end_of_turn_effects();
    opponent_state.apply_end_of_turn_effects();
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
        defender_state.break_protect();
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
            attacker_state.heal(health_gained);
        }
        if (move == Move::Recover ||
            move == Move::Softboiled ||
            move == Move::MilkDrink ||
            move == Move::SlackOff ||
            move == Move::HealOrder
        ) {
            attacker_state.heal(attacker_state.max_health / 2);
        }
        if (move == Move::MorningSun ||
            move == Move::Synthesis ||
            move == Move::Moonlight
        ) {
            if (battle_state.weather == Weather::SUN) {
                attacker_state.heal(
                    2 * attacker_state.max_health / 3
                );
            } else if (battle_state.weather == Weather::CLEAR) {
                attacker_state.heal(attacker_state.max_health / 2);
            } else {
                attacker_state.heal(attacker_state.max_health / 4);
            }
        }
        if (attacker_state.get_item() == Item::ShellBell) {
            attacker_state.heal(attacker_move.damage / 8);
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
        if (attacker_state.is_player) {
            attacker_state.continue_multi_turn_move(player_turns);
        } else {
            attacker_state.continue_multi_turn_move(opponent_turns);
        }
    }

    if (move == Move::Uproar) {
        if (defender_state.get_status() == Status::SLEEP) {
            defender_state.clear_status();
        }
        if (attacker_state.is_player) {
            constexpr uint8_t player_turns = 5;
            attacker_state.continue_multi_turn_move(player_turns);
        } else {
            constexpr uint8_t opponent_turns = 2;
            attacker_state.continue_multi_turn_move(opponent_turns);
        }
    }

    if (move == Move::BugBite) {
        if (BERRIES.contains(defender_state.get_item()) &&
            defender_state.get_ability() != Ability::StickyHold
        ) {
            attacker_state.eat_berry(defender_state.get_item());
            defender_state.clear_item();
        }
    }

    if (move_has_flag(move, MoveFlag::REQUIRES_RECHARGE_TURN)) {
        attacker_state.used_move_that_requires_recharge();
    }

    // Recoil
    if (move_has_flag(move, MoveFlag::HAS_RECOIL)) {
        if (move == Move::HeadSmash ||
            (attacker_state.is_player &&
                (move == Move::JumpKick ||
                    move == Move::HighJumpKick))
        ) {
            attacker_state.apply_damage(attacker_move.damage / 2);
        } else if (
            move == Move::BraveBird ||
            move == Move::DoubleEdge ||
            move == Move::FlareBlitz ||
            move == Move::VoltTackle ||
            move == Move::WoodHammer
        ) {
            attacker_state.apply_damage(attacker_move.damage / 3);
        } else if (
            move == Move::Submission ||
            move == Move::TakeDown
        ) {
            attacker_state.apply_damage(attacker_move.damage / 4);
        } else if (move == Move::BellyDrum) {
            attacker_state.apply_damage(attacker_state.max_health / 2);
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
                    defender_state.try_apply_status(Status::POISON);
                } else if (defender_ability == Ability::FlameBody) {
                    defender_state.try_apply_status(Status::BURN);
                }
            }
        }
        if (defender_ability == Ability::RoughSkin) {
            attacker_state.apply_damage(attacker_state.max_health / 8);
        }
        if (defender_state.get_item() == Item::RockyHelmet) {
            attacker_state.apply_damage(attacker_state.max_health / 6);
        }
        if (defender_state.get_item() == Item::StickyBarb &&
            attacker_state.try_set_item(Item::StickyBarb)
        ) {
            defender_state.clear_item();
        }
    }

    // Life orb
    if (attacker_state.get_ability() != Ability::MagicGuard &&
        attacker_state.get_item() == Item::LifeOrb &&
        attacker_move.damage > 0 &&
        move_has_flag(attacker_move.move->move, MoveFlag::HAS_POWER)
    ) {
        attacker_state.apply_damage(
            std::max(
                1,
                attacker_state.get_health() / 10
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
                    defender_state.try_apply_status(Status::BURN);
                }
            } else {
                defender_state.try_apply_status(Status::BURN);
            }
        }
    }

    // Poison application
    if (!defender_state.has_type(PokemonType::POISON) &&
        !defender_state.has_type(PokemonType::STEEL)
    ) {
        const bool flung_poison_barb = is_fling &&
            attacker_state.get_item() == Item::PoisonBarb;
        if (move_has_flag(move, MoveFlag::POISONS_DEFENDER) ||
            flung_poison_barb
        ) {
            if (attacker_state.is_player) {
                if (flung_poison_barb ||
                    move == Move::PoisonGas ||
                    move == Move::PoisonPowder
                ) {
                    defender_state.try_apply_status(Status::POISON);
                }
            } else {
                defender_state.try_apply_status(Status::POISON);
            }
        }
    }

    // Bad poison application
    if (move_has_flag(move, MoveFlag::BADLY_POISONS)) {
        if (defender_state.is_player) {
            defender_state.try_apply_status(Status::BADLY_POISONED);
        }
        if (move == Move::Toxic) {
            defender_state.try_apply_status(Status::BADLY_POISONED);
        }
    }

    // Other status
    if (move == Move::TriAttack && defender_state.is_player) {
        defender_state.try_apply_status(Status::POISON);
    }

    // Attack
    if (move_has_flag(move, MoveFlag::BOOSTS_ATTACKERS_ATTACK)) {
        if (move == Move::Meditate ||
            move == Move::Sharpen ||
            move == Move::Howl ||
            move == Move::BulkUp ||
            move == Move::DragonDance
        ) {
            attacker_state.change_stat_modifier(Stat::ATTACK, 1);
        }
        if (move == Move::SwordsDance) {
            attacker_state.change_stat_modifier(Stat::ATTACK, 2);
        }
        if (move == Move::BellyDrum) {
            attacker_state.change_stat_modifier(Stat::ATTACK, 6);
        }
        if (!attacker_state.is_player &&
            (move == Move::MeteorMash ||
                move == Move::MetalClaw)
        ) {
            attacker_state.change_stat_modifier(Stat::ATTACK, 1);
        }
    }

    if (move_has_flag(move, MoveFlag::LOWERS_ATTACKERS_ATTACK)) {
        attacker_state.change_stat_modifier(Stat::ATTACK, -1);
    }

    if (move_has_flag(move, MoveFlag::LOWERS_DEFENDER_ATTACK)) {
        if (move == Move::Tickle ||
            move == Move::Growl
        ) {
            defender_state.change_stat_modifier(Stat::ATTACK, -1);
        }
        if (move == Move::Charm ||
            move == Move::Memento ||
            move == Move::Featherdance
        ) {
            defender_state.change_stat_modifier(Stat::ATTACK, -2);
        }
        if (defender_state.is_player &&
            move == Move::AuroraBeam
        ) {
            defender_state.change_stat_modifier(Stat::ATTACK, -1);
        }
    }

    // Defense
    if (move_has_flag(move, MoveFlag::BOOSTS_ATTACKERS_DEFENSE)) {
        if (move == Move::BulkUp) {
            attacker_state.change_stat_modifier(Stat::DEFENSE, 1);
        }
    }
    if (move_has_flag(move, MoveFlag::LOWERS_ATTACKERS_DEFENSE)) {
        attacker_state.change_stat_modifier(Stat::DEFENSE, -1);
    }

    if (move_has_flag(move, MoveFlag::LOWERS_DEFENDER_DEFENSE)) {
        if (defender_state.is_player &&
            (move == Move::IronTail ||
                move == Move::Crunch ||
                move == Move::RockSmash ||
                move == Move::CrushClaw)
        ) {
            defender_state.change_stat_modifier(Stat::DEFENSE, -1);
        }
        if (move == Move::TailWhip ||
            move == Move::Leer ||
            move == Move::Tickle
        ) {
            defender_state.change_stat_modifier(Stat::DEFENSE, -1);
        }
        if (move == Move::Screech) {
            defender_state.change_stat_modifier(Stat::DEFENSE, -2);
        }
    }

    // Special Attack
    if (move_has_flag(move, MoveFlag::LOWERS_ATTACKERS_SPECIAL_ATTACK)) {
        attacker_state.change_stat_modifier(Stat::SPECIAL_ATTACK, -2);
    }

    if (move_has_flag(move, MoveFlag::LOWERS_DEFENDER_SPECIAL_ATTACK)) {
        if (defender_state.is_player &&
            move == Move::MistBall
        ) {
            defender_state.change_stat_modifier(Stat::SPECIAL_ATTACK, -1);
        }
        if (move == Move::Memento) {
            defender_state.change_stat_modifier(Stat::SPECIAL_ATTACK, -2);
        }
    }

    // Special Defense
    if (move_has_flag(move, MoveFlag::LOWERS_ATTACKERS_SPECIAL_DEFENSE)) {
        attacker_state.change_stat_modifier(Stat::SPECIAL_DEFENSE, -1);
    }

    if (move_has_flag(move, MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE)) {
        if (move == Move::MetalSound ||
            move == Move::FakeTears
        ) {
            defender_state.change_stat_modifier(Stat::SPECIAL_DEFENSE, -2);
        }
        if (defender_state.is_player &&
            move == Move::SeedFlare
        ) {
            defender_state.change_stat_modifier(Stat::SPECIAL_DEFENSE, -2);
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
            defender_state.change_stat_modifier(Stat::SPECIAL_DEFENSE, -1);
        }
    }

    // Speed
    if (move_has_flag(move, MoveFlag::BOOSTS_ATTACKERS_SPEED)) {
        if (move == Move::DragonDance) {
            attacker_state.change_stat_modifier(Stat::SPEED, 1);
        }
    }

    if (move_has_flag(move, MoveFlag::LOWERS_ATTACKERS_SPEED)) {
        attacker_state.change_stat_modifier(Stat::SPEED, -1);
    }

    if (move_has_flag(move, MoveFlag::LOWERS_DEFENDER_SPEED)) {
        if (move == Move::IcyWind ||
            move == Move::RockTomb ||
            move == Move::MudShot
        ) {
            defender_state.change_stat_modifier(Stat::SPEED, -1);
        }
        if (defender_state.is_player &&
            (move == Move::Bubblebeam ||
                move == Move::Constrict ||
                move == Move::Bubble ||
                move == Move::StringShot)
        ) {
            defender_state.change_stat_modifier(Stat::SPEED, -1);
        }
        if (defender_state.is_player &&
            (move == Move::CottonSpore ||
                move == Move::ScaryFace)
        ) {
            defender_state.change_stat_modifier(Stat::SPEED, -2);
        }
    }


    // Omni boost
    if (!attacker_state.is_player &&
        move_has_flag(move, MoveFlag::OMNI_BOOSTS_ATTACKER)
    ) {
        attacker_state.change_stat_modifier(Stat::ATTACK, 1);
        attacker_state.change_stat_modifier(Stat::DEFENSE, 1);
        attacker_state.change_stat_modifier(Stat::SPECIAL_ATTACK, 1);
        attacker_state.change_stat_modifier(Stat::SPECIAL_DEFENSE, 1);
        attacker_state.change_stat_modifier(Stat::SPEED, 1);
    }

    if (is_fling) {
        // TODO other items
        if (attacker_state.get_item() == Item::KingsRock ||
            attacker_state.get_item() == Item::RazorFang
        ) {
            defender_state.got_flinched();
        } else if (attacker_state.get_item() == Item::LightBall) {
            defender_state.try_apply_status(Status::PARALYZED);
        } else if (attacker_state.get_item() == Item::MentalHerb) {
            defender_state.got_infatuated();
        } else if (attacker_state.get_item() == Item::WhiteHerb) {
            defender_state.clear_negative_stat_changes();
        }
        attacker_state.clear_item();
    }
}

bool is_battle_over(
    const PokemonState& player_state,
    const PokemonState& opponent_state
) {
    return player_state.get_health() <= 0 ||
        opponent_state.get_health() <= 0;
}

bool battle(const CustomPokemon& player, const CustomPokemon& opponent) {
    BattleState battle_state{};
    auto player_state = PokemonState{
        player,
        true
    };
    auto opponent_state = PokemonState{
        opponent,
        false
    };

    BestMove player_move;
    BestMove opponent_move;
    while (player_state.get_health() > 0 &&
        opponent_state.get_health() > 0
    ) {
        player_move = player_state.get_best_move_against_defender(
            battle_state,
            opponent_state,
            false
        );
        opponent_move = opponent_state.get_best_move_against_defender(
            battle_state,
            player_state,
            false
        );
        const bool player_goes_first = player_state.outspeeds(
            opponent_state,
            opponent_move.move,
            player_move.move
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
        if (player_goes_first) {
            player_state.execute_move(
                battle_state,
                opponent_state,
                opponent_move
            );
            apply_post_move_effects(
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
            if (!opponent_state.is_flinched()) {
                opponent_state.execute_move(
                    battle_state,
                    player_state,
                    player_move
                );
                apply_post_move_effects(
                    battle_state,
                    opponent_state,
                    opponent_move,
                    player_state,
                    player_move
                );
            }
            if (is_battle_over(player_state, opponent_state)) {
                break;
            }
        } else {
            opponent_state.execute_move(
                battle_state,
                player_state,
                player_move
            );
            apply_post_move_effects(
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
            if (!player_state.is_flinched()) {
                player_state.execute_move(
                    battle_state,
                    opponent_state,
                    opponent_move
                );
                apply_post_move_effects(
                    battle_state,
                    player_state,
                    player_move,
                    opponent_state,
                    opponent_move
                );
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
            !player_state.is_recharging() &&
            !opponent_state.is_recharging() &&
            player_state.get_location_on_field() == FieldLocation::ON_FIELD &&
            opponent_state.get_location_on_field() == FieldLocation::ON_FIELD &&
            !player_state.is_charging() &&
            !opponent_state.is_charging()
        ) {
            break;
        }
    }
    if (player_state.get_health() > 0 &&
        opponent_state.get_health() <= 0 &&
        opponent_state.get_ability() == Ability::Aftermath &&
        player_state.get_ability() != Ability::Damp
    ) {
        player_state.apply_damage(player_state.max_health / 4);
    }

    if (player_state.get_health() > 0) {
        return true;
    }
    return false;
}
