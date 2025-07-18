#include "SimulationState.h"

#include <cassert>
#include <cmath>

#include "config.h"
#include "type_effectiveness.h"


void PokemonState::apply_end_of_turn_effects(
    const Weather weather,
    PokemonState& defender_state
) {
    const auto item = get_item_for_effect();
    const auto ability = get_ability();
    const bool has_magic_guard = ability == Ability::MagicGuard;

    // 1.0 Reflect wears off: "your team's reflect wore off"
    // 1.1 Light Screen wears off: "your team's light screen wore off"
    // 1.2 Mist wears off: "your team's mist wore off"
    // 1.3 Safeguard fades: "your team is no longer protected by safeguard"
    // 1.4 Tailwind ends: "your team's tailwind petered out"
    // 1.5 Lucky Chant: your team's lucky chant wore off"
    //
    // 2.0 Wish: "pokemon's wish came true"
    //
    // 3.0 Hail, Rain, Sandstorm, or Sun message
    if (!has_magic_guard &&
        weather == Weather::HAIL &&
        !has_type(PokemonType::ICE) &&
        ability != Ability::IceBody &&
        ability != Ability::SnowCloak
    ) {
        apply_damage(max_health / 16);
    }
    if (!has_magic_guard &&
        weather == Weather::SANDSTORM &&
        !has_type(PokemonType::ROCK) &&
        !has_type(PokemonType::STEEL) &&
        !has_type(PokemonType::GROUND) &&
        ability != Ability::SandVeil
    ) {
        apply_damage(max_health / 16);
    }

    if (ability == Ability::SolarPower &&
        weather == Weather::SUN
    ) {
        apply_damage(max_health / 8);
    }
    //
    // 4.0 Dry Skin, Hydration, Ice Body, Rain Dish
    if (ability == Ability::DrySkin) {
        if (weather == Weather::SUN) {
            apply_damage(max_health / 8);
        } else if (weather == Weather::RAIN) {
            heal(max_health / 8);
        }
    }
    if (ability == Ability::Hydration &&
        weather == Weather::RAIN
    ) {
        clear_status();
    }
    if (ability == Ability::IceBody &&
        weather == Weather::HAIL &&
        ability == pokemon.ability
    ) {
        heal(max_health / 8);
    }
    if (ability == Ability::RainDish &&
        weather == Weather::RAIN
    ) {
        heal(max_health / 16);
    }
    //
    // 5.0 Gravity
    //
    // 6.0 Ingrain
    // 6.1 Aqua Ring
    // 6.2 Speed Boost, Shed Skin
    if (ability == Ability::SpeedBoost) {
        change_stat_modifier(Stat::SPEED, 1, false);
    }
    if (ability == Ability::ShedSkin && !is_player) {
        clear_status();
    }
    // 6.3 Black Sludge, Leftovers: "pokémon restored a little HP using its leftovers"
    if (item == Item::BlackSludge) {
        if (has_type(PokemonType::POISON)) {
            heal(std::max(1, max_health / 16));
        } else if (!has_magic_guard) {
            apply_damage(std::max(1, max_health / 8));
        }
    }
    if (item == Item::Leftovers) {
        heal(std::max(1, max_health / 16));
    }
    // 6.4 Leech Seed: "pokémon's health is sapped by leech seed"
    // 6.5 Burn, Nightmare, Poison Heal, Poison: "pokémon is hurt by poison"
    if (!has_magic_guard && status == Status::BURN) {
        if (get_ability() == Ability::Heatproof) {
            apply_damage(max_health / 16);
        } else {
            apply_damage(max_health / 8);
        }
    }
    if (!has_magic_guard && status == Status::POISON) {
        if (get_ability() == Ability::PoisonHeal) {
            heal(max_health / 8);
        } else {
            apply_damage(max_health / 8);
        }
    }
    if (status == Status::BADLY_POISONED) {
        turns_badly_poisoned = std::min(15, turns_badly_poisoned + 1);
    }
    if (!has_magic_guard && status == Status::BADLY_POISONED) {
        if (get_ability() == Ability::PoisonHeal) {
            heal(max_health / 8);
        } else {
            apply_damage(turns_badly_poisoned * (max_health / 16));
        }
    }
    // 6.6 Flame Orb activation, Toxic Orb activation
    if (item == Item::FlameOrb && !has_type(PokemonType::FIRE)) {
        try_apply_status(Status::BURN, weather, defender_state);
    }
    // 6.7 Curse (from a Ghost)
    // 6.8 Bind, Clamp, Fire Spin, Magma Storm, Sand Tomb, Whirlpool, Wrap
    if (!has_magic_guard && trapped_counter > 0) {
        apply_damage(max_health / 8);
    }
    if (trapped_counter > 0) {
        trapped_counter--;
    }
    // 6.9 Bad Dreams Damage
    // 6.10 End of Outrage, Petal Dance, Thrash, Uproar: "pokémon caused an uproar" & "pokémon calmed down"
    // 6.11 Disable ends: "pokémon is no longer disabled"
    // 6.12 Encore ends
    // 6.13 Taunt wears off
    // 6.14 Magnet Rise
    // 6.15 Heal Block: "the foe pokémon's heal block wore off"
    // 6.16 Embargo
    // 6.17 Yawn
    // 6.18 Sticky Barb
    if (!has_magic_guard && item == Item::StickyBarb) {
        apply_damage(max_health / 8);
    }
    //
    // 7.0 Doom Desire, Future Sight
    //
    // 8.0 Perish Song
    //
    // 9.0 Trick Room
    //
    // 10.0 Pokemon is switched in (if previous Pokemon fainted)
    // 10.1 Toxic Spikes
    // 10.2 Spikes
    // 10.3 Stealth Rock

    was_hit_ = false;
    flinched = false;
    slow_start_count++;
    if (ability == Ability::Truant) {
        truant = !truant;
        recharging = false;
    }
}

uint PokemonState::get_damage_of_attacker_move(
    const MoveInfo* attacker_move_info,
    PokemonState& defender_state,
    const Weather weather,
    const bool is_mid_turn
) {
    const auto category = attacker_move_info->category;
    const bool is_special = category == Category::SPECIAL;
    assert(category != Category::STATUS);

    const auto defender_ability = defender_state.get_ability();
    uint16_t attacker_attack =
        is_special
            ? get_special_attack(weather, defender_ability)
            : get_attack(weather, defender_ability);


    const auto attacker_ability = get_ability();
    const uint16_t defender_defense =
        is_special
            ? defender_state.get_special_defense(weather, defender_ability)
            : defender_state.get_defense(defender_ability);

    const auto attacker_health = get_health();
    const auto attacker_move = attacker_move_info->move;

    // Damp
    if (defender_ability == Ability::Damp &&
        (attacker_move == Move::Selfdestruct ||
            attacker_move == Move::Explosion)
    ) {
        return 0;
    }

    // Soundproof
    if (defender_ability == Ability::Soundproof &&
        move_has_flag(attacker_move, MoveFlag::IS_SOUND_BASED)
    ) {
        return 0;
    }

    // Moves with fixed damage
    if (attacker_move == Move::Endeavor) {
        return std::max(
            0,
            defender_state.get_health() - attacker_health
        );
    }
    const auto defender_chosen_move_info = defender_state.chosen_move.move;
    if (attacker_move == Move::Counter) {
        if (defender_chosen_move_info != nullptr &&
            defender_chosen_move_info->category == Category::PHYSICAL
        ) {
            return defender_state.chosen_move.damage * 2;
        }
        return 0;
    }
    if (attacker_move == Move::MirrorCoat) {
        if (defender_chosen_move_info != nullptr &&
            defender_chosen_move_info->category == Category::SPECIAL
        ) {
            return defender_state.chosen_move.damage * 2;
        }
        return 0;
    }

    // Determine type of move
    auto move_type = attacker_move_info->type;
    if (attacker_move == Move::WeatherBall) {
        switch (weather) {
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
    if (attacker_ability == Ability::Forecast) {
        switch (weather) {
        case Weather::SUN:
            change_type(PokemonType::FIRE);
            break;
        case Weather::RAIN:
            change_type(PokemonType::WATER);
            break;
        case Weather::HAIL:
            change_type(PokemonType::ICE);
            break;
        default:
            move_type = PokemonType::NORMAL;
        }
    } else if (attacker_ability == Ability::Normalize) {
        move_type = PokemonType::NORMAL;
    }

    // Abilities that rely on type
    if ((defender_ability == Ability::FlashFire &&
            move_type == PokemonType::FIRE) ||
        (defender_ability == Ability::MotorDrive &&
            move_type == PokemonType::ELECTRIC) ||
        (defender_ability == Ability::Levitate &&
            move_type == PokemonType::GROUND) ||
        (defender_ability == Ability::VoltAbsorb &&
            move_type == PokemonType::ELECTRIC) ||
        (defender_ability == Ability::WaterAbsorb &&
            move_type == PokemonType::WATER)
    ) {
        return 0;
    }
    if (defender_ability == Ability::ThickFat &&
        (move_type == PokemonType::ICE ||
            move_type == PokemonType::FIRE)
    ) {
        attacker_attack = attacker_attack / 2;
    }

    static std::array<double, LEVEL + 1> cache = [] {
        std::array<double, LEVEL + 1> arr{};
        arr.fill(-1.0);
        return arr;
    }();
    double damage = cache[level];
    if (damage < 0) {
        damage = std::floor(2.0 * level * 0.2) + 2.0;
        cache[level] = damage;
    }

    // Power calculations

    // Moves with dynamic power
    double power = attacker_move_info->power;
    const auto status = get_status();
    const auto item = get_item_for_effect();
    if (attacker_move == Move::Eruption ||
        attacker_move == Move::WaterSpout
    ) {
        power = std::max(
            1.0,
            std::floor(150.0 * attacker_health / max_health)
        );
    } else if ((attacker_move == Move::Facade &&
            (status == Status::POISON ||
                status == Status::PARALYZED ||
                status == Status::BURN)) ||
        (was_hit() &&
            (attacker_move == Move::Avalanche ||
                attacker_move == Move::Revenge ||
                attacker_move == Move::Payback)) ||
        (attacker_move == Move::WeatherBall &&
            weather != Weather::CLEAR
        ) ||
        (attacker_move == Move::Brine &&
            attacker_health <= max_health / 2)
    ) {
        power = power * 2;
    } else if (attacker_move == Move::HiddenPower) {
        if (is_player) {
            power = 30;
        } else {
            power = 70;
        }
    } else if (attacker_move == Move::LowKick ||
        attacker_move == Move::GrassKnot
    ) {
        if (const double defender_weight = defender_state.pounds;
            defender_weight < 21.9
        ) {
            power = 20;
        } else if (defender_weight < 55.1) {
            power = 40;
        } else if (defender_weight < 110.2) {
            power = 60;
        } else if (defender_weight < 220.4) {
            power = 80;
        } else if (defender_weight < 440.9) {
            power = 100;
        } else {
            power = 120;
        }
    } else if (attacker_move == Move::Fling) {
        if (item == Item::None) {
            power = 0;
        } else if (item == Item::ChoiceBand) {
            power = 10;
        } else {
            throw std::runtime_error{
                "No fling: " + ITEM_TO_STRING.at(item)
            };
        }
    } else if (attacker_move == Move::WringOut) {
        power =
            1 + 120 * defender_state.get_health() / defender_state.max_health;
    }

    // Technician
    if (ability == Ability::Technician && power <= 50) {
        power = std::floor(power * 1.5);
    }

    // Reckless
    if (attacker_ability == Ability::Reckless &&
        move_has_flag(attacker_move, MoveFlag::HAS_RECOIL)
    ) {
        if (attacker_move == Move::HeadSmash) {
            power = 180;
        } else if (attacker_move == Move::HighJumpKick) {
            power = 156;
        } else if (attacker_move == Move::BraveBird ||
            attacker_move == Move::DoubleEdge ||
            attacker_move == Move::FlareBlitz
        ) {
            power = 144;
        } else if (attacker_move == Move::JumpKick) {
            power = 120;
        } else if (attacker_move == Move::TakeDown) {
            power = 108;
        } else if (attacker_move == Move::Submission) {
            power = 96;
        }
    }

    // Items that increase power
    if (item == Item::Metronome) {
        const double multiplier = 1.0 + std::max(
            0.0,
            0.1 * get_metronome_count()
        );
        power = std::floor(power * multiplier);
    } else if ((item == Item::MuscleBand &&
            attacker_move_info->category == Category::PHYSICAL) ||
        (item == Item::WiseGlasses &&
            attacker_move_info->category == Category::SPECIAL)
    ) {
        power = std::floor(power * 1.1);
    } else if ((POWER_ITEMS[static_cast<int>(item)] &&
            ITEM_TO_TYPE.at(item) == move_type) ||
        (PLATE_ITEMS[static_cast<int>(item)] &&
            PLATE_ITEM_TYPES.at(item) == move_type)
    ) {
        power = std::floor(power * 1.2);
    } else if (get_item() == Item::LightBall &&
        pokemon.name == Pokemon::Pikachu
    ) {
        power = power * 2;
    }

    // Abilities that affect power
    if ((attacker_ability == Ability::Blaze &&
            attacker_health <= max_health / 3 &&
            move_type == PokemonType::FIRE) ||
        (attacker_ability == Ability::Overgrow &&
            attacker_health <= max_health / 3 &&
            move_type == PokemonType::GRASS) ||
        (attacker_ability == Ability::Swarm &&
            attacker_health <= max_health / 3 &&
            move_type == PokemonType::BUG) ||
        (attacker_ability == Ability::Torrent &&
            attacker_health <= max_health / 3 &&
            move_type == PokemonType::WATER)
    ) {
        power = std::floor(power * 1.5);
    }
    if (attacker_ability == Ability::Rivalry) {
        if (is_player) {
            power = std::floor(power * 0.75);
        } else {
            power = std::floor(power * 1.25);
        }
    }
    if (attacker_ability == Ability::IronFist &&
        move_has_flag(attacker_move, MoveFlag::POWERS_IRON_FIST)
    ) {
        power = std::floor(power * 1.2);
    }
    if (defender_ability == Ability::Heatproof &&
        move_type == PokemonType::FIRE
    ) {
        power = power / 2;
    }

    // Field location
    const bool attacker_faster = outspeeds(
        defender_state,
        nullptr,
        attacker_move_info,
        weather
    );
    if (const auto& defender_field_location =
            defender_state.get_field_location();
        defender_field_location != FieldLocation::ON_FIELD &&
        attacker_faster
    ) {
        if (!attacker_faster && !is_mid_turn) {
            power = attacker_move_info->power;
        } else {
            if (defender_field_location == FieldLocation::IN_AIR) {
                if (move_has_flag(
                        attacker_move,
                        MoveFlag::HITS_DEFENDER_IN_AIR
                    )
                ) {
                    if (attacker_move == Move::Gust ||
                        attacker_move == Move::Twister
                    ) {
                        power *= 2;
                    }
                } else if (attacker_ability != Ability::NoGuard) {
                    return 0;
                }
            } else if (defender_field_location ==
                FieldLocation::IN_THE_VOID &&
                attacker_ability != Ability::NoGuard
            ) {
                return 0;
            } else if (defender_field_location ==
                FieldLocation::UNDER_GROUND
            ) {
                if (move_has_flag(
                        attacker_move,
                        MoveFlag::HITS_DEFENDER_UNDER_GROUND
                    )
                ) {
                    if (attacker_move == Move::Earthquake ||
                        attacker_move == Move::Magnitude
                    ) {
                        power *= 2;
                    }
                } else if (attacker_ability != Ability::NoGuard) {
                    return 0;
                }
            } else if (defender_field_location ==
                FieldLocation::UNDER_WATER
            ) {
                if (move_has_flag(
                        attacker_move,
                        MoveFlag::HITS_DEFENDER_UNDER_WATER)
                ) {
                    power *= 2;
                } else if (attacker_ability != Ability::NoGuard) {
                    return 0;
                }
            }
        }
    }

    // Abilities that change attack stat
    if (attacker_move_info->category == Category::PHYSICAL) {
        if ((attacker_ability == Ability::Guts &&
                status != Status::NONE) ||
            (attacker_ability == Ability::Hustle &&
                !is_player) ||
            (attacker_ability == Ability::FlashFire &&
                was_flash_fired() &&
                move_type == PokemonType::FIRE)
        ) {
            attacker_attack = std::floor(attacker_attack * 1.5);
        } else if (attacker_ability == Ability::HugePower) {
            attacker_attack = attacker_attack * 2;
        }
    }

    damage =
        std::floor(damage * power * attacker_attack / defender_defense);
    damage = std::floor(damage * 0.02) + 2.0;

    // STAB
    if (has_type(move_type)) {
        if (attacker_ability == Ability::Adaptability) {
            damage = damage * 2;
        } else {
            damage = std::floor(damage * 1.5);
        }
    }

    // Random
    if (is_player) {
        damage = std::floor(damage * 0.85);
    }

    // Type effectiveness
    auto defender_types = defender_state.types;
    if (defender_state.get_item() == Item::IronBall &&
        attacker_move_info->type == PokemonType::GROUND
    ) {
        if (defender_types[0] == PokemonType::FLYING) {
            defender_types = {defender_types[1]};
        } else if (defender_types[1] == PokemonType::FLYING) {
            defender_types = {defender_types[0]};
        }
    }
    auto effectiveness =
        get_effectiveness(defender_types, move_type);
    if (attacker_move == Move::HiddenPower) {
        if (is_player) {
            if (defender_state.has_type(PokemonType::COUNT)) {
                effectiveness = 0.5;
            } else {
                effectiveness = 0.25;
            }
        } else {
            if (defender_state.has_type(PokemonType::COUNT)) {
                effectiveness = 4;
            } else {
                effectiveness = 2;
            }
        }
    }
    if (attacker_ability == Ability::Scrappy &&
        defender_state.has_type(PokemonType::GHOST) &&
        (move_type == PokemonType::NORMAL ||
            move_type == PokemonType::FIGHTING)
    ) {
        effectiveness = 1.0;
    } else if (defender_ability == Ability::SolidRock &&
        effectiveness >= 2.0
    ) {
        effectiveness = std::floor(effectiveness * 0.75);
    } else if (attacker_ability == Ability::WonderGuard &&
        effectiveness < 2.0
    ) {
        return 0;
    }
    damage = static_cast<int>(damage * effectiveness);
    if (item == Item::ExpertBelt && effectiveness >= 2) {
        damage = std::floor(damage * 1.2);
    }

    // Damage reducers
    if ((attacker_move != Move::BrickBreak &&
            (defender_state.has_reflect_up() &&
                attacker_move_info->category == Category::PHYSICAL) ||
            (defender_state.has_light_screen_up() &&
                attacker_move_info->category == Category::SPECIAL)) ||
        (status == Status::BURN &&
            attacker_move_info->category == Category::PHYSICAL &&
            !move_has_flag(attacker_move, MoveFlag::HAS_FIXED_DAMAGE) &&
            attacker_ability != Ability::Guts)
    ) {
        damage = std::floor(damage / 2.0);
    }

    // Abilities affecting damage
    if (defender_ability == Ability::DrySkin) {
        if (move_type == PokemonType::FIRE) {
            damage = std::floor(damage * 1.25);
        } else if (move_type == PokemonType::WATER) {
            damage = 0;
        }
    } else if (defender_ability == Ability::Filter &&
        effectiveness >= 2.0
    ) {
        damage = std::floor(damage * 0.25);
    } else if (attacker_ability == Ability::TintedLens &&
        effectiveness <= 0.5
    ) {
        damage = damage * 2;
    }

    // Weather
    switch (weather) {
    case Weather::SUN:
        if (move_type == PokemonType::FIRE) {
            damage = std::floor(damage * 1.5);
        } else if (move_type == PokemonType::WATER) {
            damage = std::floor(damage * 0.5);
        }
        break;
    case Weather::RAIN:
        if (move_type == PokemonType::FIRE) {
            damage = std::floor(damage * 0.5);
        } else if (move_type == PokemonType::WATER) {
            damage = std::floor(damage * 1.5);
        }
        break;
    default:
        break;
    }

    return static_cast<int>(damage);
}