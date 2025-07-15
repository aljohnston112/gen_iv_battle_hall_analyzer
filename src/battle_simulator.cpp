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

void apply_end_of_turn_effects(
    BattleState& battle_state,
    PokemonState& player_state,
    PokemonState& opponent_state
) {
    player_state.apply_end_of_turn_effects(
        battle_state.get_weather(),
        opponent_state
    );
    opponent_state.apply_end_of_turn_effects(
        battle_state.get_weather(),
        player_state
    );
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
    const auto attacker_ability = attacker_state.get_ability();
    const auto defender_ability = defender_state.get_ability();

    bool apply_effect = defender_ability != Ability::ShieldDust;

    //Flash fire
    if (defender_ability == Ability::FlashFire &&
        attacker_move.move->type == PokemonType::FIRE
    ) {
        defender_state.set_flash_fire();
    }

    if (defender_ability == Ability::MotorDrive &&
        attacker_move.move->type == PokemonType::ELECTRIC
    ) {
        defender_state.change_stat_modifier(Stat::SPEED, 1, true);
    }

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
            if (defender_ability == Ability::LiquidOoze) {
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
            const auto weather = battle_state.get_weather();
            if (weather == Weather::SUN) {
                attacker_state.heal(
                    2 * attacker_state.max_health / 3
                );
            } else if (weather == Weather::CLEAR) {
                attacker_state.heal(attacker_state.max_health / 2);
            } else {
                attacker_state.heal(attacker_state.max_health / 4);
            }
        }
        if (attacker_state.get_item() == Item::ShellBell) {
            attacker_state.heal(attacker_move.damage / 8);
        }
    }
    if (defender_ability == Ability::DrySkin &&
        move_has_flag(move, MoveFlag::HAS_POWER) &&
        attacker_move.move->type == PokemonType::WATER
    ) {
        attacker_state.heal(attacker_state.max_health / 4);
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
            attacker_state.increment_multi_turn_move_counter(player_turns);
        } else {
            attacker_state.increment_multi_turn_move_counter(opponent_turns);
        }
    }

    if (move == Move::Uproar) {
        if (defender_state.get_status() == Status::SLEEP) {
            defender_state.clear_status();
        }
        if (attacker_state.is_player) {
            constexpr uint8_t player_turns = 5;
            attacker_state.increment_multi_turn_move_counter(player_turns);
        } else {
            constexpr uint8_t opponent_turns = 2;
            attacker_state.increment_multi_turn_move_counter(opponent_turns);
        }
    }

    if (move == Move::BugBite) {
        if (BERRIES.contains(defender_state.get_item()) &&
            defender_ability != Ability::StickyHold
        ) {
            attacker_state.eat_berry(defender_state.get_item());
            defender_state.clear_item();
        }
    }

    if (move_has_flag(move, MoveFlag::REQUIRES_RECHARGE_TURN)) {
        attacker_state.used_move_that_requires_recharge();
    }

    // Recoil
    if (move_has_flag(move, MoveFlag::HAS_RECOIL) &&
        attacker_ability != Ability::RockHead
    ) {
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
    const auto weather = battle_state.get_weather();
    if (move_has_flag(move, MoveFlag::MAKES_CONTACT)) {
        if (attacker_state.is_player) {
            if (defender_state.get_status() == Status::NONE) {
                if (defender_ability == Ability::EffectSpore ||
                    defender_ability == Ability::PoisonPoint
                ) {
                    defender_state.try_apply_status(
                        Status::POISON,
                        weather,
                        attacker_state
                    );
                } else if (defender_ability == Ability::FlameBody) {
                    defender_state.try_apply_status(
                        Status::BURN,
                        weather,
                        attacker_state
                    );
                }
            }
        }
        if (defender_ability == Ability::RoughSkin) {
            attacker_state.apply_damage(attacker_state.max_health / 8);
        }
        if (defender_state.get_item() == Item::RockyHelmet) {
            attacker_state.apply_damage(attacker_state.max_health / 6);
        } else if (defender_state.get_item() == Item::StickyBarb &&
            attacker_state.try_set_item(Item::StickyBarb)
        ) {
            defender_state.clear_item();
        }
    }

    // Life orb
    if (attacker_ability != Ability::MagicGuard &&
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
    if (!defender_state.has_type(PokemonType::FIRE) && apply_effect) {
        const bool flung_flame_orb =
            (is_fling && attacker_state.get_item() == Item::FlameOrb);
        if (move_has_flag(move, MoveFlag::BURNS_DEFENDER) ||
            flung_flame_orb
        ) {
            if (attacker_state.is_player) {
                if (flung_flame_orb || move == Move::WillOWisp) {
                    defender_state.try_apply_status(
                        Status::BURN,
                        weather,
                        attacker_state
                    );
                }
            } else {
                defender_state.try_apply_status(
                    Status::BURN,
                    weather,
                    attacker_state
                );
            }
        }
    }

    // Poison application
    if (!defender_state.has_type(PokemonType::POISON) &&
        !defender_state.has_type(PokemonType::STEEL) && apply_effect
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
                    defender_state.try_apply_status(
                        Status::POISON,
                        weather,
                        attacker_state
                    );
                }
            } else {
                defender_state.try_apply_status(
                    Status::POISON,
                    weather,
                    attacker_state
                );
            }
        }
    }

    // Bad poison application
    if (move_has_flag(move, MoveFlag::BADLY_POISONS) && apply_effect) {
        if (defender_state.is_player) {
            defender_state.try_apply_status(
                Status::BADLY_POISONED,
                weather,
                attacker_state
            );
        }
        if (move == Move::Toxic) {
            defender_state.try_apply_status(
                Status::BADLY_POISONED,
                weather,
                attacker_state
            );
        }
    }

    // Other status
    if (move == Move::TriAttack && defender_state.is_player && apply_effect) {
        defender_state.try_apply_status(
            Status::POISON,
            weather,
            attacker_state
        );
    }

    // Attack
    if (move_has_flag(move, MoveFlag::BOOSTS_ATTACKERS_ATTACK)) {
        if (move == Move::Meditate ||
            move == Move::Sharpen ||
            move == Move::Howl ||
            move == Move::BulkUp ||
            move == Move::DragonDance
        ) {
            attacker_state.change_stat_modifier(
                Stat::ATTACK,
                1,
                false
            );
        }
        if (move == Move::SwordsDance) {
            attacker_state.change_stat_modifier(
                Stat::ATTACK,
                2,
                false
            );
        }
        if (move == Move::BellyDrum) {
            attacker_state.change_stat_modifier(
                Stat::ATTACK,
                6,
                false
            );
        }
        if (!attacker_state.is_player &&
            (move == Move::MeteorMash ||
                move == Move::MetalClaw) && apply_effect
        ) {
            attacker_state.change_stat_modifier(
                Stat::ATTACK,
                1,
                false
            );
        }
    }

    if (move_has_flag(move, MoveFlag::LOWERS_ATTACKERS_ATTACK)) {
        attacker_state.change_stat_modifier(
            Stat::ATTACK,
            -1,
            false
        );
    }

    if (move_has_flag(move, MoveFlag::RAISES_DEFENDER_ATTACK)) {
        defender_state.change_stat_modifier(
            Stat::ATTACK,
            2,
            false
        );
    }

    if (move_has_flag(move, MoveFlag::LOWERS_DEFENDER_ATTACK)) {
        if (move == Move::Tickle ||
            move == Move::Growl
        ) {
            defender_state.change_stat_modifier(
                Stat::ATTACK,
                -1,
                true
            );
        }
        if (move == Move::Charm ||
            move == Move::Memento ||
            move == Move::Featherdance
        ) {
            defender_state.change_stat_modifier(
                Stat::ATTACK,
                -2,
                true
            );
        }
        if (defender_state.is_player &&
            move == Move::AuroraBeam && apply_effect
        ) {
            defender_state.change_stat_modifier(
                Stat::ATTACK,
                -1,
                true
            );
        }
    }

    // Defense
    if (move_has_flag(move, MoveFlag::BOOSTS_ATTACKERS_DEFENSE)) {
        if (move == Move::BulkUp ||
            move == Move::Harden ||
            move == Move::Withdraw ||
            move == Move::CosmicPower ||
            move == Move::DefendOrder ||
            (move == Move::SteelWing &&
                !attacker_state.is_player &&
                apply_effect)
        ) {
            attacker_state.change_stat_modifier(
                Stat::DEFENSE,
                1,
                false
            );
        }
        if (move == Move::AcidArmor ||
            move == Move::IronDefense
        ) {
            attacker_state.change_stat_modifier(
                Stat::DEFENSE,
                2,
                false
            );
        }
    }
    if (move_has_flag(move, MoveFlag::LOWERS_ATTACKERS_DEFENSE)) {
        attacker_state.change_stat_modifier(
            Stat::DEFENSE,
            -1,
            false
        );
    }

    if (move_has_flag(move, MoveFlag::LOWERS_DEFENDER_DEFENSE)) {
        if (defender_state.is_player &&
            (move == Move::IronTail ||
                move == Move::Crunch ||
                move == Move::RockSmash ||
                move == Move::CrushClaw)
        ) {
            defender_state.change_stat_modifier(
                Stat::DEFENSE,
                -1,
                true
            );
        }
        if (move == Move::TailWhip ||
            move == Move::Leer ||
            move == Move::Tickle
        ) {
            defender_state.change_stat_modifier(
                Stat::DEFENSE,
                -1,
                true
            );
        }
        if (move == Move::Screech) {
            defender_state.change_stat_modifier(
                Stat::DEFENSE,
                -2,
                true
            );
        }
    }

    // Special Attack
    if (move_has_flag(move, MoveFlag::BOOSTS_ATTACKERS_SPECIAL_ATTACK)) {
        if (move == Move::Growth ||
            move == Move::CalmMind ||
            (move == Move::ChargeBeam && !attacker_state.is_player)
        ) {
            attacker_state.change_stat_modifier(
                Stat::SPECIAL_ATTACK,
                1,
                false
            );
        }
        if (move == Move::TailGlow ||
            move == Move::NastyPlot
        ) {
            attacker_state.change_stat_modifier(
                Stat::SPECIAL_ATTACK,
                2,
                false
            );
        }
    }

    if (move_has_flag(move, MoveFlag::LOWERS_ATTACKERS_SPECIAL_ATTACK)) {
        attacker_state.change_stat_modifier(
            Stat::SPECIAL_ATTACK,
            -2,
            false
        );
    }

    if (move_has_flag(move, MoveFlag::RAISES_DEFENDER_SPECIAL_ATTACK)) {
        defender_state.change_stat_modifier(
            Stat::SPECIAL_ATTACK,
            2,
            false
        );
    }

    if (move_has_flag(move, MoveFlag::LOWERS_DEFENDER_SPECIAL_ATTACK)) {
        if (defender_state.is_player &&
            move == Move::MistBall
        ) {
            defender_state.change_stat_modifier(
                Stat::SPECIAL_ATTACK,
                -1,
                true
            );
        }
        if (move == Move::Memento) {
            defender_state.change_stat_modifier(
                Stat::SPECIAL_ATTACK,
                -2,
                true
            );
        }
    }

    // Special Defense
    if (move_has_flag(move, MoveFlag::BOOSTS_ATTACKERS_SPECIAL_DEFENSE)) {
        if (move == Move::CosmicPower ||
            move == Move::DefendOrder ||
            move == Move::CalmMind
        ) {
            attacker_state.change_stat_modifier(
                Stat::DEFENSE,
                1,
                false
            );
        }
        if (move == Move::Amnesia) {
            attacker_state.change_stat_modifier(
                Stat::DEFENSE,
                2,
                false
            );
        }
    }

    if (move_has_flag(move, MoveFlag::LOWERS_ATTACKERS_SPECIAL_DEFENSE)) {
        attacker_state.change_stat_modifier(
            Stat::SPECIAL_DEFENSE,
            -1,
            false
        );
    }

    if (move_has_flag(move, MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE)) {
        if (move == Move::MetalSound ||
            move == Move::FakeTears
        ) {
            defender_state.change_stat_modifier(
                Stat::SPECIAL_DEFENSE,
                -2,
                true
            );
        }
        if (defender_state.is_player &&
            move == Move::SeedFlare
        ) {
            defender_state.change_stat_modifier(
                Stat::SPECIAL_DEFENSE,
                -2,
                true
            );
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
            defender_state.change_stat_modifier(
                Stat::SPECIAL_DEFENSE,
                -1,
                true
            );
        }
    }

    // Speed
    if (move_has_flag(move, MoveFlag::BOOSTS_ATTACKERS_SPEED)) {
        if (move == Move::DragonDance) {
            attacker_state.change_stat_modifier(
                Stat::SPEED,
                1,
                false
            );
        }
        if (move == Move::Agility ||
            move == Move::RockPolish
        ) {
            attacker_state.change_stat_modifier(
                Stat::SPEED,
                2,
                false
            );
        }
    }

    if (move_has_flag(move, MoveFlag::LOWERS_ATTACKERS_SPEED)) {
        attacker_state.change_stat_modifier(
            Stat::SPEED,
            -1,
            false
        );
    }

    if (move_has_flag(move, MoveFlag::LOWERS_DEFENDER_SPEED)) {
        if (move == Move::IcyWind ||
            move == Move::RockTomb ||
            move == Move::MudShot
        ) {
            defender_state.change_stat_modifier(
                Stat::SPEED,
                -1,
                true
            );
        }
        if (defender_state.is_player &&
            (move == Move::Bubblebeam ||
                move == Move::Constrict ||
                move == Move::Bubble ||
                move == Move::StringShot)
        ) {
            defender_state.change_stat_modifier(
                Stat::SPEED,
                -1,
                true
            );
        }
        if (defender_state.is_player &&
            (move == Move::CottonSpore ||
                move == Move::ScaryFace)
        ) {
            defender_state.change_stat_modifier(
                Stat::SPEED,
                -2,
                true
            );
        }
    }


    // Omni boost
    if (!attacker_state.is_player &&
        move_has_flag(move, MoveFlag::OMNI_BOOSTS_ATTACKER)
    ) {
        attacker_state.change_stat_modifier(
            Stat::ATTACK,
            1,
            false
        );
        attacker_state.change_stat_modifier(
            Stat::DEFENSE,
            1,
            false
        );
        attacker_state.change_stat_modifier(
            Stat::SPECIAL_ATTACK,
            1,
            false
        );
        attacker_state.change_stat_modifier(
            Stat::SPECIAL_DEFENSE,
            1,
            false
        );
        attacker_state.change_stat_modifier(
            Stat::SPEED,
            1,
            false
        );
    }

    if (is_fling) {
        // TODO other items
        if (attacker_state.get_item() == Item::KingsRock ||
            attacker_state.get_item() == Item::RazorFang
        ) {
            defender_state.set_flinched();
        } else if (attacker_state.get_item() == Item::LightBall) {
            defender_state.try_apply_status(
                Status::PARALYZED,
                weather,
                attacker_state
            );
        } else if (attacker_state.get_item() == Item::MentalHerb) {
            defender_state.set_infatuated();
        } else if (attacker_state.get_item() == Item::WhiteHerb) {
            defender_state.clear_negative_stat_changes();
        }
        attacker_state.clear_item();
    }

    // Color Change
    if (defender_ability == Ability::ColorChange &&
        defender_state.has_type(attacker_move.move->type) &&
        attacker_move.move->move != Move::Struggle &&
        attacker_move.move->move != Move::PainSplit
    ) {
        defender_state.change_type(attacker_move.move->type);
    }
}

bool is_battle_over(
    const PokemonState& player_state,
    const PokemonState& opponent_state
) {
    return player_state.get_health() <= 0 ||
        opponent_state.get_health() <= 0;
}

void check_download(PokemonState& state0, const PokemonState& state1) {
    if (state0.get_ability() == Ability::Download) {
        if (state1.get_special_defense(Weather::CLEAR) > state1.get_defense()) {
            state0.change_stat_modifier(
                Stat::ATTACK,
                1,
                false
            );
        } else {
            state0.change_stat_modifier(
                Stat::SPECIAL_ATTACK,
                1,
                false
            );
        }
    }
}

void check_drizzle(
    BattleState& battle_state,
    const PokemonState& pokemon_state
) {
    if (pokemon_state.get_ability() == Ability::Drizzle) {
        battle_state.set_weather(Weather::RAIN, -1);
    }
}

void check_drought(
    BattleState& battle_state,
    const PokemonState& pokemon_state
) {
    if (pokemon_state.get_ability() == Ability::Drought) {
        battle_state.set_weather(Weather::SUN, -1);
    }
}

void check_mold_breaker(
    const PokemonState& state0,
    PokemonState& state1
) {
    if (state0.get_ability() == Ability::MoldBreaker &&
        ability_is_ignorable(state1.get_ability())
    ) {
        state1.clear_ability();
    }
}

void check_sandstorm(
    BattleState& battle_state,
    const PokemonState& pokemon_state
) {
    if (pokemon_state.get_ability() == Ability::SandStream) {
        battle_state.set_weather(Weather::SANDSTORM, -1);
    }
}

void check_snow_warning(
    BattleState& battle_state,
    const PokemonState& pokemon_state
) {
    if (pokemon_state.get_ability() == Ability::SnowWarning) {
        battle_state.set_weather(Weather::HAIL, -1);
    }
}

void check_trace(
    PokemonState& state0,
    const PokemonState& state1
) {
    if (state0.get_ability() == Ability::Trace) {
        state0.set_ability(state1.get_ability(), true);
    }
}

void check_intimidate(
    const PokemonState& state0,
    PokemonState& state1
) {
    if (state0.get_ability() == Ability::Intimidate) {
        state1.change_stat_modifier(Stat::ATTACK, -1, true);
    }
}

std::pair<bool, std::vector<const MoveInfo*>> battle(
    const CustomPokemon& player, const CustomPokemon& opponent
) {
    BattleState battle_state{player, opponent};
    auto player_state = PokemonState{
        player,
        true
    };
    auto opponent_state = PokemonState{
        opponent,
        false
    };

    std::vector<const MoveInfo*> player_moves{};

    // May suppress abilities
    check_mold_breaker(player_state, opponent_state);
    check_mold_breaker(opponent_state, player_state);
    check_trace(player_state, opponent_state);
    check_trace(opponent_state, player_state);
    check_download(player_state, opponent_state);
    check_download(opponent_state, player_state);
    check_intimidate(player_state, opponent_state);
    check_intimidate(opponent_state, player_state);

    bool player_goes_first = player_state.outspeeds(
        opponent_state,
        nullptr,
        nullptr,
        battle_state.get_weather()
    );
    if (player_goes_first) {
        check_drizzle(battle_state, player_state);
        check_drought(battle_state, player_state);
        check_sandstorm(battle_state, player_state);
        check_snow_warning(battle_state, player_state);

        check_drizzle(battle_state, opponent_state);
        check_drought(battle_state, opponent_state);
        check_sandstorm(battle_state, opponent_state);
        check_snow_warning(battle_state, opponent_state);
    } else {
        check_drizzle(battle_state, opponent_state);
        check_drought(battle_state, opponent_state);
        check_sandstorm(battle_state, opponent_state);
        check_snow_warning(battle_state, opponent_state);

        check_drizzle(battle_state, player_state);
        check_drought(battle_state, player_state);
        check_sandstorm(battle_state, player_state);
        check_snow_warning(battle_state, player_state);
    }


    BestMove player_move;
    BestMove opponent_move;
    while (player_state.get_health() > 0 &&
        opponent_state.get_health() > 0
    ) {
        player_move = player_state.get_best_move_against_defender(
            opponent_state,
            false,
            battle_state.get_weather(),
            battle_state.is_mid_turn()
        );
        opponent_move = opponent_state.get_best_move_against_defender(
            player_state,
            false,
            battle_state.get_weather(),
            battle_state.is_mid_turn()
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
                move != Move::Revenge &&
                move != Move::LastResort &&
                move != Move::BugBite &&
                move != Move::Payback &&
                move != Move::Endeavor &&
                move != Move::LowKick &&
                move != Move::GrassKnot &&
                move != Move::Transform &&
                move != Move::FakeOut &&
                move != Move::FutureSight && // TODO probably useless
                move != Move::WringOut &&
                (move_has_flag(
                        move,
                        MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE
                    ) ||
                    move_has_flag(move, MoveFlag::HAS_FIXED_DAMAGE) ||
                    move_has_flag(move, MoveFlag::MAKES_ATTACKER_FAINT) ||
                    move_has_flag(move, MoveFlag::IS_OTHER) ||
                    move_has_flag(move, MoveFlag::PROTECTS_USER))
            ) {
                printf("");
            }
        }

        player_goes_first = player_state.outspeeds(
            opponent_state,
            opponent_move.move,
            player_move.move,
            battle_state.get_weather()
        );

        if (player_goes_first) {
            battle_state.execute_move(
                player_state,
                opponent_state,
                battle_state.get_weather(),
                battle_state.is_mid_turn()
            );
            player_moves.push_back(player_move.move);
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
            battle_state.set_mid_turn();
            if (!opponent_state.is_flinched()) {
                battle_state.execute_move(
                    opponent_state,
                    player_state,
                    battle_state.get_weather(),
                    battle_state.is_mid_turn()
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
            battle_state.execute_move(
                opponent_state,
                player_state,
                battle_state.get_weather(),
                battle_state.is_mid_turn()
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
            battle_state.set_mid_turn();
            if (!player_state.is_flinched()) {
                battle_state.execute_move(
                    player_state,
                    opponent_state,
                    battle_state.get_weather(),
                    battle_state.is_mid_turn()
                );
                player_moves.push_back(player_move.move);
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
            battle_state,
            player_state,
            opponent_state
        );
        battle_state.end_turn();

        if (player_move.damage == 0 &&
            opponent_move.damage == 0 &&
            !player_state.is_recharging() &&
            !opponent_state.is_recharging() &&
            player_state.get_field_location() == FieldLocation::ON_FIELD &&
            opponent_state.get_field_location() == FieldLocation::ON_FIELD &&
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
        return std::make_pair(true, player_moves);
    }
    return std::make_pair(false, player_moves);
}
