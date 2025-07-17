#ifndef POKEMONSTATE_H
#define POKEMONSTATE_H

#include <cassert>
#include <cmath>

#include "custom_pokemon.h"
#include "nature.h"

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
    uint damage = 0;
    uint times_to_hit = 1;
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

    std::vector<const MoveInfo*> moves;
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

    uint slow_start_count = 0;
    bool truant = false;
    bool unburdened = false;

    BestMove last_used_move{};
    BestMove chosen_move{};
    double pounds;

    bool first_turn = true;

    [[nodiscard]] Item get_item() const {
        return item;
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
            change_stat_modifier(
                Stat::SPECIAL_ATTACK,
                1,
                false
            );
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
        clear_item();
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

    uint get_damage_of_attacker_move(
        uint16_t attacker_attack,
        const MoveInfo* attacker_move_info,
        PokemonState& defender_state,
        uint16_t defender_defense,
        Weather weather,
        bool is_mid_turn
    );

    int get_weather_ball_damage(
        PokemonState& defender_state,
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

public:
    const bool is_player;
    const CustomPokemon pokemon;
    const uint8_t level;
    const int max_health;

    PokemonState(CustomPokemon pokemon, const bool is_player):
        moves(pokemon.moves),
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
        pounds(pokemon.pounds),
        is_player(is_player),
        pokemon(std::move(pokemon)),
        level(pokemon.level),
        max_health(pokemon.stats[HEALTH_INDEX]) {
        if (ability == Ability::Multitype &&
            PLATE_ITEMS[static_cast<int>(item)] &&
            pokemon.name == Pokemon::Arceus
        ) {
            types = {PLATE_ITEM_TYPES.at(item), PokemonType::COUNT};
        }
    }

    [[nodiscard]] std::vector<const MoveInfo*>& get_moves() {
        return moves;
    }

    [[nodiscard]] double get_weight() const {
        return pounds;
    }

    [[nodiscard]] bool is_confused() const {
        return confused;
    }

    void set_confused() {
        if (get_ability() != Ability::OwnTempo) {
            confused = true;
        }
    }

    [[nodiscard]] bool was_flash_fired() const {
        return this->flash_fired;
    }

    void done_recharging() {
        recharging = false;
    }

    [[nodiscard]] int get_metronome_count() const {
        return metronome;
    }

    [[nodiscard]] bool has_reflect_up() const {
        return reflect;
    }

    [[nodiscard]] bool has_light_screen_up() const {
        return light_screen;
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

    void set_ability(
        const Ability ability,
        const bool can_overwrite_truant
    ) {
        if (can_overwrite_truant || get_ability() != Ability::Truant) {
            this->ability = ability;
        }
    }

    void disable_ability() {
        ability = Ability::Disabled;
    }

    [[nodiscard]] Item get_item_for_effect() const {
        if (get_ability() != Ability::Klutz) {
            return item;
        }
        return Item::None;
    }

    bool try_set_item(const Item item) {
        if (this->item == Item::None) {
            this->item = item;
            grounded = this->item == Item::IronBall;
            if (ability == Ability::Multitype &&
                PLATE_ITEMS[static_cast<int>(item)] &&
                pokemon.name == Pokemon::Arceus
            ) {
                types = {PLATE_ITEM_TYPES.at(item), PokemonType::COUNT};
            }
            return true;
        }
        return false;
    }

    void clear_item() {
        if (item != Item::None) {
            unburdened = true;
        }
        item = Item::None;
        grounded = false;
        if (PLATE_ITEMS[static_cast<int>(item)] &&
            pokemon.name == Pokemon::Arceus &&
            ability == Ability::Multitype
        ) {
            types = pokemon.types;
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

    void apply_damage(const uint damage) {
        current_stats[HEALTH_INDEX] -= damage;
        if (current_stats[HEALTH_INDEX] > 0) {
            try_apply_berry(false);
        }
    }

    void heal(const int health_gained) {
        current_stats[HEALTH_INDEX] = std::min(
            max_health,
            current_stats[HEALTH_INDEX] + health_gained
        );
    }

    [[nodiscard]] int32_t get_attack(const Weather weather) const {
        uint16_t stat = current_stats[ATTACK_INDEX];
        const auto ability = get_ability();
        if (ability == Ability::FlowerGift &&
            weather == Weather::SUN
        ) {
            stat = std::floor(stat * 1.5);
        }
        if (ability == Ability::PurePower) {
            stat = stat * 2;
        } else if (ability == Ability::SlowStart &&
            slow_start_count < 5
        ) {
            stat = stat / 2;
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

    [[nodiscard]] int32_t get_special_attack(
        const Weather weather) const {
        uint16_t stat = current_stats[SPECIAL_ATTACK_INDEX];
        if (get_ability() == Ability::SolarPower &&
            weather == Weather::SUN
        ) {
            stat = std::floor(stat * 1.5);
        }
        return stat;
    }

    [[nodiscard]] int32_t get_special_defense(
        const Weather weather
    ) const {
        uint16_t stat = current_stats[SPECIAL_DEFENSE_INDEX];
        if (get_ability() == Ability::FlowerGift &&
            weather == Weather::SUN
        ) {
            stat = std::floor(stat * 1.5);
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

        const auto ability = get_ability();
        if ((ability == Ability::Chlorophyll &&
                weather == Weather::SUN) ||
            (ability == Ability::SwiftSwim &&
                weather == Weather::RAIN) ||
            (ability == Ability::Unburden && unburdened)
        ) {
            speed = speed * 2;
        } else if (ability == Ability::QuickFeet) {
            speed = std::floor(speed * 1.5);
        } else if (ability == Ability::SlowStart &&
            slow_start_count < 5
        ) {
            speed = speed / 2;
        }

        if (status == Status::PARALYZED &&
            ability != Ability::QuickFeet
        ) {
            speed = std::floor(speed * 0.25);
        }
        return speed;
    }

    void change_stat_modifier(
        const Stat stat,
        int change,
        const bool from_other
    ) {
        assert(change != 0 && change >= -6 && change <= 6);
        if (get_ability() == Ability::Simple) {
            change = change * 2;
        }
        const int index = static_cast<int>(stat);
        const auto stage = stat_stages[index];
        const auto new_stage = static_cast<int8_t>(stage + change);
        if (change > 0) {
            stat_stages[index] = std::min(new_stage, MAX_STAT_STAGE);
        } else {
            if (const auto ability = get_ability();
                !((ability == Ability::ClearBody ||
                    ability == Ability::WhiteSmoke) && from_other) &&
                !(ability == Ability::HyperCutter && from_other &&
                    stat == Stat::ATTACK)
            ) {
                if (get_item_for_effect() == Item::WhiteHerb) {
                    clear_item();
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

    [[nodiscard]] FieldLocation& get_field_location() {
        return field_location;
    }

    [[nodiscard]] Status get_status() const {
        return status;
    }

    void try_apply_status(
        const Status status,
        const Weather weather,
        PokemonState& other_state
    ) {
        if (const auto ability = get_ability();
            status == Status::NONE &&
            !((ability == Ability::Immunity ||
                    has_type(PokemonType::POISON) ||
                    has_type(PokemonType::STEEL)) &&
                (status == Status::POISON ||
                    status == Status::BADLY_POISONED)) &&
            !((ability == Ability::Insomnia ||
                ability == Ability::VitalSpirit) && status == Status::SLEEP) &&
            !(ability == Ability::LeafGuard && weather == Weather::SUN) &&
            !(ability == Ability::Limber && status == Status::PARALYZED) &&
            !(ability == Ability::MagmaArmor && status == Status::FROZEN) &&
            !((ability == Ability::WaterVeil || has_type(PokemonType::WATER)) &&
                status == Status::BURN)
        ) {
            this->status = status;
            if (ability == Ability::Synchronize &&
                (status == Status::BURN ||
                    status == Status::SLEEP ||
                    status == Status::POISON)
            ) {
                other_state.try_apply_status(status, weather, *this);
            }
            try_apply_berry(false);
        }
    }

    void clear_status() {
        status = Status::NONE;
    }

    [[nodiscard]] bool is_flinched() const {
        return flinched;
    }

    void set_flinched() {
        flinched = true;
    }

    void set_infatuated() {
        if (get_ability() != Ability::Oblivious) {
            infatuated = true;
        }
    }

    void set_flash_fire() {
        this->flash_fired = true;
    }

    void start_charging() {
        charging = true;
    }

    void done_charging() {
        charging = false;
    }

    [[nodiscard]] bool is_charging() const {
        return charging;
    }

    void used_move_that_requires_recharge() {
        recharging = true;
    }

    [[nodiscard]] bool is_recharging() const {
        return recharging;
    }

    void increment_multi_turn_move_counter(const uint8_t max_turns) {
        if (multi_turn_move_counter >= max_turns) {
            multi_turn_move_counter = 0;
        } else {
            multi_turn_move_counter++;
        }
    }

    void set_was_hit() {
        was_hit_ = true;
    }

    [[nodiscard]] bool was_hit() const {
        return was_hit_;
    }

    void clear_protect() {
        protected_ = false;
    }

    void break_reflect() {
        reflect = false;
    }

    void break_light_screen() {
        light_screen = false;
    }

    [[nodiscard]] bool is_first_turn() const {
        return first_turn;
    }

    [[nodiscard]] BestMove& get_last_used_move() {
        return last_used_move;
    }

    [[nodiscard]] BestMove& get_chosen_move() {
        return chosen_move;
    }

    void increase_metronome() {
        metronome++;
    }

    void clear_metronome() {
        metronome = 0;
    }

    [[nodiscard]] bool is_protected() const {
        return protected_;
    }

    void set_trapped_counter(const int turns) {
        trapped_counter = turns;
    }

    void transform(const PokemonState& other) {
        this->types = other.types;
        const auto health = this->current_stats[HEALTH_INDEX];
        this->current_stats = other.current_stats;
        this->current_stats[HEALTH_INDEX] = health;
        this->stat_stages = other.stat_stages;
        this->moves = other.moves;
        this->pounds = other.pounds;
        this->ability = other.ability;
        this->is_choiced = false;
    }

    void apply_end_of_turn_effects(
        Weather weather,
        PokemonState& defender_state
    );

    [[nodiscard]] bool outspeeds(
        const PokemonState& other_state,
        const MoveInfo* other_move,
        const MoveInfo* this_move,
        const Weather weather
    ) const {
        if (is_player && other_state.get_item() == Item::QuickClaw) {
            return false;
        }
        if (get_ability() == Ability::Stall) {
            return false;
        }
        const int this_priority = get_move_priority(this_move);
        const int other_priority = get_move_priority(other_move);
        if (this_priority != other_priority) {
            return this_priority > other_priority;
        }
        return get_speed(weather) > other_state.get_speed(weather);
    }

    BestMove get_best_move_against_defender(
        PokemonState& defender_state,
        bool chosen_move_only,
        Weather weather,
        bool is_mid_turn
    );

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
    );

    [[nodiscard]] Weather get_weather() const;

    /**
     * @param weather
     * @param turns -1 for weather that should not end.
     */
    void set_weather(
        Weather weather,
        int turns
    );

    [[nodiscard]] bool is_mid_turn() const;

    void set_mid_turn();

    void end_turn();

    void execute_move(
        PokemonState& attacker_state,
        PokemonState& defender_state,
        Weather weather,
        bool is_mid_turn
    );
};

#endif //POKEMONSTATE_H
