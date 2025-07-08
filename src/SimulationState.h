#ifndef POKEMONSTATE_H
#define POKEMONSTATE_H

#include <utility>

#include "custom_pokemon.h"
#include "nature.h"
#include "type_effectiveness.h"

enum class Weather {
    CLEAR,
    SUN,
    RAIN,
    HAIL,
    SANDSTORM,
    FOG
};

enum class Status {
    BURN,
    POISON,
    BADLY_POISONED,
    PARALYZED,
    SLEEP,
    FROZEN,
    NONE
};

enum class FieldLocation {
    IN_AIR,
    UNDER_GROUND,
    UNDER_WATER,
    IN_THE_VOID,
    ON_FIELD
};

struct BestMove {
    const MoveInfo* move = nullptr;
    int damage = 0;
};

inline int calculate_stat_based_on_stage(
    const uint16_t stat,
    const int stage
) {
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

class PokemonState {
    static constexpr int HEALTH_INDEX =
        static_cast<int>(Stat::HEALTH);
    static constexpr int ATTACK_INDEX =
        static_cast<int>(Stat::ATTACK);
    static constexpr int DEFENSE_INDEX =
        static_cast<int>(Stat::DEFENSE);
    static constexpr int SPECIAL_ATTACK_INDEX =
        static_cast<int>(Stat::SPECIAL_ATTACK);
    static constexpr int SPECIAL_DEFENSE_INDEX =
        static_cast<int>(Stat::SPECIAL_DEFENSE);
    static constexpr int SPEED_INDEX =
        static_cast<int>(Stat::SPEED);
    static constexpr int8_t MAX_STAT_STAGE = 6;
    static constexpr int8_t MIN_STAT_STAGE = -6;

    std::array<PokemonType, 2> types;
    Ability ability;
    Item item;
    std::array<int32_t, 6> current_stats;
    std::array<int8_t, 6> stat_stages = {0, 0, 0, 0, 0, 0};
    FieldLocation field_location = FieldLocation::ON_FIELD;

    Status status = Status::NONE;
    int turns_badly_poisoned = 0;
    int trapped_counter = 0;
    bool confused = false;
    bool flinched = false;
    bool infatuated = false;
    bool was_hit_ = false;
    bool flash_fired = false;

    bool charging = false;
    bool recharging = false;
    int multi_turn_move_counter = 0;

    bool grounded;
    int metronome = 0;
    bool is_choiced = false;

    bool protected_ = false;
    bool reflect = false;
    bool light_screen = false;

    BestMove last_used_move{};
    BestMove chosen_move{};

public:
    const bool is_player;
    const CustomPokemon pokemon;
    const uint8_t level;
    const int max_health;
    const double pounds;

    PokemonState(CustomPokemon pokemon, const bool is_player):
        types(pokemon.types),
        ability(pokemon.ability),
        item(pokemon.item),
        current_stats(
            {
                pokemon.stats[HEALTH_INDEX],
                pokemon.stats[ATTACK_INDEX],
                pokemon.stats[DEFENSE_INDEX],
                pokemon.stats[SPECIAL_ATTACK_INDEX],
                pokemon.stats[SPECIAL_DEFENSE_INDEX],
                pokemon.stats[SPEED_INDEX]
            }
        ),
        grounded(item == Item::IronBall),
        is_player(is_player),
        pokemon(std::move(pokemon)),
        level(pokemon.level),
        max_health(pokemon.stats[HEALTH_INDEX]),
        pounds(pokemon.pounds) {
        if (ability == Ability::Multitype &&
            PLATE_ITEMS[static_cast<int>(item)]
            // TODO multitype
        ) {
            types = {PLATE_ITEM_TYPES.at(item), PokemonType::COUNT};
        }
    }

    [[nodiscard]] bool has_type(const PokemonType type) const {
        return types[0] == type || types[1] == type;
    }

    void change_type(const PokemonType type) {
        types[0] = type;
        types[1] = PokemonType::COUNT;
    }

    [[nodiscard]] Ability get_ability() const {
        return ability;
    }

    void clear_ability() {
        ability = Ability::Disabled;
    }

    [[nodiscard]] Item get_item_for_effect() const {
        if (get_ability() != Ability::Klutz) {
            return item;
        }
        return Item::None;
    }

    [[nodiscard]] Item get_item() const {
        return item;
    }

    bool try_set_item(const Item item) {
        if (this->item == Item::None) {
            this->item = item;
            grounded = this->item == Item::IronBall;
            return true;
        }
        return false;
    }

    void clear_item() {
        item = Item::None;
        grounded = false;
        // TODO mutlitype
    }

    void apply_berry() {
        bool eaten = false;
        if (const auto berry = get_item_for_effect();
            berry == Item::ApicotBerry
        ) {
            change_stat_modifier(
                Stat::SPECIAL_DEFENSE,
                1,
                false
            );
            eaten = true;
        } else if (berry == Item::ChestoBerry) {
            if (status == Status::SLEEP) {
                status = Status::NONE;
            }
            eaten = true;
        } else if (berry == Item::GanlonBerry) {
            change_stat_modifier(
                Stat::DEFENSE,
                1,
                false
            );
            eaten = true;
        } else if (berry == Item::LiechiBerry) {
            change_stat_modifier(
                Stat::ATTACK,
                1,
                false
            );
            eaten = true;
        } else if (berry == Item::LumBerry) {
            if (status != Status::NONE) {
                status = Status::NONE;
            }
            if (confused) {
                confused = false;
            }
            eaten = true;
        } else if (berry == Item::PersimBerry) {
            if (confused) {
                confused = false;
            }
            eaten = true;
        } else if (berry == Item::PetayaBerry) {
            change_stat_modifier(Stat::SPECIAL_ATTACK, 1, false);
            eaten = true;
        } else if (berry == Item::SalacBerry) {
            change_stat_modifier(Stat::SPEED, 1, false);
            eaten = true;
        } else if (berry == Item::SitrusBerry) {
            const auto current_health =
                current_stats[HEALTH_INDEX];
            current_stats[HEALTH_INDEX] =
                std::min(max_health, current_health + max_health / 4);
            eaten = true;
        }
        if (!eaten) {
            throw std::runtime_error("Unimplemented berry");
        }
        item = Item::None;
    }

    void try_apply_berry(const bool eaten) {
        if (BERRIES.contains(get_item_for_effect()) ||
            (BERRIES.contains(this->item) && eaten)
        ) {
            const int current_health = get_health();
            const bool less_than_half_health = current_health <= max_health
                / 2;
            const bool eats_stat_berry =
                STAT_BERRIES.contains(get_item_for_effect()) &&
                ((current_health <= max_health / 4) ||
                    (get_ability() == Ability::Gluttony &&
                        less_than_half_health)
                );
            const bool eats_status_berry =
                (item == Item::ChestoBerry && status == Status::SLEEP) ||
                (item == Item::PersimBerry && confused) ||
                (item == Item::LumBerry &&
                    (status != Status::NONE || confused)
                );
            const bool eats_sitrus = less_than_half_health;
            const bool eats_berry =
                eaten ||
                eats_stat_berry ||
                eats_sitrus ||
                eats_status_berry;
            if (eats_berry) {
                apply_berry();
            }
        }
    }

    void eat_berry(const Item item) {
        const Item backup = this->item;
        this->item = item;
        try_apply_berry(true);
        this->item = backup;
    }

    [[nodiscard]] int32_t get_health() const {
        return current_stats[HEALTH_INDEX];
    }

    void apply_damage(const int damage) {
        current_stats[HEALTH_INDEX] -= damage;
        try_apply_berry(false);
    }

    void heal(const int health_gained) {
        current_stats[HEALTH_INDEX] = std::min(
            max_health,
            current_stats[HEALTH_INDEX] + health_gained
        );
    }

    [[nodiscard]] int32_t get_attack(const Weather weather) const {
        uint16_t stat = current_stats[ATTACK_INDEX];
        if (get_ability() == Ability::FlowerGift &&
            weather == Weather::SUN
        ) {
            stat = std::floor(stat * 1.5);
        }
        return stat;
    }

    [[nodiscard]] int32_t get_defense() const {
        uint16_t stat = current_stats[DEFENSE_INDEX];
        if (get_ability() == Ability::MarvelScale &&
            status != Status::NONE
        ) {
            stat = std::floor(stat * 1.5);
        }
        return stat;
    }

    [[nodiscard]] int32_t get_special_attack() const {
        return current_stats[SPECIAL_ATTACK_INDEX];
    }

    [[nodiscard]] int32_t get_special_defense(const Weather weather) const {
        uint16_t stat = current_stats[SPECIAL_DEFENSE_INDEX];
        if (get_ability() == Ability::FlowerGift &&
            weather == Weather::SUN
        ) {
            stat = stat * 1.5;
        }
        return stat;
    }

    [[nodiscard]] int32_t get_speed(const Weather weather) const {
        uint16_t speed = current_stats[SPEED_INDEX];
        if (const auto item = get_item_for_effect();
            item == Item::ChoiceScarf
        ) {
            speed = std::floor(speed * 1.5);
        } else if (item == Item::IronBall) {
            speed = speed / 2;
        }

        if (get_ability() == Ability::Chlorophyll &&
            weather == Weather::SUN
        ) {
            speed = speed * 2;
        }

        if (status == Status::PARALYZED) {
            speed = std::floor(speed * 0.25);
        }
        return speed;
    }

    void change_stat_modifier(
        const Stat stat,
        const int8_t change,
        const bool from_other
    ) {
        assert(change != 0 && change >= -6 && change <= 6);
        const int index = static_cast<int>(stat);
        const auto stage = stat_stages[index];
        const auto new_stage = static_cast<int8_t>(stage + change);
        if (change > 0) {
            stat_stages[index] = std::min(new_stage, MAX_STAT_STAGE);
        } else {
            if (const auto ability = get_ability();
                !(ability == Ability::ClearBody && from_other) &&
                !(ability == Ability::HyperCutter && from_other &&
                    stat == Stat::ATTACK)
            ) {
                if (get_item_for_effect() == Item::WhiteHerb) {
                    item = Item::None;
                } else {
                    stat_stages[index] =
                        std::max(new_stage, MIN_STAT_STAGE);
                }
            }
        }
        current_stats[index] =
            calculate_stat_based_on_stage(
                pokemon.stats[index],
                stat_stages[index]
            );
    }

    void clear_negative_stat_changes() {
        int i = 0;
        for (const auto stat_stage : stat_stages) {
            if (stat_stage < 0) {
                stat_stages[i] = 0;
                current_stats[i] =
                    calculate_stat_based_on_stage(
                        pokemon.stats[i],
                        stat_stages[i]
                    );
            }
            i++;
        }
    }

    [[nodiscard]] FieldLocation get_field_location() const {
        return field_location;
    }

    [[nodiscard]] Status get_status() const {
        return status;
    }

    void try_apply_status(const Status status, const Weather weather) {
        if (const auto ability = get_ability();
            status == Status::NONE &&
            !(ability == Ability::Immunity &&
                (status == Status::POISON ||
                    status == Status::BADLY_POISONED)) &&
            !(ability == Ability::Insomnia && status == Status::SLEEP) &&
            !(ability == Ability::LeafGuard && weather == Weather::SUN) &&
            !(ability == Ability::Limber && status == Status::PARALYZED) &&
            !(ability == Ability::MagmaArmor && status == Status::FROZEN)
        ) {
            this->status = status;
            try_apply_berry(false);
        }
    }

    void clear_status() {
        status = Status::NONE;
    }

    [[nodiscard]] bool is_confused() const {
        return confused;
    }

    [[nodiscard]] bool is_flinched() const {
        return flinched;
    }

    void set_flinched() {
        flinched = true;
    }

    void set_infatuated() {
        infatuated = true;
    }

    void set_was_hit() {
        was_hit_ = true;
    }

    [[nodiscard]] bool was_hit() const {
        return was_hit_;
    }

    [[nodiscard]] bool was_flash_fired() const {
        return this->flash_fired;
    }

    void set_flash_fire() {
        this->flash_fired = true;
    }

    [[nodiscard]] bool is_charging() const {
        return charging;
    }

    void start_charging() {
        charging = true;
    }

    void done_charging() {
        charging = false;
    }

    void used_move_that_requires_recharge() {
        recharging = true;
    }

    [[nodiscard]] bool is_recharging() const {
        return recharging;
    }

    void done_recharging() {
        recharging = false;
    }

    void increment_multi_turn_move_counter(const uint8_t max_turns) {
        if (multi_turn_move_counter >= max_turns) {
            multi_turn_move_counter = 0;
        } else {
            multi_turn_move_counter++;
        }
    }

    [[nodiscard]] int get_metronome_count() const {
        return metronome;
    }

    [[nodiscard]] bool is_protected() const {
        return protected_;
    }

    void break_protect() {
        protected_ = false;
    }

    [[nodiscard]] bool has_reflect_up() const {
        return reflect;
    }

    void break_reflect() {
        reflect = false;
    }

    [[nodiscard]] bool has_light_screen_up() const {
        return light_screen;
    }

    void break_light_screen() {
        light_screen = false;
    }

    void apply_end_of_turn_effects(
        const Weather weather,
        const PokemonState& defender_state
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
        //
        // 5.0 Gravity
        //
        // 6.0 Ingrain
        // 6.1 Aqua Ring
        // 6.2 Speed Boost, Shed Skin
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
            apply_damage(max_health / 8);
        }
        if (status == Status::BADLY_POISONED) {
            turns_badly_poisoned = std::min(15, turns_badly_poisoned + 1);
        }
        if (!has_magic_guard && status == Status::BADLY_POISONED) {
            apply_damage(turns_badly_poisoned * (max_health / 16));
        }
        // 6.6 Flame Orb activation, Toxic Orb activation
        if (item == Item::FlameOrb && !has_type(PokemonType::FIRE)) {
            try_apply_status(Status::BURN, weather);
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
    }

    [[nodiscard]] bool outspeeds(
        const PokemonState& other_state,
        const MoveInfo* other_move,
        const MoveInfo* this_move,
        const Weather weather
    ) const {
        if (is_player && other_state.get_item() == Item::QuickClaw) {
            return false;
        }
        const int this_priority = get_move_priority(this_move);
        const int other_priority = get_move_priority(other_move);
        if (this_priority != other_priority) {
            return this_priority > other_priority;
        }
        return get_speed(weather) > other_state.get_speed(weather);
    }

    int get_damage_of_attacker_move(
        uint16_t attacker_attack,
        const MoveInfo* attacker_move_info,
        const PokemonState& defender_state,
        const uint16_t defender_defense,
        const Weather weather,
        const bool is_mid_turn
    ) {
        if (defender_state.get_ability() == Ability::Damp &&
            (attacker_move_info->move == Move::Selfdestruct ||
                attacker_move_info->move == Move::Explosion)
        ) {
            return 0;
        }

        auto move_type = attacker_move_info->type;
        if (defender_state.get_ability() == Ability::FlashFire &&
            move_type == PokemonType::FIRE
        ) {
            return 0;
        }

        if (defender_state.get_ability() == Ability::MotorDrive &&
            move_type == PokemonType::ELECTRIC
        ) {
            return 0;
        }

        if (defender_state.get_ability() == Ability::Levitate &&
            move_type == PokemonType::GROUND
        ) {
            return 0;
        }

        if (attacker_move_info->category == Category::PHYSICAL) {
            if (get_ability() == Ability::FlashFire &&
                was_flash_fired() &&
                move_type == PokemonType::FIRE
            ) {
                attacker_attack = std::floor(attacker_attack * 1.5);
            }
            if (get_ability() == Ability::Guts &&
                status != Status::NONE
            ) {
                attacker_attack = std::floor(attacker_attack * 1.5);
            }

            if (get_ability() == Ability::HugePower) {
                attacker_attack = attacker_attack * 2;
            }

            if (get_ability() == Ability::Hustle &&
                !is_player
            ) {
                attacker_attack = std::floor(attacker_attack * 1.5);
            }
        }

        if (attacker_move_info->move == Move::Endeavor) {
            return std::max(0, defender_state.get_health() - get_health());
        }

        if (attacker_move_info->move == Move::Counter) {
            if (defender_state.chosen_move.move != nullptr &&
                defender_state.chosen_move.move->category == Category::PHYSICAL
            ) {
                return defender_state.chosen_move.damage * 2;
            }
            return 0;
        }
        if (attacker_move_info->move == Move::MirrorCoat) {
            if (defender_state.chosen_move.move != nullptr &&
                defender_state.chosen_move.move->category == Category::SPECIAL
            ) {
                return defender_state.chosen_move.damage * 2;
            }
            return 0;
        }

        static std::array<double, LEVEL + 1> cache = [] {
            std::array<double, LEVEL + 1> arr{};
            arr.fill(-1.0);
            return arr;
        }();
        double damage = cache[level];
        if (damage < 0) {
            damage = std::floor(2.0 * level / 5.0) + 2.0;
            cache[level] = damage;
        }

        // Power calculations
        // Moves that increase power
        const auto attacker_move = attacker_move_info->move;
        double power = attacker_move_info->power;
        const auto status = get_status();
        if (attacker_move == Move::Eruption ||
            attacker_move == Move::WaterSpout
        ) {
            power = std::max(
                1.0,
                std::floor(150.0 * get_health() / max_health)
            );
        } else if ((attacker_move == Move::Facade &&
                (status == Status::POISON ||
                    status == Status::PARALYZED ||
                    status == Status::BURN)) ||
            (was_hit() &&
                (attacker_move_info->move == Move::Avalanche ||
                    attacker_move_info->move == Move::Revenge)) ||
            (attacker_move_info->move == Move::WeatherBall &&
                weather != Weather::CLEAR
            ) ||
            (attacker_move_info->move == Move::Brine &&
                get_health() <= max_health / 2)
        ) {
            power = power * 2;
        } else if (attacker_move_info->move == Move::HiddenPower) {
            if (is_player) {
                power = 30;
            } else {
                power = 70;
            }
        } else if (attacker_move_info->move == Move::LowKick ||
            attacker_move_info->move == Move::GrassKnot
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
        }

        // Items that increase power
        if (get_item_for_effect() == Item::Metronome) {
            const double multiplier = 1.0 + std::max(
                0.0,
                0.1 * get_metronome_count()
            );
            power = std::floor(power * multiplier);
        } else if ((get_item_for_effect() == Item::WiseGlasses &&
                attacker_move_info->category == Category::SPECIAL) ||
            (get_item_for_effect() == Item::MuscleBand &&
                attacker_move_info->category == Category::PHYSICAL)
        ) {
            power = std::floor(power * 1.1);
        } else if (get_item() == Item::LightBall &&
            pokemon.name == Pokemon::Pikachu
        ) {
            power = power * 2;
        } else if (POWER_ITEMS[static_cast<int>(get_item_for_effect())] &&
            ITEM_TO_TYPE.at(get_item_for_effect()) == move_type
        ) {
            power = std::floor(power * 1.2);
        } else if (PLATE_ITEMS[static_cast<int>(get_item_for_effect())] &&
            PLATE_ITEM_TYPES.at(get_item_for_effect()) == move_type
        ) {
            power = std::floor(power * 1.2);
        }

        // Abilities that affect power
        if (get_ability() == Ability::Blaze &&
            get_health() < max_health / 3 &&
            move_type == PokemonType::FIRE
        ) {
            power = std::floor(power * 1.5);
        }
        if (defender_state.get_ability() == Ability::Heatproof &&
            move_type == PokemonType::FIRE
        ) {
            power = power / 2;
        }
        if (get_ability() == Ability::IronFist &&
            move_has_flag(attacker_move_info->move, MoveFlag::POWERS_IRON_FIST)
        ) {
            power = std::floor(power * 1.2);
        }

        // Field location
        const bool attacker_faster = outspeeds(
            defender_state,
            nullptr,
            attacker_move_info,
            weather
        );
        if (const auto defender_field_location =
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
                    } else {
                        return 0;
                    }
                } else if (defender_field_location ==
                    FieldLocation::IN_THE_VOID
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
                    } else {
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
                    } else {
                        return 0;
                    }
                }
            }
        }

        damage =
            std::floor(damage * power * attacker_attack / defender_defense);
        damage = std::floor(damage / 50.0) + 2.0;

        // STAB
        if (attacker_move_info->move == Move::WeatherBall) {
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
        if (get_ability() == Ability::Forecast) {
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
        }
        if (has_type(move_type)) {
            if (get_ability() == Ability::Adaptability) {
                damage = damage * 2;
            } else {
                damage = std::floor(damage * 1.5);
            }
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
        if (attacker_move_info->move == Move::HiddenPower) {
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
        damage = static_cast<int>(damage * effectiveness);
        if (get_item_for_effect() == Item::ExpertBelt && effectiveness >= 2) {
            damage = std::floor(damage * 1.2);
        }

        // Random
        if (is_player) {
            damage = std::floor(damage * 0.85);
        }

        // Burn
        if (status == Status::BURN &&
            attacker_move_info->category == Category::PHYSICAL &&
            !move_has_flag(attacker_move, MoveFlag::HAS_FIXED_DAMAGE) &&
            get_ability() != Ability::Guts
        ) {
            damage = std::floor(damage / 2.0);
        }

        if (attacker_move_info->move != Move::BrickBreak) {
            if ((defender_state.has_reflect_up() &&
                    attacker_move_info->category == Category::PHYSICAL) ||
                (defender_state.has_light_screen_up() &&
                    attacker_move_info->category == Category::SPECIAL)
            ) {
                damage = std::floor(damage / 2.0);
            }
        }

        // Abilities
        if (defender_state.get_ability() == Ability::DrySkin) {
            if (move_type == PokemonType::FIRE) {
                damage = std::floor(damage * 1.25);
            } else {
                damage = 0;
            }
        } else if (defender_state.get_ability() == Ability::Filter &&
            effectiveness >= 2.0
        ) {
            damage = std::floor(damage / 4);
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

    int get_weather_ball_damage(
        const PokemonState& defender_state,
        const int32_t attack,
        const int32_t special_attack,
        const int32_t defense,
        const int32_t special_defense,
        const Weather weather,
        const bool is_mid_turn
    ) {
        for (const auto castform_move : pokemon.moves) {
            if (castform_move->move == Move::WeatherBall) {
                const auto category =
                    castform_move->category;
                const bool is_special =
                    category == Category::SPECIAL;
                const auto attack_used =
                    is_special
                        ? special_attack
                        : attack;
                const auto defense_used =
                    is_special
                        ? special_defense
                        : defense;
                MoveInfo new_move = *castform_move;
                switch (weather) {
                case Weather::CLEAR:
                    new_move.type = PokemonType::NORMAL;
                    break;
                case Weather::SUN:
                    new_move.type = PokemonType::FIRE;
                    break;
                case Weather::RAIN:
                    new_move.type = PokemonType::WATER;
                    break;
                case Weather::HAIL:
                    new_move.type = PokemonType::ICE;
                    break;
                case Weather::SANDSTORM:
                    new_move.type = PokemonType::ROCK;
                    break;
                case Weather::FOG:
                    new_move.type = PokemonType::NORMAL;
                    break;
                }
                if (weather != Weather::CLEAR) {
                    new_move.power = new_move.power * 2;
                }
                const auto damage =
                    get_damage_of_attacker_move(
                        attack_used,
                        &new_move,
                        defender_state,
                        defense_used,
                        weather,
                        is_mid_turn
                    );
                return damage;
            }
        }
        return 0;
    }

    BestMove get_best_move_against_defender(
        PokemonState& defender_state,
        const bool chosen_move_only,
        const Weather weather,
        bool is_mid_turn
    ) {
        if (is_charging()) {
            return chosen_move;
        }

        BestMove best_move{};
        bool best_move_must_charge = false;
        auto attack = get_attack(weather);
        auto special_attack = get_special_attack();
        if (get_item_for_effect() == Item::ChoiceBand) {
            attack = std::floor(attack * 1.5);
        } else if (get_item_for_effect() == Item::ChoiceSpecs) {
            special_attack = std::floor(special_attack * 1.5);
        }
        const auto defense = defender_state.get_defense();
        auto special_defense =
            defender_state.get_special_defense(weather);

        if (weather == Weather::SANDSTORM &&
            defender_state.has_type(PokemonType::ROCK)
        ) {
            special_defense = std::floor(special_defense * 1.5);
        }
        const bool attacker_faster = outspeeds(
            defender_state,
            nullptr,
            nullptr,
            weather
        );
        if ((is_mid_turn || chosen_move_only) &&
            chosen_move.move != nullptr
        ) {
            // When call mid turn, the move damage has to be recalculated
            const auto category = chosen_move.move->category;
            const bool is_special = category == Category::SPECIAL;
            const auto attack_used =
                is_special ? special_attack : attack;
            const auto defense_used =
                is_special ? special_defense : defense;
            chosen_move = {
                chosen_move.move,
                get_damage_of_attacker_move(
                    attack_used,
                    chosen_move.move,
                    defender_state,
                    defense_used,
                    weather,
                    is_mid_turn
                )
            };
            return chosen_move;
        }

        if (is_recharging()) {
            // No move can be used during recharge
            done_recharging();
            return best_move;
        }

        if (get_field_location() != FieldLocation::ON_FIELD ||
            multi_turn_move_counter > 0
        ) {
            return last_used_move;
        }

        BestMove best_fire_move{};
        BestMove best_water_move{};
        // TODO add priority effects
        //      should not be using lower priority moves
        //      if it means getting KO'd
        for (const auto& move : pokemon.moves) {
            // Skip other moves if locked into a move
            if (is_choiced &&
                move->move != last_used_move.move->move
            ) {
                continue;
            }
            // Skip status moves since they do not do damage
            const auto category = move->category;
            if (category == Category::STATUS &&
                move->move != Move::DreamEater // TODO
            ) {
                continue;
            }

            // Skip focus punch if the other pokemon is attacking
            if (move->move == Move::FocusPunch &&
                !is_player &&
                defender_state.chosen_move.move->category != Category::STATUS
            ) {
                continue;
            }
            const bool is_special = category == Category::SPECIAL;
            const auto attack_used =
                is_special ? special_attack : attack;
            const auto defense_used =
                is_special ? special_defense : defense;
            auto damage =
                get_damage_of_attacker_move(
                    attack_used,
                    move,
                    defender_state,
                    defense_used,
                    weather,
                    is_mid_turn
                );
            if (get_item_for_effect() == Item::LifeOrb) {
                damage = std::floor(damage * 1.3);
            }
            bool move_must_charge = false;
            if (move_has_flag(
                    move->move,
                    MoveFlag::REQUIRES_CHARGING_TURN
                )
            ) {
                move_must_charge = true;
                if (move->move == Move::SolarBeam &&
                    (weather == Weather::SUN ||
                        get_item_for_effect() == Item::PowerHerb)
                ) {
                    move_must_charge = false;
                }
            }

            int damage_from_defender;
            if (is_player) {
                const auto backup = defender_state.chosen_move;
                damage_from_defender =
                    defender_state.get_best_move_against_defender(
                        *this,
                        false,
                        weather,
                        is_mid_turn
                    ).damage;
                defender_state.chosen_move = backup;
            } else {
                damage_from_defender = defender_state.chosen_move.damage;
            }

            if ((!move_must_charge && !best_move_must_charge && damage >
                    best_move.damage) ||
                (best_move_must_charge && damage > best_move.damage / 2) ||
                (move_must_charge && damage > best_move.damage * 2 &&
                    ((damage_from_defender < get_health() && attacker_faster) ||
                        (damage_from_defender < get_health() / 2)))
            ) {
                best_move.damage = damage;
                best_move.move = move;
                best_move_must_charge = move_must_charge;
                if (move->type == PokemonType::FIRE) {
                    best_fire_move.move = move;
                    best_fire_move.damage = damage;
                } else if (move->type == PokemonType::WATER) {
                    best_water_move.move = move;
                    best_water_move.damage = damage;
                }
            }
        }

        // Check if it is better to use a status move
        if (best_move.damage != 0 && !is_choiced) {
            const auto hits =
                defender_state.get_health() / best_move.damage;
            for (const auto& move : pokemon.moves) {
                if (move_has_flag(
                        move->move,
                        MoveFlag::BOOSTS_ATTACKER_STAT)
                ) {
                    // TODO is_first check
                    if (best_move.move->category == Category::PHYSICAL) {
                        if (get_attack(weather) < 6) {
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
                        if (current_stats[SPECIAL_ATTACK_INDEX] < 6) {
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

        // Healing may be better too
        for (const auto& move : pokemon.moves) {
            if (move_has_flag(
                    move->move,
                    MoveFlag::HEALS_ATTACKER)
            ) {
                int potential_hp_gain = 0;

                if (move->move == Move::Absorb ||
                    move->move == Move::MegaDrain ||
                    move->move == Move::LeechLife ||
                    move->move == Move::GigaDrain ||
                    move->move == Move::DrainPunch
                ) {
                    const auto category = move->category;
                    const bool is_special = category == Category::SPECIAL;
                    const auto attack_used =
                        is_special ? special_attack : attack;
                    const auto defense_used =
                        is_special ? special_defense : defense;
                    auto damage =
                        get_damage_of_attacker_move(
                            attack_used,
                            move,
                            defender_state,
                            defense_used,
                            weather,
                            is_mid_turn
                        );

                    potential_hp_gain = damage / 2;

                    if (get_item() == Item::BigRoot) {
                        potential_hp_gain = std::floor(damage * 0.65);
                    }

                    potential_hp_gain = std::max(1, potential_hp_gain);
                    if (defender_state.get_ability() == Ability::LiquidOoze) {
                        potential_hp_gain = -potential_hp_gain;
                    }
                    if (get_health() < max_health / 2 &&
                        defender_state.last_used_move.damage < max_health / 2
                    ) {
                        best_move.move = move;
                        best_move.damage = damage;
                    }
                }

                if (move->move == Move::Recover ||
                    move->move == Move::Softboiled ||
                    move->move == Move::MilkDrink ||
                    move->move == Move::SlackOff ||
                    move->move == Move::HealOrder
                ) {
                    potential_hp_gain = max_health / 2;
                }
                if (move->move == Move::MorningSun ||
                    move->move == Move::Synthesis ||
                    move->move == Move::Moonlight
                ) {
                    if (weather == Weather::SUN) {
                        potential_hp_gain = 2 * max_health / 3;
                    } else if (weather == Weather::CLEAR) {
                        potential_hp_gain = max_health / 2;
                    } else {
                        potential_hp_gain = max_health / 4;
                    }
                    if (get_health() < max_health / 2 &&
                        defender_state.last_used_move.damage < max_health / 2
                    ) {
                        best_move.move = move;
                        best_move.damage = 0;
                    }
                }
            }
        }

        // Weather
        for (const auto& move : pokemon.moves) {
            if (move_has_flag(
                    move->move,
                    MoveFlag::CHANGES_WEATHER
                )
            ) {
                if (best_move.damage > 0) {
                    const auto hits_to_ko =
                        defender_state.get_health() / best_move.damage;
                    if (move->move == Move::SunnyDay &&
                        weather != Weather::SUN
                    ) {
                        if (defender_state.chosen_move.damage > 0) {
                            const auto hits_taken =
                                get_health() / defender_state.chosen_move.
                                damage;
                            if (hits_taken > hits_to_ko + 1 &&
                                best_fire_move.damage > 0
                            ) {
                                const auto hits_to_ko_if_fire =
                                    defender_state.get_health() /
                                    best_fire_move.damage;
                                if (hits_to_ko_if_fire > hits_to_ko + 1) {
                                    best_move.move = move;
                                    best_move.damage = 0;
                                }

                                if (best_move.move->move == Move::SolarBeam) {
                                    if (hits_to_ko > 1) {
                                        best_move.move = move;
                                        best_move.damage = 0;
                                    }
                                }
                            }
                            if (hits_taken > 3 &&
                                defender_state.chosen_move.move->type ==
                                PokemonType::WATER
                            ) {
                                best_move.move = move;
                                best_move.damage = 0;
                            }
                        }
                        if (pokemon.name == Pokemon::Castform) {
                            const int damage = get_weather_ball_damage(
                                defender_state,
                                attack,
                                special_attack,
                                defense,
                                special_defense,
                                Weather::SUN,
                                is_mid_turn
                            );
                            const auto new_hits_to_KO =
                                defender_state.get_health() / damage;
                            if (new_hits_to_KO > hits_to_ko) {
                                best_move.move = move;
                                best_move.damage = 0;
                            }
                        }
                    } else if (move->move == Move::RainDance &&
                        weather != Weather::RAIN
                    ) {
                        if (pokemon.name == Pokemon::Castform) {
                            const int damage = get_weather_ball_damage(
                                defender_state,
                                attack,
                                special_attack,
                                defense,
                                special_defense,
                                Weather::RAIN,
                                is_mid_turn
                            );
                            const auto new_hits_to_KO =
                                defender_state.get_health() / damage;
                            if (new_hits_to_KO > hits_to_ko) {
                                best_move.move = move;
                                best_move.damage = 0;
                            }
                        }
                        if (defender_state.chosen_move.damage > 0) {
                            const auto hits_taken =
                                get_health() /
                                defender_state.chosen_move.damage;
                            if (hits_taken > hits_to_ko + 1 &&
                                best_water_move.damage > 0
                            ) {
                                const auto hits_to_ko_if_water =
                                    defender_state.get_health() /
                                    best_water_move.damage;
                                if (hits_to_ko_if_water > hits_to_ko + 1) {
                                    best_move.move = move;
                                    best_move.damage = 0;
                                }
                            }
                            if (hits_taken > 3 &&
                                defender_state.chosen_move.move->type ==
                                PokemonType::WATER
                            ) {
                                best_move.move = move;
                                best_move.damage = 0;
                            }
                        }
                    } else if (move->move == Move::Hail &&
                        weather != Weather::HAIL
                    ) {
                        if (pokemon.name == Pokemon::Castform) {
                            const int damage = get_weather_ball_damage(
                                defender_state,
                                attack,
                                special_attack,
                                defense,
                                special_defense,
                                Weather::HAIL,
                                is_mid_turn
                            );
                            const auto new_hits_to_KO =
                                defender_state.get_health() / damage;
                            if (new_hits_to_KO > hits_to_ko) {
                                best_move.move = move;
                                best_move.damage = 0;
                            }
                        }
                    } else if (move->move == Move::Sandstorm &&
                        weather != Weather::SANDSTORM
                    ) {
                        if (pokemon.name == Pokemon::Castform) {
                            const int damage = get_weather_ball_damage(
                                defender_state,
                                attack,
                                special_attack,
                                defense,
                                special_defense,
                                Weather::SANDSTORM,
                                is_mid_turn
                            );
                            const auto new_hits_to_KO =
                                defender_state.get_health() / damage;
                            if (new_hits_to_KO > hits_to_ko) {
                                best_move.move = move;
                                best_move.damage = 0;
                            }
                        }
                        if (defender_state.chosen_move.damage > 0) {
                            const auto hits_taken =
                                get_health() /
                                defender_state.chosen_move.damage;
                            if (hits_taken > 3 &&
                                defender_state.has_type(PokemonType::ROCK)
                            ) {
                                best_move.move = move;
                                best_move.damage = 0;
                            }
                        }
                    }
                }
            }
        }

        if (CHOICE_ITEMS.contains(get_item_for_effect()) &&
            !is_choiced &&
            best_move.move != nullptr
        ) {
            is_choiced = true;
            chosen_move = best_move;
            return chosen_move;
        }
        if (is_choiced) {
            assert(last_used_move.move->move == best_move.move->move);
            last_used_move.damage = best_move.damage;
            chosen_move = last_used_move;
            return chosen_move;
        }
        if (defender_state.get_field_location() != FieldLocation::ON_FIELD &&
            attacker_faster &&
            best_move.move == nullptr
        ) {
            // For metronome since no move was picked
            last_used_move.damage = 0;
            chosen_move = last_used_move;
            return chosen_move;
        }

        chosen_move = best_move;
        return best_move;
    }

    void execute_move(
        PokemonState& defender_state,
        const Weather weather,
        const bool is_mid_turn
    ) {
        if (chosen_move.move == nullptr) {
            return;
        }

        if (defender_state.get_ability() == Ability::Damp &&
            (chosen_move.move->move == Move::Selfdestruct ||
                chosen_move.move->move == Move::Explosion)
        ) {
            return;
        }


        const auto attacker_move = chosen_move.move->move;
        auto& [defender_move, defender_move_damage] = defender_state.
            last_used_move;
        if (!is_mid_turn &&
            attacker_move == Move::Snatch &&
            move_has_flag(defender_move->move, MoveFlag::CAN_BE_SNATCHED)
        ) {
            // Snatchable moves do no damage
            chosen_move.move = defender_move;
            chosen_move.damage = 0;

            // Defender can no longer attack
            defender_move = nullptr;
            defender_move_damage = 0;
        }

        if (attacker_move == Move::BrickBreak) {
            defender_state.break_reflect();
            defender_state.break_light_screen();
            auto [move, damage] =
                get_best_move_against_defender(
                    defender_state,
                    true,
                    weather,
                    is_mid_turn
                );
            chosen_move.damage = damage;
        }

        // Sucker punch
        if (attacker_move == Move::SuckerPunch &&
            defender_move != nullptr &&
            defender_move->category == Category::STATUS
        ) {
            last_used_move.damage = 0;
            return;
        }

        // Charge move
        const bool requires_charging =
            move_has_flag(attacker_move, MoveFlag::REQUIRES_CHARGING_TURN);
        if (requires_charging && !is_charging()) {
            if (get_item_for_effect() == Item::PowerHerb) {
                clear_item();
            } else {
                start_charging();
                last_used_move.damage = 0;
                return;
            }
        }
        if (is_charging()) {
            assert(requires_charging);
            done_charging();
        }
        if (attacker_move == Move::FocusPunch && was_hit()) {
            last_used_move.damage = 0;
            return;
        }

        // Off field move
        bool attacker_vanished = false;
        if (field_location == FieldLocation::ON_FIELD) {
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
                if (get_item_for_effect() == Item::PowerHerb) {
                    clear_item();
                } else {
                    if (goes_into_air) {
                        field_location = FieldLocation::IN_AIR;
                        attacker_vanished = true;
                    } else if (goes_under_ground) {
                        field_location = FieldLocation::UNDER_GROUND;
                        attacker_vanished = true;
                    } else if (goes_under_water) {
                        field_location = FieldLocation::UNDER_WATER;
                        attacker_vanished = true;
                    } else if (goes_into_void) {
                        field_location = FieldLocation::IN_THE_VOID;
                        attacker_vanished = true;
                    }
                }
            }
        }
        if (!attacker_vanished &&
            field_location != FieldLocation::ON_FIELD
        ) {
            field_location = FieldLocation::ON_FIELD;
        }

        if (field_location != FieldLocation::ON_FIELD) {
            // Defender attack may miss or do double damage
            if (defender_move != nullptr) {
                auto [move, damage] =
                    defender_state.get_best_move_against_defender(
                        *this,
                        true,
                        weather,
                        is_mid_turn
                    );
                defender_move_damage = damage;
            }
        }

        if (was_hit() && attacker_move == Move::Avalanche) {
            auto [move, damage] =
                get_best_move_against_defender(
                    defender_state,
                    true,
                    weather,
                    is_mid_turn
                );
            chosen_move.damage = damage;
        }

        if (last_used_move.move == nullptr ||
            (last_used_move.move != nullptr && chosen_move.move != nullptr
                &&
                last_used_move.move->move == chosen_move.move->move)
        ) {
            metronome++;
        } else {
            metronome = 0;
        }
        last_used_move = chosen_move;

        // Do the damage
        if (field_location == FieldLocation::ON_FIELD) {
            if (move_has_flag(attacker_move, MoveFlag::BYPASSES_PROTECT) ||
                !defender_state.is_protected()
            ) {
                // Apply the damage
                if (defender_state.max_health == defender_state.get_health() &&
                    defender_state.get_health() <= chosen_move.damage &&
                    defender_state.get_item() == Item::FocusSash
                ) {
                    defender_state.apply_damage(
                        defender_state.get_health() - 1
                    );
                    defender_state.clear_item();
                } else {
                    defender_state.apply_damage(chosen_move.damage);
                }


                if (chosen_move.damage > 0) {
                    defender_state.set_was_hit();
                    if (move_has_flag(attacker_move, MoveFlag::CONTINUES)) {
                        if (is_player) {
                            defender_state.trapped_counter = 2;
                        } else {
                            defender_state.trapped_counter = 5;
                        }
                    }

                    if (defender_move != nullptr) {
                        const auto attacker_move_category =
                            chosen_move.move->category;
                        if (defender_move->move == Move::Counter) {
                            if (attacker_move_category ==
                                Category::PHYSICAL) {
                                defender_move_damage = chosen_move.damage *
                                    2;
                            }
                        } else if (
                            defender_move->move == Move::MirrorCoat
                        ) {
                            if (attacker_move_category ==
                                Category::SPECIAL) {
                                defender_move_damage = chosen_move.damage *
                                    2;
                            }
                        } else {
                            // Several moves do more damage after getting hit
                            auto [move, damage] =
                                defender_state.
                                get_best_move_against_defender(
                                    *this,
                                    true,
                                    weather,
                                    is_mid_turn
                                );
                            defender_move_damage = damage;
                        }
                    }
                }
            }
        }
    }
};

class BattleState {
    PokemonState player_state;
    PokemonState opponent_state;
    Weather weather = Weather::CLEAR;
    bool mid_turn = false;
    int weather_turns = 0;

public:
    BattleState(
        const CustomPokemon& player_pokemon,
        const CustomPokemon& opponent_pokemon
    ):
        player_state(player_pokemon, true),
        opponent_state(opponent_pokemon, false) {}

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
    }

    [[nodiscard]] bool is_mid_turn() const {
        return mid_turn;
    }

    void set_mid_turn() {
        mid_turn = true;
    }

    void end_turn() {
        mid_turn = false;
        if (weather_turns != -1 && weather_turns != 0) {
            weather_turns--;
            if (weather_turns == 0) {
                weather = Weather::CLEAR;
            }
        }
    }
};

#endif //POKEMONSTATE_H
