#ifndef POKEMONSTATE_H
#define POKEMONSTATE_H

#include <utility>

#include "custom_pokemon.h"
#include "nature.h"
#include "type_effectiveness.h"

enum class Status {
    BURN,
    POISON,
    BADLY_POISONED,
    PARALYZED,
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

enum class Weather {
    CLEAR,
    SUN,
    RAIN,
    HAIL,
    SANDSTORM,
    FOG
};

class BattleState {
    Weather weather = Weather::CLEAR;
    bool mid_turn = false;
    uint weather_turns = 0;

public:
    Weather get_weather(const Ability a0, const Ability a1) const {
        if (a0 != Ability::AirLock && a1 != Ability::AirLock) {
            return Weather::CLEAR;
        }
        return weather;
    }

    bool is_mid_turn() const {
        return mid_turn;
    }

    void set_mid_turn() {
        mid_turn = true;
    }

    void end_turn() {
        mid_turn = false;
    }
};

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

static std::array<
    bool,
    static_cast<int>(Item::None) + 1
> make_power_items() {
    std::array<bool, static_cast<int>(Item::None) + 1> array{};
    array.fill(false);
    for (const auto& item : ITEM_TO_TYPE | std::views::keys) {
        array[static_cast<int>(item)] = true;
    }
    return array;
}

static const auto POWER_ITEMS =
    make_power_items();


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

class PokemonState {
public:
    const bool is_player;
    const CustomPokemon pokemon;
    const uint8_t level;
    const std::array<PokemonType, 2> types;
    const int max_health;

private:
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
    PokemonState(CustomPokemon pokemon, const bool is_player):
        is_player(is_player),
        pokemon(std::move(pokemon)),
        level(pokemon.level),
        types(pokemon.types),
        max_health(pokemon.stats[HEALTH_INDEX]),
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
        grounded(item == Item::IronBall) {}

    [[nodiscard]] Ability get_ability() const {
        return ability;
    }

    [[nodiscard]] FieldLocation get_location_on_field() const {
        return field_location;
    }

    [[nodiscard]] Status get_status() const {
        return status;
    }

    void clear_status() {
        status = Status::NONE;
    }

    [[nodiscard]] bool is_confused() const {
        return confused;
    }

    void got_flinched() {
        flinched = true;
    }

    [[nodiscard]] bool is_flinched() const {
        return flinched;
    }

    void got_infatuated() {
        infatuated = true;
    }

    [[nodiscard]] bool was_hit() const {
        return was_hit_;
    }

    void got_hit() {
        was_hit_ = true;
    }


    [[nodiscard]] Item get_item() const {
        return item;
    }

    bool try_set_item(const Item item) {
        if (this->item == Item::None) {
            this->item = item;
            return true;
        }
        return false;
    }

    void clear_item() {
        item = Item::None;
    }

    [[nodiscard]] int32_t get_health() const {
        return current_stats[HEALTH_INDEX];
    }

    [[nodiscard]] int32_t get_attack() const {
        return current_stats[ATTACK_INDEX];
    }

    [[nodiscard]] int32_t get_defense() const {
        return current_stats[DEFENSE_INDEX];
    }

    [[nodiscard]] int32_t get_special_attack() const {
        return current_stats[SPECIAL_ATTACK_INDEX];
    }

    [[nodiscard]] int32_t get_special_defense() const {
        return current_stats[SPECIAL_DEFENSE_INDEX];
    }

    [[nodiscard]] int32_t get_speed() const {
        uint16_t speed = current_stats[SPEED_INDEX];
        if (item == Item::ChoiceScarf) {
            speed = std::floor(speed * 1.5);
        } else if (item == Item::IronBall) {
            speed = speed / 2;
        }
        return speed;
    }

    void continue_multi_turn_move(const uint8_t max_turns) {
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

    void change_stat_modifier(
        const Stat stat,
        const int8_t change
    ) {
        assert(change != 0 && change >= -6 && change <= 6);
        const int index = static_cast<int>(stat);
        const auto stage = stat_stages[index];
        const auto new_stage = static_cast<int8_t>(stage + change);
        if (change > 0) {
            stat_stages[index] = std::min(new_stage, MAX_STAT_STAGE);
        } else {
            if (new_stage < 0 && item == Item::WhiteHerb) {
                item = Item::None;
            } else {
                stat_stages[index] = std::max(new_stage, MIN_STAT_STAGE);
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

    void apply_berry() {
        if (const auto berry = item;
            berry == Item::ApicotBerry
        ) {
            change_stat_modifier(Stat::SPECIAL_DEFENSE, 1);
        } else if (berry == Item::ChestoBerry) {
            if (status == Status::SLEEP) {
                status = Status::NONE;
            }
        } else if (berry == Item::GanlonBerry) {
            change_stat_modifier(Stat::DEFENSE, 1);
        } else if (berry == Item::LiechiBerry) {
            change_stat_modifier(Stat::ATTACK, 1);
        } else if (berry == Item::LumBerry) {
            if (status != Status::NONE) {
                status = Status::NONE;
            }
            if (confused) {
                confused = false;
            }
        } else if (berry == Item::PersimBerry) {
            if (confused) {
                confused = false;
            }
        } else if (berry == Item::PetayaBerry) {
            change_stat_modifier(Stat::SPECIAL_ATTACK, 1);
        } else if (berry == Item::SalacBerry) {
            change_stat_modifier(Stat::SPEED, 1);
        } else if (berry == Item::SitrusBerry) {
            const auto current_health =
                current_stats[HEALTH_INDEX];
            current_stats[HEALTH_INDEX] =
                std::min(max_health, current_health + max_health / 4);
        }
        item = Item::None;
    }

    void try_apply_berry(const bool eaten) {
        if (BERRIES.contains(item)) {
            const int current_health = get_health();
            const bool less_than_half_health = current_health <= max_health / 2;
            const bool eats_stat_berry = STAT_BERRIES.contains(item) &&
            ((current_health <= max_health / 4) ||
                (ability == Ability::Gluttony && less_than_half_health)
            );
            const bool eats_status_berry =
                (item == Item::ChestoBerry && status == Status::SLEEP) ||
                (item == Item::PersimBerry && confused) ||
                (item == Item::LumBerry &&
                    (status != Status::NONE || confused)
                );
            const bool eats_sitrus = less_than_half_health;
            if (eaten ||
                eats_stat_berry ||
                eats_sitrus ||
                eats_status_berry
            ) {
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

    void apply_damage(const int damage) {
        current_stats[HEALTH_INDEX] -= damage;
        try_apply_berry(false);
    }

    void try_apply_status(const Status status) {
        if (status == Status::NONE) {
            this->status = status;
            try_apply_berry(false);
        }
    }

    void heal(const int health_gained) {
        current_stats[HEALTH_INDEX] += health_gained;
        if (current_stats[HEALTH_INDEX] > max_health) {
            current_stats[HEALTH_INDEX] = max_health;
        }
    }

    [[nodiscard]] bool has_type(const PokemonType type) const {
        return types[0] == type || types[1] == type;
    }

    void apply_end_of_turn_effects(
        const BattleState& battle_state,
        const PokemonState& defender_state
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
        if (item == Item::BlackSludge && has_type(PokemonType::POISON)) {
            heal(std::max(1, max_health / 16));
        }
        if (ability != Ability::MagicGuard) {
            if (trapped_counter > 0) {
                apply_damage(max_health / 8);
            }
            if (status == Status::BADLY_POISONED) {
                turns_badly_poisoned =
                    std::min(15, turns_badly_poisoned + 1);
                apply_damage(turns_badly_poisoned * (max_health / 16));
            }
            if (status == Status::BURN ||
                status == Status::POISON
            ) {
                apply_damage(max_health / 8);
            }
            if (item == Item::StickyBarb) {
                apply_damage(max_health / 8);
            }
            if (item == Item::BlackSludge && !has_type(PokemonType::POISON)) {
                apply_damage(std::max(1, max_health / 8));
            }
        }

        if (item == Item::FlameOrb && !has_type(PokemonType::FIRE)) {
            try_apply_status(Status::BURN);
        }
        if (trapped_counter > 0) {
            trapped_counter--;
        }

        // Weather
        const auto weather = battle_state.get_weather(
            ability,
            defender_state.get_ability()
        );
        if (weather == Weather::SANDSTORM &&
            !has_type(PokemonType::ROCK) &&
            !has_type(PokemonType::STEEL) &&
            !has_type(PokemonType::GROUND) &&
            ability != Ability::SandVeil &&
            ability != Ability::MagicGuard
        ) {
            apply_damage(max_health / 16);
        }

        if (weather == Weather::HAIL &&
            !has_type(PokemonType::ICE) &&
            ability != Ability::IceBody &&
            ability != Ability::SnowCloak &&
            ability != Ability::MagicGuard
        ) {
            apply_damage(max_health / 16);
        }

        if (item == Item::Leftovers) {
            heal(std::max(1, max_health / 16));
        }
        was_hit_ = false;
        flinched = false;
    }

    [[nodiscard]] bool is_charging() const {
        return charging;
    }

    //
    // void start_charging() {
    //     charging = true;
    // }
    //
    // void done_charging() {
    //     charging = false;
    // }

    void used_move_that_requires_recharge() {
        recharging = true;
    }

    [[nodiscard]] bool is_recharging() const {
        return recharging;
    }

    [[nodiscard]] bool outspeeds(
        const PokemonState& other_state,
        const MoveInfo* other_move,
        const MoveInfo* this_move
    ) const {
        if (is_player && other_state.get_item() == Item::QuickClaw) {
            return false;
        }
        const int this_priority = get_move_priority(this_move);
        const int other_priority = get_move_priority(other_move);
        if (this_priority != other_priority) {
            return this_priority > other_priority;
        }
        return get_speed() > other_state.get_speed();
    }

    int get_damage_of_attacker_move(
        const BattleState& battle_state,
        const uint16_t attacker_attack,
        const MoveInfo* attacker_move_info,
        const PokemonState& defender_state,
        const uint16_t defender_defense
    ) const {
        if (attacker_move_info->move == Move::Counter) {
            if (
                defender_state.chosen_move.move->category == Category::PHYSICAL
            ) {
                return defender_state.chosen_move.damage * 2;
            }
            return 0;
        }
        if (attacker_move_info->move == Move::MirrorCoat) {
            if (
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
        auto move_type = attacker_move_info->type;
        const auto status = get_status();
        const auto weather = battle_state.get_weather(
            ability,
            defender_state.ability
        );
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
        }

        // Items that increase power
        if (item == Item::Metronome) {
            const double multiplier = 1.0 + std::max(
                0.0,
                0.1 * get_metronome_count()
            );
            power = std::floor(power * multiplier);
        } else if ((item == Item::WiseGlasses &&
                attacker_move_info->category == Category::SPECIAL) ||
            (item == Item::MuscleBand &&
                attacker_move_info->category == Category::PHYSICAL)
        ) {
            power = std::floor(power * 1.1);
        } else if (pokemon.name == Pokemon::Pikachu &&
            get_item() == Item::LightBall
        ) {
            power = power * 2;
        } else if (POWER_ITEMS[static_cast<int>(item)] &&
            ITEM_TO_TYPE.at(item) == move_type
        ) {
            power = std::floor(power * 1.2);
        }

        // Field location
        const bool attacker_faster = outspeeds(
            defender_state,
            nullptr,
            attacker_move_info
        );
        if (const auto defender_field_location =
                defender_state.get_location_on_field();
            defender_field_location != FieldLocation::ON_FIELD &&
            attacker_faster
        ) {
            if (!attacker_faster && !battle_state.is_mid_turn()) {
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
        damage = std::floor(damage / 50) + 2;

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
        if (has_type(move_type)) {
            if (ability == Ability::Adaptability) {
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
        if (item == Item::ExpertBelt && effectiveness >= 2) {
            damage = std::floor(damage * 1.2);
        }

        // Random
        if (is_player) {
            damage = std::floor(damage * 0.85);
        }

        // Burn
        if (status == Status::BURN &&
            attacker_move_info->category == Category::PHYSICAL &&
            !move_has_flag(attacker_move, MoveFlag::HAS_FIXED_DAMAGE)
        ) {
            damage = std::floor(damage / 2);
        }

        if (attacker_move_info->move != Move::BrickBreak) {
            if ((defender_state.has_reflect_up() &&
                    attacker_move_info->category == Category::PHYSICAL) ||
                (defender_state.has_light_screen_up() &&
                    attacker_move_info->category == Category::SPECIAL)
            ) {
                damage = std::floor(damage / 2);
            }
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
        const BattleState& battle_state,
        const PokemonState& defender_state,
        const int32_t attack,
        const int32_t special_attack,
        const int32_t defense,
        const int32_t special_defense,
        const Weather weather
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
                        battle_state,
                        attack_used,
                        &new_move,
                        defender_state,
                        defense_used
                    );
                return damage;
            }
        }
        return 0;
    }

    BestMove get_best_move_against_defender(
        const BattleState& battle_state,
        const PokemonState& defender_state,
        const bool chosen_move_only
    ) {
        BestMove best_move{};
        bool best_move_must_charge = false;
        auto attack = get_attack();
        if (item == Item::ChoiceBand) {
            attack = std::floor(attack * 1.5);
        }
        auto special_attack = get_special_attack();
        if (item == Item::ChoiceSpecs) {
            special_attack = std::floor(special_attack * 1.5);
        }
        const auto defense = defender_state.get_defense();
        auto special_defense =
            defender_state.get_special_defense();

        const auto weather = battle_state.get_weather(
            get_ability(),
            defender_state.get_ability()
        );
        if (weather == Weather::SANDSTORM &&
            defender_state.has_type(PokemonType::ROCK)
        ) {
            special_defense = std::floor(special_defense * 1.5);
        }
        const bool attacker_faster = outspeeds(
            defender_state,
            nullptr,
            nullptr
        );
        if ((battle_state.is_mid_turn() || chosen_move_only) &&
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
                    battle_state,
                    attack_used,
                    chosen_move.move,
                    defender_state,
                    defense_used
                )
            };
            return chosen_move;
        }

        if (recharging) {
            // No move can be used during recharge
            recharging = false;
            return best_move;
        }

        if (get_location_on_field() != FieldLocation::ON_FIELD ||
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
                    battle_state,
                    attack_used,
                    move,
                    defender_state,
                    defense_used
                );
            if (item == Item::LifeOrb) {
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
                        item == Item::PowerHerb)
                ) {
                    move_must_charge = false;
                }
            }
            if ((!move_must_charge && !best_move_must_charge && damage >
                    best_move.damage) ||
                (best_move_must_charge && damage > best_move.damage / 2) ||
                (move_must_charge && damage > best_move.damage * 2)
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
                        if (get_attack() < 6) {
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
                            battle_state,
                            attack_used,
                            move,
                            defender_state,
                            defense_used
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
                                battle_state,
                                defender_state,
                                attack,
                                special_attack,
                                defense,
                                special_defense,
                                Weather::SUN
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
                                battle_state,
                                defender_state,
                                attack,
                                special_attack,
                                defense,
                                special_defense,
                                Weather::RAIN
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
                                battle_state,
                                defender_state,
                                attack,
                                special_attack,
                                defense,
                                special_defense,
                                Weather::HAIL
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
                                battle_state,
                                defender_state,
                                attack,
                                special_attack,
                                defense,
                                special_defense,
                                Weather::SANDSTORM
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

        if (CHOICE_ITEMS.contains(item) &&
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
            return last_used_move;
        }
        if (defender_state.field_location != FieldLocation::ON_FIELD &&
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
        BattleState& battle_state,
        PokemonState& defender_state
    ) {
        if (chosen_move.move == nullptr) {
            return;
        }

        const auto attacker_move = chosen_move.move->move;
        auto& [defender_move, defender_move_damage] = defender_state.
            last_used_move;
        if (!battle_state.is_mid_turn() &&
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
                    battle_state,
                    defender_state,
                    true
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
        if (requires_charging && !charging) {
            if (item == Item::PowerHerb) {
                clear_item();
            } else {
                charging = true;
                last_used_move.damage = 0;
                return;
            }
        }
        if (charging) {
            assert(requires_charging);
            charging = false;
        }
        if (attacker_move == Move::FocusPunch &&
            was_hit_
        ) {
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
                if (item == Item::PowerHerb) {
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
                        battle_state,
                        *this,
                        true
                    );
                defender_move_damage = damage;
            }
        }

        if (was_hit_ &&
            attacker_move == Move::Avalanche
        ) {
            auto [move, damage] =
                get_best_move_against_defender(
                    battle_state,
                    defender_state,
                    true
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
                const bool defender_had_max_hp =
                    defender_state.max_health == defender_state.
                    get_health();
                defender_state.apply_damage(chosen_move.damage);
                if (defender_had_max_hp &&
                    defender_state.get_health() <= 0 &&
                    defender_state.get_item() == Item::FocusSash
                ) {
                    defender_state.apply_damage(
                        defender_state.get_health() - 1
                    );
                    defender_state.clear_item();
                }
                if (chosen_move.damage > 0) {
                    defender_state.got_hit();
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
                                    battle_state,
                                    *this,
                                    true
                                );
                            defender_move_damage = damage;
                        }
                    }
                }
            }
        }
    }
};

#endif //POKEMONSTATE_H
