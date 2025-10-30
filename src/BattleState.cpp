#include "BattleState.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <set>

#include "config.h"
#include "custom_pokemon.h"
#include "Items.h"
#include "PokemonState.h"

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

inline void check_download(PokemonState& state0, PokemonState& state1) {
    if (const auto ability0 = state0.get_ability();
        ability0 == Ability::Download
    ) {
        if (state1.get_special_defense(Weather::CLEAR, ability0) >
            state1.get_defense(Weather::CLEAR, ability0)
        ) {
            state0.change_stat_stage(
                Stat::ATTACK,
                1,
                false
            );
        } else {
            state0.change_stat_stage(
                Stat::SPECIAL_ATTACK,
                1,
                false
            );
        }
    }
}

inline void check_intimidate(
    const PokemonState& state0,
    PokemonState& state1
) {
    if (state0.get_ability() == Ability::Intimidate) {
        state1.change_stat_stage(Stat::ATTACK, -1, true);
    }
}

inline void check_mold_breaker(
    const PokemonState& state0,
    PokemonState& state1
) {
    if (state0.get_ability() == Ability::MoldBreaker &&
        ability_is_ignorable(state1.get_ability())
    ) {
        state1.disable_ability();
    }
}

inline void check_trace(
    PokemonState& state0,
    const PokemonState& state1
) {
    if (state0.get_ability() == Ability::Trace) {
        state0.set_ability(state1.get_ability(), true);
    }
}

inline void check_unimplemented_moves(
    const BestMove& player_move,
    const BestMove& opponent_move
) {
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
            move != Move::Pluck &&
            move != Move::Payback &&
            move != Move::Endeavor &&
            move != Move::LowKick &&
            move != Move::GrassKnot &&
            move != Move::Transform &&
            move != Move::FakeOut &&
            move != Move::FutureSight && // TODO probably useless
            move != Move::Bide && // TODO also probably useless
            move != Move::Snore && // TODO ignored for now
            move != Move::WringOut &&
            move != Move::Rollout &&
            move != Move::KnockOff &&
            move != Move::Reversal &&
            move != Move::SuperFang &&
            move != Move::SeismicToss &&
            move != Move::NightShade &&
            move != Move::DragonRage &&
            move != Move::GyroBall &&
            move != Move::MetalBurst &&
            move != Move::Psywave &&
            move != Move::Punishment &&
            move != Move::Judgment &&
            move != Move::TripleKick &&
            move != Move::Flail &&
            move != Move::IceBall &&
            move != Move::Fling &&
            move != Move::Thief &&
            move != Move::Covet &&
            move != Move::SecretPower &&
            move != Move::Explosion &&
            move != Move::Selfdestruct &&
            move != Move::FalseSwipe &&
            move != Move::Feint &&
            move != Move::NaturalGift &&
            move != Move::Pursuit &&
            move != Move::WakeUpSlap &&
            move != Move::Magnitude &&
            move != Move::SpitUp &&
            move != Move::Assurance &&
            move != Move::CrushGrip &&
            move != Move::Rage &&
            move != Move::Splash &&
            move != Move::Safeguard &&
            move != Move::Struggle &&
            !move_has_flag(move, MoveFlag::LOWERS_DEFENDER_ATTACK) &&
            (move_has_flag(
                    move,
                    MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE
                ) ||
                move_has_flag(move, MoveFlag::HAS_FIXED_DAMAGE) ||
                move_has_flag(move, MoveFlag::MAKES_ATTACKER_FAINT) ||
                move_has_flag(move, MoveFlag::IS_OTHER) ||
                move_has_flag(move, MoveFlag::PROTECTS_USER))
        ) {
            throw std::logic_error{
                "Unimplemented move: " + MOVE_TO_STRING.at(move)
            };
        }
    }
}

void add_last_used_move(
    std::vector<
        std::pair<const MoveInfo*, int>
    >& attacker_moves,
    const PokemonState& attacker_state,
    const PokemonState& defender_state
) {
    if (const auto last_used_move = attacker_state.get_last_used_move();
        last_used_move.move != nullptr &&
        defender_state.get_field_location() == FieldLocation::ON_FIELD
    ) {
        attacker_moves.push_back(
            std::make_pair(
                last_used_move.move,
                last_used_move.damage
            )
        );
    }
}

class BattleState {
    PokemonState player_state;
    PokemonState opponent_state;

    bool first_turn = true;
    bool mid_turn = false;

    Weather weather = Weather::CLEAR;
    int weather_turns = 0;

    [[nodiscard]] bool is_first_turn() const {
        return first_turn;
    }

    [[nodiscard]] bool is_mid_turn() const {
        return mid_turn;
    }

    void set_mid_turn() {
        mid_turn = true;
    }

    void end_turn() {
        first_turn = false;
        mid_turn = false;
        if (weather_turns != -1 && weather_turns != 0) {
            weather_turns--;
            if (weather_turns == 0) {
                weather = Weather::CLEAR;
            }
        }
    }

    void check_forecast(PokemonState& pokemon_state) const {
        if (const auto ability = pokemon_state.get_ability();
            ability == Ability::Forecast
        ) {
            switch (weather) {
            case Weather::SUN:
                pokemon_state.change_type(PokemonType::FIRE);
                break;
            case Weather::RAIN:
                pokemon_state.change_type(PokemonType::WATER);
                break;
            case Weather::HAIL:
                pokemon_state.change_type(PokemonType::ICE);
                break;
            default:
                pokemon_state.change_type(PokemonType::NORMAL);
            }
        }
    }

    /**
     * @param weather
     * @param turns -1 for weather that should not end.
     */
    void set_weather(
        const Weather weather,
        const int turns
    ) {
        this->weather = weather;
        this->weather_turns = turns;
        check_forecast(player_state);
        check_forecast(opponent_state);
    }

    [[nodiscard]] Weather get_weather() const {
        const auto player_ability = player_state.get_ability();
        const auto opponent_ability = opponent_state.get_ability();
        if (player_ability != Ability::AirLock &&
            opponent_ability != Ability::AirLock &&
            player_ability != Ability::CloudNine &&
            opponent_ability == Ability::CloudNine
        ) {
            return Weather::CLEAR;
        }
        return weather;
    }

    void check_drizzle(
        const PokemonState& pokemon_state
    ) {
        if (pokemon_state.get_ability() == Ability::Drizzle) {
            set_weather(Weather::RAIN, -1);
        }
    }

    void check_drought(
        const PokemonState& pokemon_state
    ) {
        if (pokemon_state.get_ability() == Ability::Drought) {
            set_weather(Weather::SUN, -1);
        }
    }

    void check_sandstorm(
        const PokemonState& pokemon_state
    ) {
        if (pokemon_state.get_ability() == Ability::SandStream) {
            set_weather(Weather::SANDSTORM, -1);
        }
    }

    void check_snow_warning(
        const PokemonState& pokemon_state
    ) {
        if (pokemon_state.get_ability() == Ability::SnowWarning) {
            set_weather(Weather::HAIL, -1);
        }
    }

    void check_abilities(const bool player_goes_first) {
        // May suppress abilities
        check_mold_breaker(player_state, opponent_state);
        check_mold_breaker(opponent_state, player_state);
        // May change ability
        check_trace(player_state, opponent_state);
        check_trace(opponent_state, player_state);
        // May change stats
        check_download(player_state, opponent_state);
        check_download(opponent_state, player_state);
        check_intimidate(player_state, opponent_state);
        check_intimidate(opponent_state, player_state);

        if (player_goes_first) {
            check_drizzle(player_state);
            check_drought(player_state);
            check_sandstorm(player_state);
            check_snow_warning(player_state);

            check_drizzle(opponent_state);
            check_drought(opponent_state);
            check_sandstorm(opponent_state);
            check_snow_warning(opponent_state);
        } else {
            check_drizzle(opponent_state);
            check_drought(opponent_state);
            check_sandstorm(opponent_state);
            check_snow_warning(opponent_state);

            check_drizzle(player_state);
            check_drought(player_state);
            check_sandstorm(player_state);
            check_snow_warning(player_state);
        }
    }

    BestMove choose_move_against_defender(
        bool attacker_is_player,
        const bool chosen_move_only,
        const Weather weather,
        bool is_mid_turn,
        bool checking_future
    ) {
        PokemonState& attacker_state =
            attacker_is_player ? player_state : opponent_state;
        PokemonState& defender_state =
            attacker_is_player ? opponent_state : player_state;

        const auto attacker_chosen_move = attacker_state.get_chosen_move();
        const auto defender_chosen_move = defender_state.get_chosen_move();
        if (attacker_state.is_charging()) {
            return attacker_chosen_move;
        }

        if (check_transform(attacker_state)) {
            return attacker_state.get_chosen_move();
        }

        // Struggle
        const auto attacker_item = attacker_state.get_item_for_effect();
        if (!attacker_state.has_power_points()) {
            constexpr static auto struggle = MoveInfo{
                .name = "Struggle",
                .move = Move::Struggle,
                .type = PokemonType::COUNT,
                .category = Category::PHYSICAL,
                .power = 50,
                .accuracy = 100,
                .power_points = 999,
                .effect_percent = 0
            };
            attacker_state.set_chosen_move(
                BestMove{
                    .move = &struggle,
                    .damage = attacker_state.get_damage_of_attacker_move(
                        attacker_item,
                        &struggle,
                        defender_state,
                        weather,
                        is_mid_turn
                    ),
                    .times_to_hit = 1
                }
            );
            return attacker_state.get_chosen_move();
        }

        if ((is_mid_turn || chosen_move_only) &&
            attacker_chosen_move.move != nullptr
        ) {
            recalculate_chosen_move_damage(
                attacker_state,
                defender_state,
                attacker_chosen_move,
                weather,
                is_mid_turn
            );
            return attacker_state.get_chosen_move();
        }

        // Cases where a move can't be used
        BestMove best_move{};
        const auto attacker_ability = attacker_state.get_ability();
        if (attacker_state.is_recharging() ||
            (attacker_ability == Ability::Truant && attacker_state.is_truant())
        ) {
            attacker_state.done_recharging();
            attacker_state.set_chosen_move(std::move(best_move));
            return attacker_state.get_chosen_move();
        }

        // Cases where a move is ready
        if (attacker_state.get_field_location() != FieldLocation::ON_FIELD ||
            attacker_state.get_multi_turn_move_counter() > 0
        ) {
            recalculate_chosen_move_damage(
                attacker_state,
                defender_state,
                attacker_chosen_move,
                weather,
                is_mid_turn
            );
            return attacker_state.get_chosen_move();
        }

        uint damage_from_defender;
        if (attacker_is_player) {
            // Since player's move is calculated first,
            // we should predict what the opponent's next move is
            auto backup = defender_state.get_chosen_move();
            damage_from_defender =
                choose_move_against_defender(
                    !attacker_is_player,
                    false,
                    weather,
                    is_mid_turn,
                    true
                ).damage;
            defender_state.set_chosen_move(std::move(backup));
        } else {
            damage_from_defender = defender_chosen_move.damage;
        }

        const auto attacker_health = attacker_state.get_health();
        const auto defender_health = defender_state.get_health();
        const bool attacker_faster = attacker_state.outspeeds(
            defender_state,
            nullptr,
            nullptr,
            weather
        );

        // Check for move that does the most damage
        BestMove best_fire_move{};
        BestMove best_water_move{};
        bool best_move_must_charge = false;
        const MoveInfo* weather_ball = nullptr;
        const auto defender_move = defender_chosen_move.move;
        const auto defender_ability = defender_state.get_ability();
        bool unable_to_hit_defender =
            (
                defender_move != nullptr &&
                !attacker_faster &&
                (move_has_flag(
                        defender_move->move,
                        MoveFlag::GOES_UNDER_GROUND
                    ) ||
                    move_has_flag(
                        defender_move->move,
                        MoveFlag::GOES_UNDER_WATER
                    ) ||
                    move_has_flag(
                        defender_move->move,
                        MoveFlag::GOES_INTO_VOID
                    ) ||
                    move_has_flag(
                        defender_move->move,
                        MoveFlag::GOES_INTO_AIR)
                )
            ) ||
            (attacker_faster &&
                defender_state.get_field_location() !=
                FieldLocation::ON_FIELD
            ) ||
            defender_ability == Ability::WonderGuard;
        const auto has_choice_item = CHOICE_ITEMS.contains(attacker_item);
        auto attackers_last_used_move = attacker_state.get_last_used_move();
        for (const auto& attacker_move :
             attacker_state.get_moves()
        ) {
            if (attacker_state.pokemon->ability == Ability::SwiftSwim &&
                attacker_state.pokemon->name == Pokemon::Magikarp &&
                defender_state.pokemon->ability == Ability::SwiftSwim &&
                defender_state.pokemon->name == Pokemon::Magikarp &&
                attacker_state.is_player &&
                (attacker_move->move == Move::Flail ||
                    attacker_move->move == Move::SecretPower)
            ) {
                volatile int a = 0;
            }

            if (has_choice_item &&
                attacker_state.is_choiced() &&
                attackers_last_used_move.move != nullptr &&
                attackers_last_used_move.move->move != attacker_move->move
            ) {
                continue;
            }
            if (should_skip_move(
                    attacker_state,
                    attacker_move,
                    defender_chosen_move
                )
            ) {
                continue;
            }
            uint16_t damage = 0;
            if (attacker_move->move == Move::WeatherBall) {
                weather_ball = attacker_move;
                damage = attacker_state.get_weather_ball_damage(
                    attacker_move,
                    defender_state,
                    weather,
                    is_mid_turn
                );
            } else {
                damage = attacker_state.get_damage_of_attacker_move(
                    attacker_item,
                    attacker_move,
                    defender_state,
                    weather,
                    is_mid_turn
                );
            }

            if (check_fake_out(
                    attacker_state,
                    attacker_move,
                    damage,
                    is_first_turn()
                )
            ) {
                return attacker_state.get_chosen_move();
            }
            if (attacker_move->move == Move::FakeOut) {
                continue;
            }

            if ((attacker_move->move == Move::Rollout ||
                    attacker_move->move == Move::IceBall) &&
                attacker_state.get_rollout_turns() > 0) {
                best_move.move = attacker_move;
                best_move.damage = damage;
                best_move.times_to_hit = 1;
                attacker_state.set_chosen_move(std::move(best_move));
                return attacker_state.get_chosen_move();
            }

            uint8_t times_to_hit =
                get_times_to_hit(attacker_is_player, attacker_move);

            if (has_choice_item &&
                attacker_state.is_choiced()
            ) {
                best_move.move = attacker_move;
                best_move.damage = damage;
                best_move.times_to_hit = times_to_hit;
                break;
            }

            const int8_t attacker_move_priority =
                get_move_priority(attacker_move);

            const int8_t defender_move_priority =
                get_move_priority(defender_move);

            if (attacker_move_priority > defender_move_priority &&
                damage >= defender_health &&
                attacker_state.get_rollout_turns() == 0
            ) {
                attacker_state.set_chosen_move(
                    BestMove{
                        .move = attacker_move,
                        .damage = damage,
                        .times_to_hit = times_to_hit
                    }
                );
                return attacker_state.get_chosen_move();
            }

            // TODO rollout and ice ball
            const bool passes_priority_check =
                (attacker_move_priority < defender_move_priority &&
                    damage_from_defender < attacker_health) ||
                attacker_move_priority >= 0 ||
                best_move.move == nullptr;
            bool move_must_charge =
                does_move_have_to_charge(attacker_move, attacker_item, weather);
            const bool passes_charge_check = best_move.move == nullptr ||
            (((!move_must_charge &&
                        !best_move_must_charge &&
                        damage > best_move.damage) ||
                    (best_move_must_charge &&
                        damage > best_move.damage / 2) ||
                    (move_must_charge && attacker_faster &&
                        damage > best_move.damage * 2 &&
                        ((damage_from_defender < attacker_health &&
                                attacker_faster) ||
                            (damage_from_defender < attacker_health / 2) ||
                            best_move.damage == 0))) &&
                !(move_must_charge &&
                    attacker_ability == Ability::Truant));

            if ((passes_priority_check && passes_charge_check)) {
                best_move.damage = damage;
                best_move.move = attacker_move;
                best_move.times_to_hit = times_to_hit;
                best_move_must_charge = move_must_charge;
                if (attacker_move->type == PokemonType::FIRE) {
                    best_fire_move.move = attacker_move;
                    best_fire_move.damage = damage;
                    best_fire_move.times_to_hit = times_to_hit;
                } else if (attacker_move->type == PokemonType::WATER) {
                    best_water_move.move = attacker_move;
                    best_water_move.damage = damage;
                    best_water_move.times_to_hit = times_to_hit;
                }
            } else if (best_move.move == nullptr) {
                if (attacker_move->move == Move::SolarBeam ||
                    attacker_move->move == Move::Counter ||
                    attacker_move->move == Move::MirrorCoat ||
                    attacker_move->move == Move::MetalBurst ||
                    attacker_move->move == Move::Fling ||
                    (get_effectiveness(
                        defender_state.get_types(),
                        attacker_move->type
                    ) == 0) ||
                    (attacker_move->type == PokemonType::GROUND &&
                        (defender_ability == Ability::Levitate ||
                            defender_state.has_type(PokemonType::FLYING))) ||
                    // TODO these might go off after a second run through the pokemon
                    // ((attacker_move->move == Move::HiddenPower ||
                    //         attacker_move->move == Move::PoisonSting) &&
                    //     damage == 0) ||
                    (attacker_move->type == PokemonType::WATER &&
                        (defender_ability == Ability::WaterAbsorb ||
                            defender_ability == Ability::DrySkin)) ||
                    (attacker_move->type == PokemonType::ELECTRIC &&
                        (defender_ability == Ability::VoltAbsorb ||
                            defender_ability == Ability::MotorDrive)) ||
                    (attacker_move->type == PokemonType::FIRE &&
                        defender_ability == Ability::FlashFire) ||
                    (attacker_move->type == PokemonType::NORMAL &&
                        defender_state.has_type(PokemonType::GHOST)) ||
                    (attacker_move->move == Move::NaturalGift &&
                        !BERRIES.at(static_cast<int>(attacker_item))) ||
                    (attacker_move->move == Move::HiddenPower) ||
                    (attacker_move->move == Move::Endeavor &&
                        defender_state.get_health() <=
                        attacker_state.get_health()
                    ) ||
                    (attacker_move->move == Move::FalseSwipe &&
                        defender_health == 1) ||
                    (move_has_flag(
                            attacker_move->move,
                            MoveFlag::IS_SOUND_BASED
                        ) &&
                        defender_ability == Ability::Soundproof) ||
                    (attacker_move->move == Move::SpitUp &&
                        damage == 0) ||
                    unable_to_hit_defender
                ) {
                    best_move = {
                        attacker_move,
                        damage,
                        1
                    };
                } else {
                    throw std::logic_error{"Move skipped when none picked"};
                }
            }
        }

        uint damage_to_defender = 0;
        if (best_move.move != nullptr &&
            best_move.move->move != Move::TripleKick
        ) {
            damage_to_defender = best_move.damage * best_move.times_to_hit;
        } else if (best_move.move != nullptr) {
            for (int i = 0; i < 3; i++) {
                MoveInfo temp = *best_move.move;
                temp.power = static_cast<int16_t>((i + 1) * 10);
                damage_to_defender +=
                    attacker_state.get_damage_of_attacker_move(
                        attacker_item,
                        &temp,
                        defender_state,
                        weather,
                        is_mid_turn
                    );
            }
        }

        const auto& attacker_pokemon = attacker_state.pokemon;
        uint hits_to_defender;
        if (damage_to_defender != 0) {
            hits_to_defender = defender_health / damage_to_defender;
        } else {
            auto attacker_name = attacker_pokemon->name;
            auto defender_name = defender_state.pokemon->name;
            if (((attacker_name == Pokemon::Wobbuffet ||
                    attacker_name == Pokemon::Wynaut)) ||
                ((attacker_name == Pokemon::Lanturn ||
                        attacker_name == Pokemon::Chinchou ||
                        attacker_name == Pokemon::Rotom ||
                        attacker_name == Pokemon::Drifblim) &&
                    (defender_state.pokemon->ability == Ability::MotorDrive)) ||
                ((attacker_name == Pokemon::Houndoom) &&
                    (defender_ability == Ability::FlashFire)) ||
                ((attacker_name == Pokemon::Zigzagoon ||
                        attacker_name == Pokemon::Furret ||
                        attacker_name == Pokemon::Riolu ||
                        attacker_name == Pokemon::Tyrogue ||
                        attacker_name == Pokemon::Machop ||
                        attacker_name == Pokemon::Machamp ||
                        attacker_name == Pokemon::Lucario ||
                        attacker_name == Pokemon::Hippopotas ||
                        attacker_name == Pokemon::Trapinch ||
                        attacker_name == Pokemon::Diglett ||
                        attacker_name == Pokemon::Cubone ||
                        attacker_name == Pokemon::Sandshrew ||
                        attacker_name == Pokemon::Phanpy ||
                        attacker_name == Pokemon::Marowak ||
                        attacker_name == Pokemon::Dugtrio ||
                        attacker_name == Pokemon::Donphan ||
                        attacker_name == Pokemon::Hippowdon ||
                        attacker_name == Pokemon::Graveler ||
                        attacker_name == Pokemon::WormadamS ||
                        attacker_name == Pokemon::Rhyperior ||
                        attacker_name == Pokemon::Croagunk ||
                        attacker_name == Pokemon::Toxicroak ||
                        attacker_name == Pokemon::Rhydon ||
                        attacker_name == Pokemon::Sandslash ||
                        attacker_name == Pokemon::Geodude ||
                        attacker_name == Pokemon::Bonsly ||
                        attacker_name == Pokemon::Golem ||
                        attacker_name == Pokemon::Rhyhorn) &&
                    defender_state.has_type(PokemonType::GHOST) &&
                    defender_ability == Ability::Levitate) ||
                ((attacker_name == Pokemon::Castform ||
                        attacker_name == Pokemon::Umbreon ||
                        attacker_name == Pokemon::Magikarp) &&
                    defender_state.has_type(PokemonType::GHOST)) ||
                ((attacker_name == Pokemon::Barboach ||
                        attacker_name == Pokemon::Wooper ||
                        attacker_name == Pokemon::Marshtomp ||
                        attacker_name == Pokemon::Quagsire ||
                        attacker_name == Pokemon::Swampert) &&
                    defender_state.has_type(PokemonType::FLYING) &&
                    defender_ability == Ability::WaterAbsorb) ||
                (attacker_state.get_ability() == Ability::Normalize &&
                    defender_state.has_type(PokemonType::GHOST)) ||
                (attacker_name == Pokemon::Unown) ||
                (attacker_name == Pokemon::Chatot &&
                    defender_ability == Ability::Soundproof) ||
                (attacker_name == Pokemon::Suicune &&
                    defender_name == Pokemon::AlteredGiratina) ||
                (attacker_name == Pokemon::Ditto &&
                    defender_name == Pokemon::Magikarp) ||
                (attacker_name == Pokemon::Magikarp &&
                    defender_name == Pokemon::Wobbuffet) ||
                (attacker_name == Pokemon::Magikarp &&
                    defender_name == Pokemon::Smeargle) ||
                (attacker_name == Pokemon::Castform &&
                    defender_name == Pokemon::Shuckle) ||
                (attacker_state.is_choiced() &&
                    best_move.move->type == PokemonType::GROUND &&
                    defender_ability == Ability::Levitate) ||
                (attacker_name == Pokemon::Smeargle) ||
                ((attacker_name == Pokemon::Geodude ||
                        attacker_name == Pokemon::Phanpy) &&
                    (defender_name == Pokemon::Unown)) ||
                ((attacker_name == Pokemon::Claydol ||
                        attacker_name == Pokemon::Shuckle) &&
                    (defender_name == Pokemon::Shuckle ||
                        defender_name == Pokemon::Bronzor)) ||
                ((attacker_name == Pokemon::Dugtrio ||
                        attacker_name == Pokemon::Shuckle ||
                        attacker_name == Pokemon::WormadamS ||
                        attacker_name == Pokemon::Bronzor) &&
                    (defender_name == Pokemon::Claydol)) ||
                unable_to_hit_defender
            ) {
                hits_to_defender = std::numeric_limits<uint>::max();
            } else {
                if (!attacker_state.has_power_points()) {
                    throw std::logic_error("Missing move ");
                }
            }
        }

        // Check if it is better to use a status move
        if (!has_choice_item) {
            for (const auto& move : attacker_pokemon->moves) {
                if (should_skip_move(
                        attacker_state,
                        move,
                        defender_chosen_move
                    )
                ) {
                    continue;
                }
                // TODO foresight
                if (move_has_flag(
                        move->move,
                        MoveFlag::BOOSTS_ATTACKER_STAT)
                ) {
                    int turns_to_add = attacker_faster ? 0 : 1;
                    int turns_for_double_boost = 2 + turns_to_add;
                    int turns_for_single_boost = 4 + turns_to_add;
                    if (best_move.move->category == Category::PHYSICAL) {
                        if (attacker_state.get_attack_stage() < 6) {
                            if (move->move == Move::SwordsDance) {
                                if (hits_to_defender > turns_for_double_boost) {
                                    best_move.move = move;
                                    best_move.damage = 0;
                                    best_move.times_to_hit = 1;
                                }
                            } else if (move->move == Move::Meditate ||
                                move->move == Move::Sharpen ||
                                move->move == Move::Howl ||
                                move->move == Move::BulkUp ||
                                move->move == Move::DragonDance
                            ) {
                                if (hits_to_defender > turns_for_single_boost) {
                                    best_move.move = move;
                                    best_move.damage = 0;
                                    best_move.times_to_hit = 1;
                                }
                            }
                        }
                    }
                    if (best_move.move->category == Category::SPECIAL) {
                        if (attacker_state.get_special_attack_stage() < 6) {
                            if (move->move == Move::NastyPlot) {
                                if (hits_to_defender > turns_for_double_boost) {
                                    best_move.move = move;
                                    best_move.damage = 0;
                                    best_move.times_to_hit = 1;
                                }
                            }
                            if (move->move == Move::Growth) {
                                if (hits_to_defender > turns_for_single_boost) {
                                    best_move.move = move;
                                    best_move.damage = 0;
                                    best_move.times_to_hit = 1;
                                }
                            }
                        }
                    }
                    // Agility, double team, minimize, bulk up, rock polish
                    // are ignored

                    // TODO check if defending is worth it
                    // single boost is good if defender hits 2 times
                    // double boost is good if defender hits 1 times

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

        // Healing may be better too
        const auto defenders_last_used_move =
            defender_state.get_last_used_move();
        for (const auto& move : attacker_pokemon->moves) {
            if (should_skip_move(
                    attacker_state,
                    move,
                    defender_chosen_move
                )
            ) {
                continue;
            }
            if (move_has_flag(
                    move->move,
                    MoveFlag::HEALS_ATTACKER
                )
            ) {
                uint potential_hp_gain = 0;

                if (move->move == Move::Absorb ||
                    move->move == Move::MegaDrain ||
                    move->move == Move::LeechLife ||
                    move->move == Move::GigaDrain ||
                    move->move == Move::DrainPunch
                ) {
                    auto damage =
                        attacker_state.get_damage_of_attacker_move(
                            attacker_item,
                            move,
                            defender_state,
                            weather,
                            is_mid_turn
                        );

                    potential_hp_gain = damage / 2;
                    if (attacker_item == Item::BigRoot) {
                        potential_hp_gain = std::floor(damage * 0.65);
                    }
                    potential_hp_gain = std::max(1u, potential_hp_gain);
                    if (defender_ability == Ability::LiquidOoze) {
                        potential_hp_gain = -potential_hp_gain;
                    }
                    if (attacker_health < attacker_state.max_health / 2 &&
                        defenders_last_used_move.damage < attacker_state.
                        max_health
                        / 2
                    ) {
                        best_move.move = move;
                        best_move.damage = damage;
                        best_move.times_to_hit = 1;
                    }
                }

                if (move->move == Move::Recover ||
                    move->move == Move::Softboiled ||
                    move->move == Move::MilkDrink ||
                    move->move == Move::SlackOff ||
                    move->move == Move::HealOrder
                ) {
                    potential_hp_gain = attacker_state.max_health / 2;
                }
                if (move->move == Move::MorningSun ||
                    move->move == Move::Synthesis ||
                    move->move == Move::Moonlight
                ) {
                    if (weather == Weather::SUN) {
                        potential_hp_gain = 2 * attacker_state.max_health / 3;
                    } else if (weather == Weather::CLEAR) {
                        potential_hp_gain = attacker_state.max_health / 2;
                    } else {
                        potential_hp_gain = attacker_state.max_health / 4;
                    }
                    if (attacker_health < attacker_state.max_health / 2 &&
                        defenders_last_used_move.damage <
                        attacker_state.max_health / 2
                    ) {
                        best_move.move = move;
                        best_move.damage = 0;
                        best_move.times_to_hit = 1;
                    }
                }
            }
        }

        // Weather
        for (const auto& move : attacker_pokemon->moves) {
            if (should_skip_move(
                    attacker_state,
                    move,
                    defender_chosen_move
                )
            ) {
                continue;
            }
            if (move_has_flag(
                    move->move,
                    MoveFlag::CHANGES_WEATHER
                )
            ) {
                if (best_move.damage > 0) {
                    uint damage_to_attacker = 0;
                    if (defender_move != nullptr) {
                        if (defender_move->move != Move::TripleKick) {
                            damage_to_attacker =
                                defender_chosen_move.damage *
                                best_move.times_to_hit;
                        } else {
                            for (int i = 0; i < 3; i++) {
                                MoveInfo temp =
                                    *defender_move;
                                temp.power = (i + 1) * 10;
                                damage_to_attacker +=
                                    defender_state.
                                    get_damage_of_attacker_move(
                                        defender_state.get_item_for_effect(
                                        ),
                                        &temp,
                                        defender_state,
                                        weather,
                                        is_mid_turn
                                    );
                            }
                        }
                    }

                    if (move->move == Move::SunnyDay &&
                        weather != Weather::SUN
                    ) {
                        if (best_fire_move.damage > 0) {
                            const auto hits_to_ko_if_fire =
                                defender_health / best_fire_move.damage;
                            if (hits_to_ko_if_fire > hits_to_defender + 1) {
                                best_move.move = move;
                                best_move.damage = 0;
                                best_move.times_to_hit = 1;
                            }
                        }

                        if (best_move.move->move == Move::SolarBeam) {
                            if (hits_to_defender > 1) {
                                best_move.move = move;
                                best_move.damage = 0;
                                best_move.times_to_hit = 1;
                            }
                        }

                        if (defender_chosen_move.damage > 0) {
                            const auto hits_taken =
                                attacker_health / damage_to_attacker;
                            if (hits_taken > 3 &&
                                defender_move->type == PokemonType::WATER
                            ) {
                                best_move.move = move;
                                best_move.damage = 0;
                                best_move.times_to_hit = 1;
                            }
                        }

                        if (weather_ball != nullptr) {
                            const int damage =
                                attacker_state.get_weather_ball_damage(
                                    weather_ball,
                                    defender_state,
                                    Weather::SUN,
                                    is_mid_turn
                                );
                            const auto new_hits_to_KO =
                                defender_health / damage;
                            if (new_hits_to_KO > hits_to_defender + 1) {
                                best_move.move = move;
                                best_move.damage = 0;
                                best_move.times_to_hit = 1;
                            }
                        }
                    } else if (move->move == Move::RainDance &&
                        weather != Weather::RAIN
                    ) {
                        if (best_water_move.damage > 0) {
                            const auto hits_to_ko_if_water =
                                defender_health / best_water_move.damage;
                            if (hits_to_ko_if_water > hits_to_defender + 1) {
                                best_move.move = move;
                                best_move.damage = 0;
                                best_move.times_to_hit = 1;
                            }
                        }

                        if (defender_chosen_move.damage > 0) {
                            const auto hits_taken =
                                attacker_health / damage_to_attacker;
                            if (hits_taken > 3 &&
                                defender_move->type == PokemonType::FIRE
                            ) {
                                best_move.move = move;
                                best_move.damage = 0;
                                best_move.times_to_hit = 1;
                            }
                        }

                        if (weather_ball != nullptr) {
                            const int damage =
                                attacker_state.get_weather_ball_damage(
                                    weather_ball,
                                    defender_state,
                                    Weather::RAIN,
                                    is_mid_turn
                                );
                            const auto new_hits_to_KO =
                                defender_health / damage;
                            if (new_hits_to_KO > hits_to_defender + 1) {
                                best_move.move = move;
                                best_move.damage = 0;
                                best_move.times_to_hit = 1;
                            }
                        }
                    } else if (move->move == Move::Hail &&
                        weather != Weather::HAIL
                    ) {
                        if (weather_ball != nullptr) {
                            const int damage =
                                attacker_state.get_weather_ball_damage(
                                    weather_ball,
                                    defender_state,
                                    Weather::HAIL,
                                    is_mid_turn
                                );
                            if (const auto new_hits_to_KO =
                                    defender_health / damage;
                                new_hits_to_KO > hits_to_defender
                            ) {
                                best_move.move = move;
                                best_move.damage = 0;
                                best_move.times_to_hit = 1;
                            }
                        }
                    } else if (move->move == Move::Sandstorm &&
                        weather != Weather::SANDSTORM
                    ) {
                        if (weather_ball != nullptr) {
                            const int damage =
                                attacker_state.get_weather_ball_damage(
                                    weather_ball,
                                    defender_state,
                                    Weather::SANDSTORM,
                                    is_mid_turn
                                );
                            if (const auto new_hits_to_KO =
                                    defender_health / damage;
                                new_hits_to_KO > hits_to_defender
                            ) {
                                best_move.move = move;
                                best_move.damage = 0;
                                best_move.times_to_hit = 1;
                            }
                        }
                        if (defender_chosen_move.damage > 0) {
                            const auto hits_taken =
                                attacker_health /
                                defender_chosen_move.damage;
                            if (hits_taken > 3 &&
                                defender_state.has_type(PokemonType::ROCK)
                            ) {
                                best_move.move = move;
                                best_move.damage = 0;
                                best_move.times_to_hit = 1;
                            }
                        }
                    }
                }
            }
        }

        if (has_choice_item &&
            !attacker_state.is_choiced() &&
            best_move.move != nullptr &&
            !checking_future
        ) {
            attacker_state.set_is_choiced();
            attacker_state.set_chosen_move(std::move(best_move));
            return attacker_state.get_chosen_move();
        }
        if (has_choice_item &&
            attacker_state.is_choiced() &&
            attackers_last_used_move.move != nullptr
        ) {
            assert(attackers_last_used_move.move->move == best_move.move->move);
            attacker_state.set_chosen_move(std::move(best_move));
            return attacker_state.get_chosen_move();
        }
        if (defender_state.get_field_location() != FieldLocation::ON_FIELD &&
            attacker_faster &&
            best_move.move == nullptr
        ) {
            // For metronome since no move was picked
            attacker_state.set_chosen_move(
                BestMove{
                    .move = attackers_last_used_move.move,
                    .damage = 0,
                    .times_to_hit = attackers_last_used_move.times_to_hit
                }
            );
            return attacker_state.get_chosen_move();
        }

        attacker_state.set_chosen_move(std::move(best_move));

        if (attacker_state.get_chosen_move().move == nullptr) {
            for (const auto move : attacker_state.get_moves()) {
                if (attacker_state.has_power_points(move->move)) {
                    attacker_state.set_chosen_move(
                        BestMove{
                            .move = move,
                            .damage = 0,
                            .times_to_hit =
                            get_times_to_hit(attacker_is_player, move)
                        }
                    );
                    return attacker_state.get_chosen_move();
                }
            }
            throw std::runtime_error("Struggle was not picked");
        }
        return attacker_state.get_chosen_move();
    }

    void execute_move(
        const bool attacker_is_player,
        const Weather weather,
        const bool is_mid_turn
    ) {
        PokemonState& attacker_state =
            attacker_is_player ? player_state : opponent_state;
        PokemonState& defender_state =
            attacker_is_player ? opponent_state : player_state;

        auto attacker_chosen_move = attacker_state.get_chosen_move();
        const auto defender_ability = defender_state.get_ability();
        if (attacker_chosen_move.move == nullptr) {
            attacker_state.update_last_used_move(true, defender_ability);
            return;
        }
        auto attacker_move = attacker_chosen_move.move->move;

        const auto defender_chosen_move = defender_state.get_chosen_move();
        if ((attacker_move == Move::FakeOut && !is_first_turn()) ||

            (attacker_move == Move::SuckerPunch &&
                (defender_chosen_move.move == nullptr ||
                    defender_chosen_move.move->category == Category::STATUS)) ||

            (defender_ability == Ability::Damp &&
                (attacker_move == Move::Selfdestruct ||
                    attacker_move == Move::Explosion)) ||

            (defender_ability == Ability::Soundproof &&
                move_has_flag(attacker_move, MoveFlag::IS_SOUND_BASED)) ||

            (attacker_move == Move::FocusPunch && attacker_state.was_hit()) ||

            (attacker_move == Move::Feint &&
                (defender_chosen_move.move == nullptr ||
                    (defender_chosen_move.move->move == Move::Protect ||
                        defender_chosen_move.move->move == Move::Detect))
            )
        ) {
            attacker_state.update_last_used_move(true, defender_ability);
            return;
        }

        // Charge move
        const bool requires_charging =
            move_has_flag(attacker_move, MoveFlag::REQUIRES_CHARGING_TURN);
        const auto attacker_item = attacker_state.get_item_for_effect();
        if (requires_charging && !attacker_state.is_charging()) {
            if (attacker_item == Item::PowerHerb) {
                attacker_state.clear_item();
            } else {
                attacker_state.start_charging();
                if (attacker_move == Move::SkullBash) {
                    attacker_state.change_stat_stage(
                        Stat::DEFENSE,
                        1,
                        false
                    );
                }
                attacker_state.update_last_used_move(true, defender_ability);
                return;
            }
        }
        if (attacker_state.is_charging()) {
            assert(requires_charging);
            attacker_state.done_charging();
            if (attacker_move == Move::SkullBash) {
                attacker_state.change_stat_stage(
                    Stat::DEFENSE,
                    -1,
                    false
                );
            }
        }

        // Rollout and Ice Ball
        if (attacker_move == Move::Rollout ||
            attacker_move == Move::IceBall
        ) {
            if (attacker_state.get_rollout_turns() == 0) {
                attacker_state.set_rollout_power(
                    get_rollout_power(
                        attacker_state,
                        attacker_chosen_move.move
                    )
                );
                attacker_state.start_rollout();
            }
        }

        // Weather changing moves
        if (move_has_flag(attacker_move, MoveFlag::CHANGES_WEATHER)) {
            int turns = 5;
            if (attacker_move == Move::Sandstorm) {
                if (attacker_item == Item::SmoothRock) {
                    turns = 8;
                }
                set_weather(Weather::SANDSTORM, turns);
            } else if (attacker_move == Move::RainDance) {
                if (attacker_item == Item::DampRock) {
                    turns = 8;
                }
                set_weather(Weather::RAIN, turns);
            } else if (attacker_move == Move::SunnyDay) {
                if (attacker_item == Item::HeatRock) {
                    turns = 8;
                }
                set_weather(Weather::SUN, turns);
            } else if (attacker_move == Move::Hail) {
                if (attacker_item == Item::IcyRock) {
                    turns = 8;
                }
                set_weather(Weather::HAIL, turns);
            }
        }

        if (!is_mid_turn &&
            attacker_move == Move::Snatch &&
            move_has_flag(
                defender_chosen_move.move->move,
                MoveFlag::CAN_BE_SNATCHED
            )
        ) {
            // Snatchable moves do no damage
            attacker_state.set_chosen_move(
                BestMove{
                    .move = defender_chosen_move.move,
                    .damage = 0,
                    .times_to_hit = 1
                }
            );
            // Defender can no longer attack
            defender_state.set_chosen_move(
                BestMove{
                    .move = nullptr,
                    .damage = 0,
                    .times_to_hit = 0
                }
            );
            attacker_chosen_move = attacker_state.get_chosen_move();
            attacker_move = attacker_chosen_move.move->move;
        }

        // Brick break
        if (attacker_move == Move::BrickBreak) {
            defender_state.break_reflect();
            defender_state.break_light_screen();
            recalculate_chosen_move_damage(
                attacker_state,
                defender_state,
                attacker_chosen_move,
                weather,
                is_mid_turn
            );
        }

        // Off field move
        bool attacker_vanished = false;
        if (attacker_state.get_field_location() == FieldLocation::ON_FIELD) {
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
                if (attacker_item == Item::PowerHerb) {
                    attacker_state.clear_item();
                } else {
                    if (goes_into_air) {
                        attacker_state.
                            set_field_location(FieldLocation::IN_AIR);
                        attacker_vanished = true;
                    } else if (goes_under_ground) {
                        attacker_state.set_field_location(
                            FieldLocation::UNDER_GROUND);
                        attacker_vanished = true;
                    } else if (goes_under_water) {
                        attacker_state.set_field_location(
                            FieldLocation::UNDER_WATER);
                        attacker_vanished = true;
                    } else if (goes_into_void) {
                        attacker_state.set_field_location(
                            FieldLocation::IN_THE_VOID);
                        attacker_vanished = true;
                    }
                }
            }
        }
        if (!attacker_vanished &&
            attacker_state.get_field_location() != FieldLocation::ON_FIELD
        ) {
            attacker_state.set_field_location(FieldLocation::ON_FIELD);
            recalculate_chosen_move_damage(
                defender_state,
                attacker_state,
                defender_chosen_move,
                weather,
                is_mid_turn
            );
        } else if (attacker_vanished) {
            recalculate_chosen_move_damage(
                defender_state,
                attacker_state,
                defender_chosen_move,
                weather,
                is_mid_turn
            );
        }

        if (defender_state.get_field_location() == FieldLocation::ON_FIELD &&
            attacker_chosen_move.move->move == Move::Transform
        ) {
            attacker_state.transform(defender_state);
            attacker_state.update_last_used_move(false, defender_ability);
            return;
        }

        if (attacker_state.was_hit() &&
            (attacker_move == Move::Avalanche ||
                attacker_move == Move::Payback ||
                attacker_move == Move::Revenge)
        ) {
            recalculate_chosen_move_damage(
                attacker_state,
                defender_state,
                attacker_chosen_move,
                weather,
                is_mid_turn
            );
            attacker_chosen_move = attacker_state.get_chosen_move();
            attacker_move = attacker_chosen_move.move->move;
        }

        if (const auto& attackers_last_used_move =
                attacker_state.get_last_used_move();
            attackers_last_used_move.move == nullptr ||
            (attackers_last_used_move.move != nullptr &&
                attacker_chosen_move.move != nullptr &&
                attackers_last_used_move.move->move == attacker_move)
        ) {
            attacker_state.increase_metronome();
        } else {
            attacker_state.clear_metronome();
        }

        // Do the damage
        if (attacker_state.get_field_location() == FieldLocation::ON_FIELD &&
            (move_has_flag(attacker_move, MoveFlag::BYPASSES_PROTECT) ||
                !defender_state.is_protected() ||
                attacker_move == Move::Feint) &&
            !(defender_state.get_field_location() != FieldLocation::ON_FIELD &&
                attacker_chosen_move.damage == 0)
        ) {
            // Apply the damage
            if ((defender_ability == Ability::VoltAbsorb &&
                    attacker_chosen_move.move->type == PokemonType::ELECTRIC) ||
                ((defender_ability == Ability::WaterAbsorb ||
                        defender_ability == Ability::DrySkin) &&
                    attacker_chosen_move.move->type == PokemonType::WATER)
            ) {
                defender_state.heal(
                    defender_state.max_health / 4
                );
            } else {
                const bool sashed =
                    defender_state.max_health == defender_state.get_health() &&
                    defender_state.get_item_for_effect() == Item::FocusSash;
                for (int i = 0; i < attacker_chosen_move.times_to_hit; i++) {
                    defender_state.apply_damage(attacker_chosen_move.damage);
                    if (move_has_flag(
                            attacker_move,
                            MoveFlag::HITS_MULTIPLE_TIMES)
                    ) {
                        if (attacker_move == Move::TripleKick &&
                            i < attacker_chosen_move.times_to_hit - 1
                        ) {
                            const auto backup =
                                attacker_chosen_move.move;
                            MoveInfo temp = *attacker_chosen_move.move;
                            temp.power = (i + 1) * 10 + 10;
                            recalculate_chosen_move_damage(
                                attacker_state,
                                defender_state,
                                BestMove{
                                    .move = &temp,
                                    .damage = 0,
                                    .times_to_hit = 1
                                },
                                weather,
                                is_mid_turn
                            );
                            attacker_chosen_move.damage +=
                                attacker_state.get_chosen_move().damage;
                            attacker_chosen_move.move = backup;
                        }
                    }
                    if (is_mid_turn &&
                        defender_state.get_last_used_move().move != nullptr &&
                        defender_state.get_last_used_move().move->move ==
                        Move::Rage
                    ) {
                        defender_state.change_stat_stage(
                            Stat::ATTACK,
                            1,
                            false
                        );
                    }
                }
                if (sashed && defender_state.get_health() <= 0) {
                    defender_state.apply_damage(
                        defender_state.get_health() - 1
                    );
                    defender_state.clear_item();
                }
                if (attacker_move == Move::FakeOut) {
                    defender_state.set_flinched();
                }
                if ((attacker_move == Move::Thief ||
                        attacker_move == Move::KnockOff ||
                        attacker_move == Move::Covet) &&
                    defender_ability != Ability::StickyHold &&
                    defender_ability != Ability::Multitype &&
                    attacker_state.try_set_item(defender_state.get_item())
                ) {
                    if (defender_state.get_item() == Item::StickyBarb ||
                        attacker_move == Move::Thief ||
                        attacker_move == Move::Covet
                    ) {
                        attacker_state.try_set_item(Item::StickyBarb);
                        attacker_state.try_apply_berry(true);
                    }
                    defender_state.clear_item(true);
                }
            }

            if (attacker_chosen_move.damage > 0) {
                defender_state.set_was_hit();
                if (move_has_flag(attacker_move, MoveFlag::CONTINUES)) {
                    if (attacker_state.is_player) {
                        defender_state.set_trapped_counter(2);
                    } else {
                        defender_state.set_trapped_counter(5);
                    }
                }
                if (attacker_move == Move::WakeUpSlap &&
                    defender_state.get_status() == Status::SLEEP) {
                    defender_state.clear_status();
                }

                if (defender_chosen_move.move != nullptr) {
                    const auto attacker_move_category =
                        attacker_chosen_move.move->category;
                    if (defender_chosen_move.move->move == Move::Counter) {
                        if (attacker_move_category == Category::PHYSICAL) {
                            defender_state.set_chosen_move(
                                BestMove{
                                    .move = defender_chosen_move.move,
                                    .damage = static_cast<uint16_t>(
                                        attacker_chosen_move.damage * 2
                                    ),
                                    .times_to_hit = 1
                                }
                            );
                        }
                    } else if (
                        defender_chosen_move.move->move == Move::MirrorCoat
                    ) {
                        if (attacker_move_category == Category::SPECIAL) {
                            defender_state.set_chosen_move(
                                BestMove{
                                    .move = defender_chosen_move.move,
                                    .damage = static_cast<uint16_t>(
                                        attacker_chosen_move.damage * 2
                                    ),
                                    .times_to_hit = 1
                                }
                            );
                        }
                    } else {
                        // Several moves do more damage after getting hit
                        recalculate_chosen_move_damage(
                            defender_state,
                            attacker_state,
                            defender_chosen_move,
                            weather,
                            is_mid_turn
                        );
                    }
                }
            }
            if (defender_state.get_health() < 0) {
                attacker_state.set_chosen_move(
                    BestMove{
                        .move = attacker_chosen_move.move,
                        .damage = static_cast<uint16_t>(
                            attacker_chosen_move.damage +
                            defender_state.get_health()
                        ),
                        .times_to_hit = 1
                    }
                );
            }
            attacker_state.update_last_used_move(false, defender_ability);
        } else {
            attacker_state.update_last_used_move(true, defender_ability);
        }
    }

    void apply_end_of_turn_effects() {
        player_state.apply_end_of_turn_effects(
            get_weather(),
            opponent_state
        );
        opponent_state.apply_end_of_turn_effects(
            get_weather(),
            player_state
        );
    }

    void apply_post_move_effects(
        PokemonState& attacker_state,
        const BestMove& attacker_move,
        PokemonState& defender_state
    ) const {
        if (attacker_move.move == nullptr) {
            return;
        }
        const auto move = attacker_move.move->move;
        const auto attacker_ability = attacker_state.get_ability();
        const auto defender_ability = defender_state.get_ability();

        const bool apply_effect = defender_ability != Ability::ShieldDust;

        if (defender_ability == Ability::FlashFire &&
            attacker_move.move->type == PokemonType::FIRE
        ) {
            defender_state.set_flash_fire();
        } else if (defender_ability == Ability::MotorDrive &&
            attacker_move.move->type == PokemonType::ELECTRIC
        ) {
            defender_state.change_stat_stage(Stat::SPEED, 1, true);
        }

        // Protect
        if (move_has_flag(move, MoveFlag::BREAKS_PROTECT)) {
            defender_state.clear_protect();
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
                uint health_gained = attacker_move.damage / 2;

                if (attacker_state.get_item_for_effect() == Item::BigRoot) {
                    health_gained = std::floor(attacker_move.damage * 0.65);
                }

                health_gained = std::max(1u, health_gained);
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
                if (const auto weather = get_weather();
                    weather == Weather::SUN
                ) {
                    attacker_state.heal(
                        2 * attacker_state.max_health / 3
                    );
                } else if (weather == Weather::CLEAR) {
                    attacker_state.heal(attacker_state.max_health / 2);
                } else {
                    attacker_state.heal(attacker_state.max_health / 4);
                }
            }
            if (attacker_state.get_item_for_effect() == Item::ShellBell) {
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
                attacker_state.
                    increment_multi_turn_move_counter(opponent_turns);
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
                attacker_state.
                    increment_multi_turn_move_counter(opponent_turns);
            }
        }

        if (move == Move::BugBite || move == Move::Pluck) {
            if (BERRIES[
                    static_cast<int>(defender_state.get_item_for_effect())
                ] &&
                defender_ability != Ability::StickyHold
            ) {
                if (attacker_ability != Ability::Klutz) {
                    attacker_state.eat_berry(
                        defender_state.get_item_for_effect()
                    );
                }
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
            } else if (move == Move::Struggle) {
                attacker_state.apply_damage(attacker_state.max_health / 4);
            }
        }

        // Contact moves
        const auto weather = get_weather();
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
            if (defender_state.get_item_for_effect() == Item::RockyHelmet) {
                attacker_state.apply_damage(attacker_state.max_health / 6);
            } else if (defender_state.get_item_for_effect() == Item::StickyBarb
                &&
                attacker_state.try_set_item(Item::StickyBarb)
            ) {
                defender_state.clear_item();
            }
        }

        // Life orb
        if (attacker_ability != Ability::MagicGuard &&
            attacker_state.get_item_for_effect() == Item::LifeOrb &&
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
            (is_fling && attacker_state.get_item_for_effect() ==
                Item::FlameOrb);
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
                attacker_state.get_item_for_effect() == Item::PoisonBarb;
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
        if (move == Move::TriAttack && defender_state.is_player &&
            apply_effect) {
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
                attacker_state.change_stat_stage(
                    Stat::ATTACK,
                    1,
                    false
                );
            }
            if (move == Move::SwordsDance) {
                attacker_state.change_stat_stage(
                    Stat::ATTACK,
                    2,
                    false
                );
            }
            if (move == Move::BellyDrum) {
                attacker_state.change_stat_stage(
                    Stat::ATTACK,
                    6,
                    false
                );
            }
            if (!attacker_state.is_player &&
                (move == Move::MeteorMash ||
                    move == Move::MetalClaw) && apply_effect
            ) {
                attacker_state.change_stat_stage(
                    Stat::ATTACK,
                    1,
                    false
                );
            }
        }

        if (move_has_flag(move, MoveFlag::LOWERS_ATTACKERS_ATTACK)) {
            attacker_state.change_stat_stage(
                Stat::ATTACK,
                -1,
                false
            );
        }

        if (move_has_flag(move, MoveFlag::RAISES_DEFENDER_ATTACK)) {
            defender_state.change_stat_stage(
                Stat::ATTACK,
                2,
                false
            );
        }

        if (move_has_flag(move, MoveFlag::LOWERS_DEFENDER_ATTACK)) {
            if (move == Move::Tickle ||
                move == Move::Growl
            ) {
                defender_state.change_stat_stage(
                    Stat::ATTACK,
                    -1,
                    true
                );
            }
            if (move == Move::Charm ||
                move == Move::Memento ||
                move == Move::Featherdance
            ) {
                defender_state.change_stat_stage(
                    Stat::ATTACK,
                    -2,
                    true
                );
            }
            if (defender_state.is_player &&
                move == Move::AuroraBeam && apply_effect
            ) {
                defender_state.change_stat_stage(
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
                attacker_state.change_stat_stage(
                    Stat::DEFENSE,
                    1,
                    false
                );
            }
            if (move == Move::AcidArmor ||
                move == Move::IronDefense
            ) {
                attacker_state.change_stat_stage(
                    Stat::DEFENSE,
                    2,
                    false
                );
            }
        }
        if (move_has_flag(move, MoveFlag::LOWERS_ATTACKERS_DEFENSE)) {
            attacker_state.change_stat_stage(
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
                defender_state.change_stat_stage(
                    Stat::DEFENSE,
                    -1,
                    true
                );
            }
            if (move == Move::TailWhip ||
                move == Move::Leer ||
                move == Move::Tickle
            ) {
                defender_state.change_stat_stage(
                    Stat::DEFENSE,
                    -1,
                    true
                );
            }
            if (move == Move::Screech) {
                defender_state.change_stat_stage(
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
                attacker_state.change_stat_stage(
                    Stat::SPECIAL_ATTACK,
                    1,
                    false
                );
            }
            if (move == Move::TailGlow ||
                move == Move::NastyPlot
            ) {
                attacker_state.change_stat_stage(
                    Stat::SPECIAL_ATTACK,
                    2,
                    false
                );
            }
        }

        if (move_has_flag(move, MoveFlag::LOWERS_ATTACKERS_SPECIAL_ATTACK)) {
            attacker_state.change_stat_stage(
                Stat::SPECIAL_ATTACK,
                -2,
                false
            );
        }

        if (move_has_flag(move, MoveFlag::RAISES_DEFENDER_SPECIAL_ATTACK)) {
            defender_state.change_stat_stage(
                Stat::SPECIAL_ATTACK,
                2,
                false
            );
        }

        if (move_has_flag(move, MoveFlag::LOWERS_DEFENDER_SPECIAL_ATTACK)) {
            if (defender_state.is_player &&
                move == Move::MistBall
            ) {
                defender_state.change_stat_stage(
                    Stat::SPECIAL_ATTACK,
                    -1,
                    true
                );
            }
            if (move == Move::Memento) {
                defender_state.change_stat_stage(
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
                attacker_state.change_stat_stage(
                    Stat::DEFENSE,
                    1,
                    false
                );
            }
            if (move == Move::Amnesia) {
                attacker_state.change_stat_stage(
                    Stat::DEFENSE,
                    2,
                    false
                );
            }
        }

        if (move_has_flag(move, MoveFlag::LOWERS_ATTACKERS_SPECIAL_DEFENSE)) {
            attacker_state.change_stat_stage(
                Stat::SPECIAL_DEFENSE,
                -1,
                false
            );
        }

        if (move_has_flag(move, MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE)) {
            if (move == Move::MetalSound ||
                move == Move::FakeTears
            ) {
                defender_state.change_stat_stage(
                    Stat::SPECIAL_DEFENSE,
                    -2,
                    true
                );
            }
            if (defender_state.is_player &&
                move == Move::SeedFlare
            ) {
                defender_state.change_stat_stage(
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
                defender_state.change_stat_stage(
                    Stat::SPECIAL_DEFENSE,
                    -1,
                    true
                );
            }
        }

        // Speed
        if (move_has_flag(move, MoveFlag::BOOSTS_ATTACKERS_SPEED)) {
            if (move == Move::DragonDance) {
                attacker_state.change_stat_stage(
                    Stat::SPEED,
                    1,
                    false
                );
            }
            if (move == Move::Agility ||
                move == Move::RockPolish
            ) {
                attacker_state.change_stat_stage(
                    Stat::SPEED,
                    2,
                    false
                );
            }
        }

        if (move_has_flag(move, MoveFlag::LOWERS_ATTACKERS_SPEED)) {
            attacker_state.change_stat_stage(
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
                defender_state.change_stat_stage(
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
                defender_state.change_stat_stage(
                    Stat::SPEED,
                    -1,
                    true
                );
            }
            if (defender_state.is_player &&
                (move == Move::CottonSpore ||
                    move == Move::ScaryFace)
            ) {
                defender_state.change_stat_stage(
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
            attacker_state.change_stat_stage(
                Stat::ATTACK,
                1,
                false
            );
            attacker_state.change_stat_stage(
                Stat::DEFENSE,
                1,
                false
            );
            attacker_state.change_stat_stage(
                Stat::SPECIAL_ATTACK,
                1,
                false
            );
            attacker_state.change_stat_stage(
                Stat::SPECIAL_DEFENSE,
                1,
                false
            );
            attacker_state.change_stat_stage(
                Stat::SPEED,
                1,
                false
            );
        }

        if (is_fling) {
            // TODO other items
            if (attacker_state.get_item_for_effect() == Item::KingsRock ||
                attacker_state.get_item_for_effect() == Item::RazorFang
            ) {
                defender_state.set_flinched();
            } else if (attacker_state.get_item_for_effect() ==
                Item::LightBall) {
                defender_state.try_apply_status(
                    Status::PARALYZED,
                    weather,
                    attacker_state
                );
            } else if (attacker_state.get_item_for_effect() ==
                Item::MentalHerb
            ) {
                defender_state.set_infatuated();
            } else if (attacker_state.get_item_for_effect() ==
                Item::WhiteHerb
            ) {
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

    [[nodiscard]] bool is_battle_over() const {
        return player_state.get_health() <= 0 ||
            opponent_state.get_health() <= 0;
    }

public:
    BattleState(
        const CustomPokemon* player_pokemon,
        const CustomPokemon* opponent_pokemon
    ) :
        player_state{player_pokemon, true},
        opponent_state{opponent_pokemon, false} {}


    BattleResultEntry battle() {
        std::vector<
            std::pair<const MoveInfo*, int>
        > player_moves{};
        std::vector<
            std::pair<const MoveInfo*, int>
        > opponent_moves{};
        bool player_goes_first = player_state.outspeeds(
            opponent_state,
            nullptr,
            nullptr,
            get_weather()
        );
        check_abilities(player_goes_first);

        BestMove opponent_move;
        while (player_state.get_health() > 0 &&
            opponent_state.get_health() > 0
        ) {
            BestMove player_move = choose_move_against_defender(
                true,
                false,
                get_weather(),
                is_mid_turn(),
                false
            );

            if (player_state.pokemon->ability == Ability::Trace &&
                player_state.pokemon->name == Pokemon::Gardevoir
                && opponent_state.pokemon->ability == Ability::Blaze &&
                opponent_state.pokemon->name == Pokemon::Torchic
            ) {
                volatile int a;
            }

            opponent_move = choose_move_against_defender(
                false,
                false,
                get_weather(),
                is_mid_turn(),
                false
            );

            check_unimplemented_moves(player_move, opponent_move);

            player_goes_first = player_state.outspeeds(
                opponent_state,
                opponent_move.move,
                player_move.move,
                get_weather()
            );

            if (player_state.pokemon->ability == Ability::SwiftSwim &&
                player_state.pokemon->name == Pokemon::Kingdra
                && opponent_state.pokemon->ability == Ability::Download &&
                opponent_state.pokemon->name == Pokemon::PorygonZ
            ) {
                volatile int a;
            }

            if (player_goes_first) {
                execute_move(
                    true,
                    get_weather(),
                    is_mid_turn()
                );
                add_last_used_move(
                    player_moves,
                    player_state,
                    opponent_state
                );
                apply_post_move_effects(
                    player_state,
                    player_state.get_last_used_move(),
                    opponent_state
                );
                if (is_battle_over()) {
                    break;
                }
                set_mid_turn();
                if (!opponent_state.is_flinched()) {
                    execute_move(
                        false,
                        get_weather(),
                        is_mid_turn()
                    );
                    add_last_used_move(
                        opponent_moves,
                        opponent_state,
                        player_state
                    );
                    apply_post_move_effects(
                        opponent_state,
                        opponent_state.get_last_used_move(),
                        player_state
                    );
                }
                if (is_battle_over()) {
                    break;
                }
            } else {
                execute_move(
                    false,
                    get_weather(),
                    is_mid_turn()
                );
                add_last_used_move(
                    opponent_moves,
                    opponent_state,
                    player_state
                );
                apply_post_move_effects(
                    opponent_state,
                    opponent_state.get_last_used_move(),
                    player_state
                );
                if (is_battle_over()) {
                    break;
                }
                set_mid_turn();
                if (!player_state.is_flinched()) {
                    execute_move(
                        true,
                        get_weather(),
                        is_mid_turn()
                    );
                    add_last_used_move(
                        player_moves,
                        player_state,
                        opponent_state
                    );
                    apply_post_move_effects(
                        player_state,
                        player_state.get_last_used_move(),
                        opponent_state
                    );
                }
                if (is_battle_over()) {
                    break;
                }
            }
            apply_end_of_turn_effects();
            end_turn();

            if (player_move.damage == 0 &&
                opponent_move.damage == 0 &&
                !player_state.is_recharging() &&
                !opponent_state.is_recharging() &&
                player_state.get_field_location() == FieldLocation::ON_FIELD &&
                opponent_state.get_field_location() == FieldLocation::ON_FIELD
                &&
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

        if (opponent_moves.size() == 0 && player_moves.size() != 1 &&
            !(player_goes_first &&
                player_moves.size() == 2 &&
                player_moves[0].first->move == Move::FakeOut) &&
            !(player_moves[0].first->move == Move::FakeOut &&
                opponent_state.get_ability() == Ability::Truant) &&
            (opponent_move.move->move != Move::SolarBeam)
        ) {
            throw std::runtime_error("Opponent could not attack");
        }

        if (player_state.get_health() > 0) {
            return {
                opponent_state.pokemon,
                true,
                std::move(player_moves),
                std::move(opponent_moves)
            };
        }
        return {
            opponent_state.pokemon,
            false,
            std::move(player_moves),
            std::move(opponent_moves)
        };
    }
};


BattleResultEntry battle(
    const CustomPokemon* player,
    const CustomPokemon* opponent
) {
    BattleState battle_state{player, opponent};
    return std::move(battle_state.battle());
}
