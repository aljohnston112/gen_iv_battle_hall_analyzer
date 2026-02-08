#ifndef POKEMONSTATE_H
#define POKEMONSTATE_H

#include <cassert>
#include <cmath>

#include "config.h"
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
    uint32_t damage = 0;
    uint32_t potential_damage = 0;
    uint32_t times_to_hit = 1;
};

inline uint16_t calculate_stat_based_on_stage(
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
    static constexpr int8_t HEALTH_INDEX =
        static_cast<int8_t>(Stat::HEALTH);
    static constexpr int8_t ATTACK_INDEX =
        static_cast<int8_t>(Stat::ATTACK);
    static constexpr int8_t DEFENSE_INDEX =
        static_cast<int8_t>(Stat::DEFENSE);
    static constexpr int8_t SPECIAL_ATTACK_INDEX =
        static_cast<int8_t>(Stat::SPECIAL_ATTACK);
    static constexpr int8_t SPECIAL_DEFENSE_INDEX =
        static_cast<int8_t>(Stat::SPECIAL_DEFENSE);
    static constexpr int8_t SPEED_INDEX =
        static_cast<int8_t>(Stat::SPEED);
    static constexpr int8_t MAX_STAT_STAGE = 6;
    static constexpr int8_t MIN_STAT_STAGE = -6;

    std::vector<const MoveInfo*> moves;
    std::array<int8_t, static_cast<int>(Move::Count)> power_points;
    std::array<PokemonType, 2> types;
    Ability ability;
    double pounds;
    Item item;
    std::array<int16_t, 6> current_stats;
    std::array<int8_t, 6> stat_stages = {0, 0, 0, 0, 0, 0};
    FieldLocation field_location = FieldLocation::ON_FIELD;

    Status status = Status::NONE;
    int turns_badly_poisoned = 0;
    int turns_asleep = 0;

    bool confused = false;
    bool infatuated = false;

    bool seeded = false;

    int trapped_counter = 0;
    bool flinched = false;
    bool was_hit_ = false;
    bool took_damage_ = false;

    bool charging = false;
    bool recharging = false;
    uint8_t multi_turn_move_counter = 0;

    bool grounded;
    int metronome = 0;
    bool is_choiced_ = false;

    bool protected_ = false;
    int safeguard_turns = 0;
    bool reflect = false;
    bool light_screen = false;

    bool flash_fired = false;
    uint8_t slow_start_count = 0;
    bool truant = false;
    bool unburdened = false;

    uint16_t rollout_power = 0;
    uint8_t rollout_turns = 0;

    uint16_t fury_cutter_power = 10;

    uint8_t stockpiles = 0;

    bool knocked_off = false;

    BestMove last_used_move{};
    BestMove chosen_move{};

    uint substitute = 0;
    bool had_sub_ = false;

    void apply_berry(bool eaten) {
        if (const auto berry = get_item_for_effect();
            berry == Item::ApicotBerry
        ) {
            change_stat_stage(
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
            change_stat_stage(
                Stat::DEFENSE,
                1,
                false
            );
            eaten = true;
        } else if (berry == Item::LiechiBerry) {
            change_stat_stage(
                Stat::ATTACK,
                1,
                false
            );
            eaten = true;
        } else if (berry == Item::LumBerry) {
            if (status != Status::NONE) {
                status = Status::NONE;
            } else if (confused) {
                confused = false;
            }
            eaten = true;
        } else if (berry == Item::PersimBerry) {
            if (confused) {
                confused = false;
            }
            eaten = true;
        } else if (berry == Item::PetayaBerry) {
            change_stat_stage(
                Stat::SPECIAL_ATTACK,
                1,
                false
            );
            eaten = true;
        } else if (berry == Item::SalacBerry) {
            change_stat_stage(
                Stat::SPEED,
                1,
                false
            );
            eaten = true;
        } else if (berry == Item::SitrusBerry) {
            const auto current_health = current_stats[HEALTH_INDEX];
            current_stats[HEALTH_INDEX] =
                std::min(
                    static_cast<int16_t>(max_health),
                    static_cast<int16_t>(current_health + max_health / 4)
                );
            eaten = true;
        } else if (berry == Item::CheriBerry) {
            if (status == Status::PARALYZED) {
                status = Status::NONE;
            }
            eaten = true;
        } else if (berry == Item::AspearBerry) {
            if (status == Status::FROZEN) {
                status = Status::NONE;
            }
            eaten = true;
        } else if (berry == Item::PechaBerry) {
            if (status == Status::POISON || status == Status::BADLY_POISONED) {
                status = Status::NONE;
            }
            eaten = true;
        } else if (berry == Item::RawstBerry) {
            if (status == Status::BURN) {
                status = Status::NONE;
            }
            eaten = true;
        }
        if (!eaten) {
            throw std::runtime_error("Unimplemented berry");
        }
        clear_item();
    }

    uint16_t get_stat(
        const Weather weather,
        const Ability other_ability,
        const Stat stat_enum
    ) {
        const auto stat_index = static_cast<int8_t>(stat_enum);
        uint16_t stat = current_stats.at(stat_index);
        if (other_ability == Ability::Unaware) {
            const int8_t backup = stat_stages.at(stat_index);
            stat_stages[stat_index] = 0;
            switch (stat_enum) {
            case Stat::ATTACK:
                stat = get_attack(weather, Ability::Disabled);
                break;
            case Stat::DEFENSE:
                stat = get_defense(weather, Ability::Disabled);
                break;
            case Stat::SPECIAL_ATTACK:
                stat = get_special_attack(weather, Ability::Disabled);
                break;
            case Stat::SPECIAL_DEFENSE:
                stat = get_special_defense(weather, Ability::Disabled);
                break;
            default:
                break;
            }
            stat_stages[stat_index] = backup;
        }
        return stat;
    }

public:
    const bool is_player;
    CustomPokemon* pokemon;
    const uint8_t level;
    const uint16_t max_health;

    PokemonState(CustomPokemon* pokemon, const bool is_player) :
        moves(pokemon->moves),
        types(pokemon->types),
        ability(pokemon->ability),
        pounds(pokemon->pounds),
        item(pokemon->item),
        current_stats(
            {
                static_cast<int16_t>(pokemon->stats[HEALTH_INDEX]),
                static_cast<int16_t>(pokemon->stats[ATTACK_INDEX]),
                static_cast<int16_t>(pokemon->stats[DEFENSE_INDEX]),
                static_cast<int16_t>(pokemon->stats[SPECIAL_ATTACK_INDEX]),
                static_cast<int16_t>(pokemon->stats[SPECIAL_DEFENSE_INDEX]),
                static_cast<int16_t>(pokemon->stats[SPEED_INDEX])
            }
        ),
        grounded(item == Item::IronBall),
        is_player(is_player),
        pokemon(pokemon),
        level(pokemon->level),
        max_health(pokemon->stats[HEALTH_INDEX]) {
        power_points.fill(0);
        for (const auto& move : moves) {
            power_points[static_cast<int>(move->move)] = move->power_points;
        }
        if (ability == Ability::Multitype &&
            PLATE_ITEMS[static_cast<int>(item)] &&
            pokemon->name == Pokemon::Arceus
        ) {
            types = {
                PLATE_ITEM_TYPES[static_cast<int>(item)],
                PokemonType::COUNT
            };
        }
    }

    [[nodiscard]] uint get_subs_health() const {
        return substitute;
    }

    [[nodiscard]] const std::vector<const MoveInfo*>& get_moves() {
        return moves;
    }

    [[nodiscard]] const std::array<PokemonType, 2>& get_types() const {
        return types;
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
        if (can_overwrite_truant ||
            get_ability() != Ability::Truant
        ) {
            this->ability = ability;
        }
    }

    void disable_ability() {
        ability = Ability::Disabled;
    }

    [[nodiscard]] double get_pounds() const {
        return pounds;
    }

    [[nodiscard]] Item get_item() const {
        return item;
    }

    [[nodiscard]] Item get_item_for_effect() const {
        if (get_ability() != Ability::Klutz) {
            return item;
        }
        return Item::None;
    }

    bool try_set_item(const Item item) {
        if (this->item == Item::None && !knocked_off) {
            this->item = item;
            grounded = this->item == Item::IronBall;
            if (ability == Ability::Multitype &&
                PLATE_ITEMS[static_cast<int>(item)] &&
                pokemon->name == Pokemon::Arceus
            ) {
                types = {
                    PLATE_ITEM_TYPES[static_cast<int>(item)],
                    PokemonType::COUNT
                };
            }
            return true;
        }
        return false;
    }

    void clear_item(const bool knocked_off = false) {
        if (item != Item::None) {
            unburdened = true;
            this->knocked_off = knocked_off;
        }
        item = Item::None;
        grounded = false;
        if (PLATE_ITEMS[static_cast<int>(item)] &&
            pokemon->name == Pokemon::Arceus &&
            ability == Ability::Multitype
        ) {
            types = pokemon->types;
        }
    }

    void try_apply_berry(const bool eaten) {
        if (const auto item = get_item_for_effect();
            BERRIES[static_cast<int>(item)]
        ) {
            const int current_health = get_health();
            const bool less_than_half_health = current_health <= max_health / 2;
            const bool eats_stat_berry =
                STAT_BERRIES.contains(item) &&
                ((current_health <= max_health / 4) ||
                    (get_ability() == Ability::Gluttony &&
                        less_than_half_health)
                );
            const bool eats_status_berry =
                (item == Item::ChestoBerry && status == Status::SLEEP) ||
                (item == Item::CheriBerry && status == Status::PARALYZED) ||
                (item == Item::RawstBerry && status == Status::BURN) ||
                (item == Item::AspearBerry && status == Status::FROZEN) ||
                (item == Item::PersimBerry && confused) ||
                (item == Item::LumBerry &&
                    (status != Status::NONE || confused)
                ) ||
                (item == Item::PechaBerry &&
                    (status == Status::POISON ||
                        status == Status::BADLY_POISONED)
                );
            const bool eats_sitrus =
                item == Item::SitrusBerry && less_than_half_health;
            const bool eats_berry =
                eaten ||
                eats_stat_berry ||
                eats_status_berry ||
                eats_sitrus;
            if (eats_berry) {
                apply_berry(eaten);
            }
        }
    }

    void eat_berry(const Item item) {
        const Item backup = this->item;
        this->item = item;
        try_apply_berry(true);
        this->item = backup;
    }

    [[nodiscard]] int16_t get_health() const {
        return current_stats[HEALTH_INDEX];
    }

    void apply_damage(const uint16_t damage) {
        current_stats[HEALTH_INDEX] =
            static_cast<int16_t>(current_stats[HEALTH_INDEX] - damage);
        took_damage_ = true;
        if (current_stats[HEALTH_INDEX] > 0) {
            try_apply_berry(false);
        }
    }

    void heal(const uint16_t health_gained) {
        current_stats[HEALTH_INDEX] = std::min(
            static_cast<int16_t>(max_health),
            static_cast<int16_t>(current_stats[HEALTH_INDEX] + health_gained)
        );
    }

    [[nodiscard]] uint16_t get_attack(
        const Weather weather,
        const Ability other_ability
    ) {
        uint16_t stat = get_stat(weather, other_ability, Stat::ATTACK);
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
        if (const auto item = get_item_for_effect();
            (pokemon->name == Pokemon::Cubone ||
                pokemon->name == Pokemon::Marowak
            ) && item == Item::ThickClub
        ) {
            stat *= 2;
        }
        return stat;
    }

    [[nodiscard]] uint16_t get_defense(
        const Weather weather,
        const Ability other_ability
    ) {
        uint16_t stat = get_stat(weather, other_ability, Stat::DEFENSE);
        if (get_ability() == Ability::MarvelScale &&
            status != Status::NONE
        ) {
            stat = std::floor(stat * 1.5);
        }
        return stat;
    }

    [[nodiscard]] uint16_t get_special_attack(
        const Weather weather,
        const Ability other_ability
    ) {
        uint16_t stat = get_stat(weather, other_ability, Stat::SPECIAL_ATTACK);
        if (get_ability() == Ability::SolarPower &&
            weather == Weather::SUN
        ) {
            stat = std::floor(stat * 1.5);
        }
        if (const auto item = get_item_for_effect();
            pokemon->name == Pokemon::Clamperl && item == Item::DeepSeaTooth) {
            stat *= 2;
        }
        return stat;
    }

    [[nodiscard]] uint16_t get_special_defense(
        const Weather weather,
        const Ability other_ability
    ) {
        uint16_t stat = get_stat(weather, other_ability, Stat::SPECIAL_DEFENSE);
        if (get_ability() == Ability::FlowerGift &&
            weather == Weather::SUN
        ) {
            stat = std::floor(stat * 1.5);
        }
        if (const auto item = get_item_for_effect();
            pokemon->name == Pokemon::Clamperl && item == Item::DeepSeaScale) {
            stat *= 2;
        }
        return stat;
    }

    [[nodiscard]] uint16_t get_speed(const Weather weather) const {
        uint16_t speed = current_stats[SPEED_INDEX];
        if (const auto item = get_item_for_effect();
            item == Item::ChoiceScarf
        ) {
            speed = speed * 3 / 2;
        } else if (item == Item::IronBall) {
            speed = speed / 2;
        }

        const auto ability = get_ability();
        if (weather != Weather::CLEAR &&
            ((ability == Ability::Chlorophyll &&
                    weather == Weather::SUN) ||
                (ability == Ability::SwiftSwim &&
                    weather == Weather::RAIN)) ||
            (ability == Ability::Unburden && unburdened)
        ) {
            speed = speed * 2;
        } else if (ability == Ability::QuickFeet) {
            speed = speed * 3 / 2;
        } else if (ability == Ability::SlowStart &&
            slow_start_count < 5
        ) {
            speed = speed / 2;
        }

        if (status == Status::PARALYZED &&
            ability != Ability::QuickFeet
        ) {
            speed = speed / 4;
        }
        return speed;
    }

    [[nodiscard]] int8_t get_attack_stage() const {
        return stat_stages.at(ATTACK_INDEX);
    }

    [[nodiscard]] int8_t get_defense_stage() const {
        return stat_stages.at(DEFENSE_INDEX);
    }

    [[nodiscard]] int8_t get_special_attack_stage() const {
        return stat_stages.at(SPECIAL_ATTACK_INDEX);
    }

    [[nodiscard]] int8_t get_special_defense_stage() const {
        return stat_stages.at(SPECIAL_DEFENSE_INDEX);
    }

    [[nodiscard]] int8_t get_speed_stage() const {
        return stat_stages.at(SPEED_INDEX);
    }

    void change_stat_stage(
        const Stat stat,
        int8_t stage_change,
        const bool from_other
    ) {
        assert(stage_change != 0 && stage_change >= -6 && stage_change <= 6);
        if (get_ability() == Ability::Simple) {
            stage_change = static_cast<int8_t>(stage_change * 2);
        }
        const auto index = static_cast<int8_t>(stat);
        const auto stage = stat_stages[index];
        const auto new_stage = static_cast<int8_t>(stage + stage_change);
        if (stage_change > 0) {
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
            static_cast<int16_t>(
                calculate_stat_based_on_stage(
                    pokemon->stats[index],
                    stat_stages[index]
                )
            );
    }

    void clear_negative_stat_changes() {
        int i = 0;
        for (const auto stat_stage : stat_stages) {
            if (stat_stage < 0) {
                stat_stages[i] = 0;
                current_stats[i] =
                    static_cast<int16_t>(
                        calculate_stat_based_on_stage(
                            pokemon->stats[i],
                            stat_stages[i]
                        )
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

    void try_apply_status(
        const Status status,
        const Weather weather,
        PokemonState& other_state
    ) {
        if (const auto ability = get_ability();
            get_status() == Status::NONE &&
            !((status == Status::BADLY_POISONED ||
                    status == Status::POISON) &&
                ability == Ability::Immunity ||
                has_type(PokemonType::POISON) ||
                has_type(PokemonType::STEEL)) &&
            !(status == Status::SLEEP &&
                (ability == Ability::Insomnia ||
                    ability == Ability::VitalSpirit)) &&
            !(status == Status::BURN &&
                (ability == Ability::WaterVeil ||
                    has_type(PokemonType::WATER))) &&
            !(status == Status::PARALYZED && ability == Ability::Limber) &&
            !(status == Status::FROZEN && ability == Ability::MagmaArmor) &&
            !(weather == Weather::SUN && ability == Ability::LeafGuard) ||
            !((status == Status::BURN ||
                    status == Status::POISON ||
                    status != Status::BADLY_POISONED) &&
                safeguard_turns > 0)
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

    void try_apply_sleep(
        const int turns,
        const Weather weather,
        PokemonState& other_state
    ) {
        try_apply_status(
            Status::SLEEP,
            weather,
            other_state
        );
        turns_asleep = turns;
    }


    void clear_status() {
        status = Status::NONE;
    }

    [[nodiscard]] bool is_confused() const {
        return confused;
    }

    void set_confused() {
        if (get_ability() != Ability::OwnTempo) {
            confused = true;
        }
    }

    void set_infatuated() {
        if (get_ability() != Ability::Oblivious) {
            infatuated = true;
        }
    }

    void set_seeded() {
        seeded = true;
    }

    void clear_seeded() {
        seeded = false;
    }

    int apply_damage_if_seeded() {
        int damage = 0;
        if (seeded) {
            damage = std::max(1, max_health / 8);
            apply_damage(damage);
        }
        return damage;
    }

    void set_trapped_counter(const int turns) {
        trapped_counter = turns;
    }

    [[nodiscard]] bool is_flinched() const {
        return flinched;
    }

    void set_flinched() {
        flinched = true;
        rollout_power = 0;
        rollout_turns = 0;
        fury_cutter_power = 10;
    }

    [[nodiscard]] bool was_hit() const {
        return was_hit_;
    }

    void set_was_hit() {
        was_hit_ = true;
    }

    [[nodiscard]] bool took_damage() const {
        return took_damage_;
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

    [[nodiscard]] bool is_recharging() const {
        return recharging;
    }

    void used_move_that_requires_recharge() {
        recharging = true;
    }

    void done_recharging() {
        recharging = false;
    }

    [[nodiscard]] uint8_t get_multi_turn_move_counter() const {
        return multi_turn_move_counter;
    }

    void increment_multi_turn_move_counter(const uint8_t max_turns) {
        if (multi_turn_move_counter >= max_turns) {
            multi_turn_move_counter = 0;
        } else {
            multi_turn_move_counter++;
        }
    }

    [[nodiscard]] bool is_grounded() const {
        return grounded;
    }

    [[nodiscard]] int get_metronome_count() const {
        return metronome;
    }

    void increase_metronome() {
        metronome++;
    }

    void clear_metronome() {
        metronome = 0;
    }

    [[nodiscard]] bool is_choiced() const {
        return is_choiced_;
    }

    void set_is_choiced() {
        is_choiced_ = true;
    }

    [[nodiscard]] bool is_protected() const {
        return protected_;
    }

    void clear_protect() {
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

    [[nodiscard]] bool was_flash_fired() const {
        return this->flash_fired;
    }

    void set_flash_fire() {
        this->flash_fired = true;
    }

    [[nodiscard]] bool is_truant() const {
        return truant;
    }

    [[nodiscard]] uint16_t get_rollout_power() const {
        return rollout_power;
    }

    void set_rollout_power(const uint16_t rollout_power) {
        this->rollout_power = rollout_power;
    }

    [[nodiscard]] uint16_t get_rollout_turns() const {
        return rollout_turns;
    }

    void start_safeguard() {
        this->safeguard_turns = 5;
    }

    void start_rollout() {
        this->rollout_turns = 5;
    }

    void stop_rollout() {
        this->rollout_turns = 0;
        rollout_power = 0;
    }

    int16_t get_fury_cutter_power() const {
        return fury_cutter_power;
    }

    void set_fury_cutter_power(const uint16_t power) {
        this->fury_cutter_power = power;
    }

    void add_stockpile() {
        if (stockpiles < 3) {
            change_stat_stage(
                Stat::DEFENSE,
                1,
                false
            );
            change_stat_stage(
                Stat::SPECIAL_DEFENSE,
                1,
                false
            );
        }
        stockpiles = std::min(3, stockpiles + 1);
    }

    void clear_stockpile() {
        change_stat_stage(
            Stat::DEFENSE,
            -stockpiles,
            false
        );
        change_stat_stage(
            Stat::SPECIAL_DEFENSE,
            -stockpiles,
            false
        );
        stockpiles = 0;
    }

    [[nodiscard]] BestMove get_last_used_move() const {
        return last_used_move;
    }

    [[nodiscard]] BestMove get_chosen_move() const {
        return chosen_move;
    }

    void set_chosen_move(BestMove&& best_move) {
        this->chosen_move = best_move;
    }

    void update_last_used_move(
        const bool move_failed,
        const Ability defender_ability
    ) {
        if (!move_failed) {
            last_used_move = BestMove{
                .move = chosen_move.move,
                .damage = chosen_move.damage,
                .potential_damage = chosen_move.potential_damage,
                .times_to_hit = 1
            };
        } else {
            last_used_move = BestMove{
                .move = nullptr,
                .damage = 0,
                .potential_damage = 0,
                .times_to_hit = 0
            };
        }
        if (last_used_move.move != nullptr) {
            assert(
                power_points[static_cast<int>(chosen_move.move->move)] > 0 ||
                (chosen_move.move->move == Move::Struggle &&
                    !has_power_points())
            );
            if (field_location == FieldLocation::ON_FIELD) {
                power_points[static_cast<int>(chosen_move.move->move)]--;
                if (defender_ability == Ability::Pressure) {
                    power_points[static_cast<int>(chosen_move.move->move)]--;
                }
            }
        }
    }

    void transform(const PokemonState& other) {
        this->types = other.types;
        const auto health = this->current_stats[HEALTH_INDEX];
        this->current_stats = other.current_stats;
        this->current_stats[HEALTH_INDEX] = health;
        this->stat_stages = other.stat_stages;
        this->moves = other.moves;
        this->power_points.fill(0);
        for (const auto& move : this->moves) {
            this->power_points[static_cast<int>(move->move)] =
                static_cast<int8_t>(std::min(move->power_points, 5));
        }
        // Needed since PP will be deducted
        this->power_points[static_cast<int>(Move::Transform)] = 1;
        this->pounds = other.pounds;
        this->ability = other.ability;
        this->is_choiced_ = false;
    }

    [[nodiscard]] bool outspeeds(
        const PokemonState& other_state,
        const MoveInfo* other_move,
        const MoveInfo* this_move,
        const Weather weather
    ) const {
        if (is_player && other_state.get_item_for_effect() == Item::QuickClaw) {
            return false;
        }
        if (get_ability() == Ability::Stall) {
            return false;
        }
        const int8_t this_priority = get_move_priority(this_move);
        const int8_t other_priority = get_move_priority(other_move);
        if (this_priority != other_priority) {
            return this_priority > other_priority;
        }
        return get_speed(weather) > other_state.get_speed(weather);
    }

    int get_weather_ball_damage(
        const MoveInfo* attacker_move_info,
        PokemonState& defender_state,
        const Weather weather,
        const bool is_mid_turn
    ) {
        assert(attacker_move_info->move == Move::WeatherBall);
        MoveInfo new_move = *attacker_move_info;
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
                item,
                &new_move,
                defender_state,
                weather,
                is_mid_turn
            );
        return damage;
    }

    uint32_t get_damage_of_attacker_move(
        Item attacker_item,
        const MoveInfo* attacker_move_info,
        PokemonState& defender_state,
        Weather weather,
        bool is_mid_turn
    );

    void try_apply_weather_damage(
        const Weather weather,
        const Ability ability
    ) {
        const bool has_magic_guard = ability == Ability::MagicGuard;
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
    }

    void try_apply_weather_healing(
        const Weather weather,
        const Ability ability
    ) {
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
            ability == pokemon->ability
        ) {
            heal(max_health / 8);
        }
        if (ability == Ability::RainDish &&
            weather == Weather::RAIN
        ) {
            heal(max_health / 16);
        }
    }

    void try_apply_speed_boost_or_shed_skin(
        const Ability ability) {
        if (ability == Ability::SpeedBoost) {
            change_stat_stage(Stat::SPEED, 1, false);
        }
        if (ability == Ability::ShedSkin && !is_player) {
            clear_status();
        }
    }

    void try_apply_leftovers_or_black_sludge(
        const Item item,
        const Ability ability
    ) {
        const bool has_magic_guard = ability == Ability::MagicGuard;
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
    }

    void try_apply_status_damage(const Ability ability) {
        const bool has_magic_guard = ability == Ability::MagicGuard;
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
    }

    void try_apply_flame_or_toxic_orb(
        const Weather weather,
        PokemonState& defender_state,
        const Item item
    ) {
        if (item == Item::FlameOrb && !has_type(PokemonType::FIRE)) {
            try_apply_status(Status::BURN, weather, defender_state);
        }
        if (item == Item::ToxicOrb &&
            (!has_type(PokemonType::STEEL) && !has_type(PokemonType::POISON))
        ) {
            try_apply_status(Status::BADLY_POISONED, weather, defender_state);
        }
    }

    void try_apply_trap_damage(const Ability ability) {
        if (const bool has_magic_guard = ability == Ability::MagicGuard;
            !has_magic_guard && trapped_counter > 0
        ) {
            apply_damage(max_health / 8);
        }
        if (trapped_counter > 0) {
            trapped_counter--;
        }
    }

    void try_apply_sticky_barb(
        const Item item,
        const Ability ability
    ) {
        if (const bool has_magic_guard = ability == Ability::MagicGuard;
            !has_magic_guard && item == Item::StickyBarb
        ) {
            apply_damage(max_health / 8);
        }
    }

    void update_end_of_turn(const Ability ability) {
        if (rollout_power > 0) {
            rollout_turns--;
        }
        if (rollout_turns != 0) {
            rollout_power *= 2;
        } else {
            stop_rollout();
        }
        if (safeguard_turns > 0) {
            safeguard_turns--;
        }
        was_hit_ = false;
        took_damage_ = false;
        flinched = false;
        if (slow_start_count < 5) {
            slow_start_count++;
        }
        if (ability == Ability::Truant) {
            truant = !truant;
            recharging = false;
        }
        if (substitute == 0) {
            had_sub_ = false;
        }
        if (turns_asleep > 0) {
            turns_asleep--;
            if (turns_asleep == 0) {
                clear_status();
            }
        }
    }

    void apply_end_of_turn_effects(
        const Weather weather,
        PokemonState& defender_state
    ) {
        if (defender_state.get_health() <= 0) {
            return;
        }
        const auto item = get_item_for_effect();
        const auto ability = get_ability();

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
        try_apply_weather_damage(weather, ability);
        // 4.0 Dry Skin, Hydration, Ice Body, Rain Dish
        try_apply_weather_healing(weather, ability);

        // 5.0 Gravity
        //
        // 6.0 Ingrain
        // 6.1 Aqua Ring
        // 6.2 Speed Boost, Shed Skin
        try_apply_speed_boost_or_shed_skin(ability);
        // 6.3 Black Sludge, Leftovers: "pokémon restored a little HP using its leftovers"
        try_apply_leftovers_or_black_sludge(item, ability);

        // 6.4 Leech Seed: "pokémon's health is sapped by leech seed"
        int seed_damage = apply_damage_if_seeded();
        if (defender_state.get_item_for_effect() == Item::BigRoot) {
            seed_damage *= 1.3;
        }
        if (ability == Ability::LiquidOoze) {
            defender_state.apply_damage(seed_damage);
        } else if (defender_state.get_health() > 0) {
            defender_state.heal(seed_damage);
        }

        // 6.5 Burn, Nightmare, Poison Heal, Poison: "pokémon is hurt by poison"
        try_apply_status_damage(ability);

        // 6.6 Flame Orb activation, Toxic Orb activation
        try_apply_flame_or_toxic_orb(weather, defender_state, item);
        // 6.7 Curse (from a Ghost)
        // 6.8 Bind, Clamp, Fire Spin, Magma Storm, Sand Tomb, Whirlpool, Wrap
        try_apply_trap_damage(ability);

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
        try_apply_sticky_barb(item, ability);
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

        update_end_of_turn(ability);
    }

    [[nodiscard]] bool has_power_points() const {
        bool result = false;
        // #pragma omp parallel for reduction(|:result) num_threads(NUMBER_OF_THREADS)
        for (size_t i = 0; i < moves.size(); ++i) {
            result |= has_power_points(moves[i]->move);
        }
        return result;
    }

    [[nodiscard]] bool has_power_points(Move move) const {
        return power_points[static_cast<int>(move)] > 0;
    }

    [[nodiscard]] int8_t get_power_points_left(Move move) const {
        return power_points[static_cast<int>(move)];
    }

    void set_field_location(const FieldLocation field_location) {
        this->field_location = field_location;
    }

    bool had_sub() const {
        return had_sub_;
    }

    void apply_substitute() {
        assert(
            get_health() > max_health / 4 ||
            pokemon->name == Pokemon::Shuckle
        );
        if (get_health() <= max_health / 4) {
            return;
        }
        apply_damage(max_health / 4);
        assert(substitute == 0);
        substitute = max_health / 4;
        had_sub_ = true;
        if (trapped_counter > 0) {
            trapped_counter = 0;
        }
    }

    void apply_damage_to_sub(const int32_t damage) {
        assert(substitute > 0);
        substitute = std::max(0, static_cast<int>(substitute) - damage);
    }
};

inline bool check_transform(PokemonState& attacker_state) {
    if (const auto& all_attacker_moves =
            attacker_state.get_moves();
        all_attacker_moves.size() == 1 &&
        all_attacker_moves[0]->move == Move::Transform
    ) {
        attacker_state.set_chosen_move(
            BestMove{
                .move = all_attacker_moves[0],
                .damage = 0,
                .potential_damage = 0,
                .times_to_hit = 1
            }
        );
        return true;
    }
    return false;
}

inline void recalculate_chosen_move_damage(
    PokemonState& attacker_state,
    PokemonState& defender_state,
    const BestMove& attacker_chosen_move,
    const Weather weather,
    const bool is_mid_turn
) {
    if (attacker_chosen_move.move == nullptr ||
        attacker_chosen_move.move->category == Category::STATUS
    ) {
        return;
    }
    const auto damage = attacker_state.get_damage_of_attacker_move(
        attacker_state.get_item_for_effect(),
        attacker_chosen_move.move,
        defender_state,
        weather,
        is_mid_turn
    );
    attacker_state.set_chosen_move(
        BestMove{
            .move = attacker_chosen_move.move,
            .damage = damage,
            .potential_damage = damage,
            .times_to_hit = attacker_chosen_move.times_to_hit
        }
    );
}

inline uint8_t get_times_to_hit(
    const bool attacker_is_player,
    const MoveInfo* move
) {
    uint8_t times_to_hit = 1;
    if (move_has_flag(
            move->move,
            MoveFlag::HITS_MULTIPLE_TIMES
        )
    ) {
        if (move->move == Move::Bonemerang ||
            move->move == Move::DoubleHit ||
            move->move == Move::DoubleKick ||
            move->move == Move::Twineedle
        ) {
            times_to_hit = 2;
        } else if (move->move == Move::ArmThrust ||
            move->move == Move::Barrage ||
            move->move == Move::BoneRush ||
            move->move == Move::BulletSeed ||
            move->move == Move::CometPunch ||
            move->move == Move::DoubleSlap ||
            move->move == Move::FuryAttack ||
            move->move == Move::FurySwipes ||
            move->move == Move::IcicleSpear ||
            move->move == Move::PinMissile ||
            move->move == Move::RockBlast ||
            move->move == Move::SpikeCannon
        ) {
            if (attacker_is_player) {
                times_to_hit = 2;
            } else {
                times_to_hit = 5;
            }
        } else if (move->move == Move::TripleKick) {
            times_to_hit = 3;
        }
    }
    return times_to_hit;
}

inline bool does_move_have_to_charge(
    const MoveInfo* move,
    const Item attacker_item,
    const Weather weather
) {
    bool move_must_charge = false;
    if (move_has_flag(
            move->move,
            MoveFlag::REQUIRES_CHARGING_TURN
        )
    ) {
        move_must_charge = true;
        if ((move->move == Move::SolarBeam &&
                weather == Weather::SUN) ||
            attacker_item == Item::PowerHerb
        ) {
            move_must_charge = false;
        }
    }
    return move_must_charge;
}

inline bool should_skip_move(
    const PokemonState& attacker_state,
    const MoveInfo* move,
    const BestMove& defender_chosen_move
) {
    if (move->move == Move::LeechSeed) {
        return false;
    }

    if (!attacker_state.has_power_points(move->move)
    ) {
        return true;
    }
    const bool defender_is_player = !attacker_state.is_player;
    const auto attackers_last_used_move =
        attacker_state.get_last_used_move();
    const auto category = move->category;
    return category == Category::STATUS ||
        (attacker_state.is_choiced() &&
            attackers_last_used_move.move != nullptr &&
            move->move != attackers_last_used_move.move->move) ||

        move->move == Move::DreamEater ||
        move->move == Move::Bide ||

        (move->move == Move::SuckerPunch &&
            (defender_chosen_move.move == nullptr ||
                defender_chosen_move.move->category == Category::STATUS)) ||

        (move->move == Move::FocusPunch &&
            ((defender_is_player &&
                    defender_chosen_move.move != nullptr &&
                    defender_chosen_move.move->category != Category::STATUS) ||
                !defender_is_player)) ||

        (move->move == Move::Feint &&
            (defender_chosen_move.move == nullptr ||
                (defender_chosen_move.move->move != Move::Protect &&
                    defender_chosen_move.move->move != Move::Detect))
        );
}

inline bool check_fake_out(
    PokemonState& attacker_state,
    const MoveInfo* attacker_move,
    const uint16_t damage,
    const bool is_first_turn
) {
    if (attacker_move->move == Move::FakeOut && is_first_turn && damage != 0) {
        attacker_state.set_chosen_move(
            BestMove{
                .move = attacker_move,
                .damage = damage,
                .potential_damage = damage,
                .times_to_hit = 1
            }
        );
        return true;
    }
    return false;
}

inline bool check_rest(
    PokemonState& attacker_state,
    const PokemonState& defender_state,
    const MoveInfo* attacker_move,
    const uint16_t damage_from_defender,
    const Weather weather
) {
    if (damage_from_defender == 0) {
        return false;
    }

    const bool attacker_faster = attacker_state.outspeeds(
        defender_state,
        defender_state.get_chosen_move().move,
        attacker_move,
        weather
    );
    const auto hp = attacker_state.get_health();
    const int turns_to_ko = hp / damage_from_defender;
    int turns_asleep = 2;
    if (!attacker_faster) {
        turns_asleep++;
    }
    if (const int turns_to_ko_asleep =
            attacker_state.max_health / damage_from_defender;
        turns_to_ko_asleep < turns_asleep &&
        (turns_to_ko > 1 || attacker_faster) &&
        turns_to_ko_asleep > turns_to_ko
    ) {
        attacker_state.set_chosen_move(
            BestMove{
                .move = attacker_move,
                .damage = 0,
                .potential_damage = 0,
                .times_to_hit = 1
            }
        );
        return true;
    }
    return false;
}


inline bool does_zero_damage(
    const PokemonState& defender_state,
    const Move attacker_move,
    const bool attacker_faster
) {
    if (const auto defender_field_position =
            defender_state.get_field_location();
        defender_field_position != FieldLocation::ON_FIELD &&
        attacker_faster
    ) {
        if ((defender_field_position == FieldLocation::IN_AIR &&
                !move_has_flag(
                    attacker_move,
                    MoveFlag::HITS_DEFENDER_IN_AIR)
            ) ||
            (defender_field_position == FieldLocation::UNDER_GROUND &&
                !move_has_flag(
                    attacker_move,
                    MoveFlag::HITS_DEFENDER_UNDER_GROUND)
            ) ||
            (defender_field_position == FieldLocation::UNDER_WATER &&
                !move_has_flag(
                    attacker_move,
                    MoveFlag::HITS_DEFENDER_UNDER_WATER)
            ) ||
            (defender_field_position == FieldLocation::IN_THE_VOID)
        ) {
            return true;
        }
    }

    const auto defender_ability = defender_state.get_ability();
    return (defender_ability == Ability::Damp &&
            (attacker_move == Move::Selfdestruct ||
                attacker_move == Move::Explosion)) ||
        (defender_ability == Ability::Soundproof &&
            move_has_flag(attacker_move, MoveFlag::IS_SOUND_BASED));
}

inline int check_for_fixed_damage(
    const PokemonState& defender_state,
    const int16_t attacker_health,
    const Move attacker_move,
    const uint16_t attacker_level,
    const bool attacker_hit
) {
    if (attacker_move == Move::Endeavor) {
        return std::max(
            0,
            defender_state.get_health() - attacker_health
        );
    }
    if (attacker_move == Move::SuperFang) {
        return std::max(
            1,
            static_cast<int>(std::floor(defender_state.get_health() / 2))
        );
    }
    if (attacker_move == Move::SeismicToss ||
        attacker_move == Move::NightShade
    ) {
        return attacker_level;
    }
    if (attacker_move == Move::DragonRage) {
        return 40;
    }
    if (attacker_move == Move::Psywave) {
        const uint8_t r = defender_state.is_player ? 0 : 10;
        return std::floor((r + 5) * attacker_level / 10);
    }
    const auto defender_chosen_move = defender_state.get_chosen_move();
    const auto defender_chosen_move_info = defender_chosen_move.move;
    if (attacker_move == Move::Counter) {
        if (defender_chosen_move_info != nullptr &&
            defender_chosen_move_info->category == Category::PHYSICAL &&
            defender_state.get_subs_health() == 0
        ) {
            return defender_chosen_move.damage * 2;
        }
        return 0;
    }
    if (attacker_move == Move::MirrorCoat) {
        if (defender_chosen_move_info != nullptr &&
            defender_chosen_move_info->category == Category::SPECIAL &&
            defender_state.get_subs_health() == 0
        ) {
            return defender_chosen_move.damage * 2;
        }
        return 0;
    }
    if (attacker_move == Move::MetalBurst &&
        defender_state.get_subs_health() == 0
    ) {
        if (attacker_hit && defender_chosen_move.damage > 0) {
            return defender_chosen_move.damage * 2;
        }
        return 0;
    }
    return -1;
}

inline PokemonType get_move_type(
    const Item attacker_item,
    const MoveInfo* attacker_move_info,
    const Ability attacker_ability,
    const Weather weather
) {
    const auto attacker_move = attacker_move_info->move;
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
    } else if (attacker_move_info->move == Move::NaturalGift &&
        BERRIES[static_cast<int>(attacker_item)]
    ) {
        move_type = NATURAL_GIFT_POWER[static_cast<int>(attacker_item)].second;
    } else if (attacker_move_info->move == Move::Judgment) {
        move_type = PLATE_ITEM_TYPES[static_cast<int>(attacker_item)];
    }
    if (attacker_ability == Ability::Normalize) {
        move_type = PokemonType::NORMAL;
    }
    return move_type;
}

inline bool type_nullified_by_ability(
    const Ability defender_ability,
    const PokemonType move_type
) {
    return (defender_ability == Ability::FlashFire &&
            move_type == PokemonType::FIRE) ||
        (defender_ability == Ability::MotorDrive &&
            move_type == PokemonType::ELECTRIC) ||
        (defender_ability == Ability::Levitate &&
            move_type == PokemonType::GROUND) ||
        (defender_ability == Ability::VoltAbsorb &&
            move_type == PokemonType::ELECTRIC) ||
        (defender_ability == Ability::WaterAbsorb &&
            move_type == PokemonType::WATER);
}

inline int16_t check_for_zero_or_fixed_damage(
    const PokemonState& attacker_state,
    const MoveInfo* attacker_move_info,
    const PokemonType move_type,
    const PokemonState& defender_state,
    const bool attacker_faster
) {
    auto const attacker_move = attacker_move_info->move;
    if (does_zero_damage(defender_state, attacker_move, attacker_faster)) {
        return 0;
    }

    const auto attacker_health = attacker_state.get_health();
    if (type_nullified_by_ability(defender_state.get_ability(), move_type)) {
        return 0;
    }

    if (attacker_state.is_player && attacker_move == Move::Present) {
        return 0;
    }

    if (attacker_move == Move::Sing ||
        attacker_move == Move::Toxic ||
        attacker_move == Move::Metronome ||
        attacker_move == Move::Refresh
    ) {
        return 0;
    }

    const int damage = check_for_fixed_damage(
        defender_state,
        attacker_health,
        attacker_move,
        attacker_state.level,
        attacker_state.was_hit()
    );
    return static_cast<int16_t>(damage);
}

inline int16_t get_rollout_power(
    const PokemonState& attacker_state,
    const MoveInfo* attacker_move_info
) {
    assert(attacker_move_info->power != -1);
    auto power = static_cast<int16_t>(attacker_move_info->power);
    if (attacker_state.get_rollout_power() != 0) {
        power = static_cast<int16_t>(attacker_state.get_rollout_power());
    } else if (attacker_state.get_last_used_move().move != nullptr &&
        attacker_state.get_last_used_move().move->move ==
        Move::DefenseCurl
    ) {
        power *= 2;
    }
    return power;
}

inline int16_t get_power_based_on_move(
    const PokemonState& attacker_state,
    const MoveInfo* attacker_move_info,
    const PokemonState& defender_state,
    const Weather weather
) {
    const auto attacker_move = attacker_move_info->move;
    const auto attacker_health = attacker_state.get_health();
    int16_t power = attacker_move_info->power;
    const auto status = attacker_state.get_status();
    const auto attacker_item = attacker_state.get_item_for_effect();
    const int attacker_max_health = attacker_state.max_health;
    if (attacker_move == Move::Rollout) {
        power = get_rollout_power(attacker_state, attacker_move_info);
    } else if (attacker_move == Move::Eruption ||
        attacker_move == Move::WaterSpout
    ) {
        power = std::max(
            static_cast<int16_t>(1),
            static_cast<int16_t>(
                floor(150.0 * attacker_health / attacker_max_health)
            )
        );
    } else if ((attacker_move == Move::Facade &&
            (status == Status::POISON ||
                status == Status::PARALYZED ||
                status == Status::BURN)) ||
        (attacker_state.was_hit() &&
            (attacker_move == Move::Avalanche ||
                attacker_move == Move::Revenge ||
                attacker_move == Move::Payback)) ||
        (attacker_move == Move::WeatherBall &&
            weather != Weather::CLEAR
        ) ||
        (attacker_move == Move::Brine &&
            attacker_health <= attacker_max_health / 2)
    ) {
        power = static_cast<int16_t>(power * 2);
    } else if (attacker_move == Move::HiddenPower) {
        if (defender_state.has_type(PokemonType::GHOST) ||
            defender_state.has_type(PokemonType::GROUND) ||
            defender_state.has_type(PokemonType::STEEL) ||
            defender_state.has_type(PokemonType::FLYING) ||
            defender_state.has_type(PokemonType::DARK) ||
            defender_state.has_type(PokemonType::NORMAL)
        ) {
            power = 0;
        } else if (attacker_state.is_player) {
            power = 30;
        } else {
            power = 70;
        }
    } else if (attacker_move == Move::LowKick ||
        attacker_move == Move::GrassKnot
    ) {
        if (const double defender_weight = defender_state.get_pounds();
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
        if (attacker_item == Item::None) {
            power = 0;
        } else if (attacker_item == Item::ChoiceBand ||
            attacker_item == Item::QuickPowder ||
            attacker_item == Item::MuscleBand
        ) {
            power = 10;
        } else if (attacker_item == Item::StickyBarb) {
            power = 80;
        } else if (attacker_item == Item::IronBall) {
            power = 130;
        } else if (attacker_item == Item::SpellTag ||
            attacker_item == Item::RazorFang
        ) {
            power = 30;
        } else {
            throw std::runtime_error{
                "No fling: " + ITEM_TO_STRING.at(attacker_item)
            };
        }
    } else if (attacker_move == Move::WringOut ||
        attacker_move == Move::CrushGrip
    ) {
        power = static_cast<int16_t>(
            1 + std::floor(
                120 * defender_state.get_health() / defender_state.max_health
            )
        );
    } else if (attacker_move == Move::Reversal ||
        attacker_move == Move::Flail
    ) {
        if (const int ratio = 64 * attacker_health / attacker_max_health;
            ratio >= 43
        ) {
            power = 20;
        } else if (ratio >= 22) {
            power = 40;
        } else if (ratio >= 13) {
            power = 80;
        } else if (ratio >= 6) {
            power = 100;
        } else if (ratio >= 2) {
            power = 150;
        } else {
            power = 200;
        }
    } else if (attacker_move == Move::GyroBall) {
        power = std::min(
            150,
            static_cast<int32_t>(
                std::floor(
                    25 * defender_state.get_speed(weather) /
                    attacker_state.get_speed(weather)
                ) + 1
            )
        );
    } else if (attacker_move == Move::Punishment) {
        power = std::min(
            200,
            static_cast<int32_t>(
                std::floor(
                    60 + 20 * (
                        defender_state.get_attack_stage() +
                        defender_state.get_defense_stage() +
                        defender_state.get_special_attack_stage() +
                        defender_state.get_special_defense_stage() +
                        defender_state.get_speed_stage()
                    )
                )
            )
        );
    } else if (attacker_move == Move::NaturalGift
    ) {
        power = NATURAL_GIFT_POWER[static_cast<int>(attacker_item)].first;
    } else if (attacker_move == Move::TrumpCard) {
        const auto pp_left =
            attacker_state.get_power_points_left(attacker_move);
        if (pp_left >= 4) {
            power = 40;
        } else if (pp_left == 3) {
            power = 50;
        } else if (pp_left == 2) {
            power = 60;
        } else if (pp_left == 1) {
            power = 80;
        } else {
            power = 200;
        }
    } else if (attacker_move == Move::SpitUp) {
        power = 100;
    } else if (attacker_move == Move::WakeUpSlap) {
        power = 60;
        if (defender_state.get_status() == Status::SLEEP) {
            power *= 2;
        }
    } else if (attacker_move == Move::Magnitude) {
        if (attacker_state.is_player) {
            power = 10;
        } else {
            power = 150;
        }
    }

    // Reckless
    if (attacker_state.get_ability() == Ability::Reckless &&
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

    if (!attacker_state.is_player && attacker_move == Move::Present) {
        return 120;
    }
    return power;
}

inline int16_t apply_ability_power_modifiers(
    const PokemonState& attacker_state,
    const Move attacker_move,
    const PokemonType move_type,
    int16_t power,
    const Ability defender_ability
) {
    const auto attacker_health = attacker_state.get_health();
    const auto attacker_max_health = attacker_state.max_health;
    if (const Ability attacker_ability = attacker_state.get_ability();
        (attacker_ability == Ability::Technician &&
            power <= 50 && attacker_move != Move::Struggle) ||
        (attacker_ability == Ability::Blaze &&
            attacker_health <= attacker_max_health / 3 &&
            move_type == PokemonType::FIRE) ||
        (attacker_ability == Ability::Overgrow &&
            attacker_health <= attacker_max_health / 3 &&
            move_type == PokemonType::GRASS) ||
        (attacker_ability == Ability::Swarm &&
            attacker_health <= attacker_max_health / 3 &&
            move_type == PokemonType::BUG) ||
        (attacker_ability == Ability::Torrent &&
            attacker_health <= attacker_max_health / 3 &&
            move_type == PokemonType::WATER)
    ) {
        power = std::floor(power * 1.5);
    } else if (attacker_ability == Ability::Rivalry) {
        if (attacker_state.is_player) {
            power = std::floor(power * 0.75);
        } else {
            power = std::floor(power * 1.25);
        }
    } else if (attacker_ability == Ability::IronFist &&
        move_has_flag(attacker_move, MoveFlag::POWERS_IRON_FIST)
    ) {
        power = std::floor(power * 1.2);
    }

    if (defender_ability == Ability::Heatproof &&
        move_type == PokemonType::FIRE
    ) {
        power = static_cast<int16_t>(std::floor(power / 2));
    }
    return power;
}

inline int16_t apply_item_power_modifiers(
    const PokemonState& attacker_state,
    const MoveInfo* attacker_move_info,
    const PokemonType move_type,
    int16_t power
) {
    if (const auto attacker_item = attacker_state.get_item_for_effect();
        attacker_item == Item::Metronome
    ) {
        const double multiplier = 1.0 + std::max(
            0.0,
            0.1 * attacker_state.get_metronome_count()
        );
        power = std::floor(power * multiplier);
    } else if ((attacker_item == Item::MuscleBand &&
            attacker_move_info->category == Category::PHYSICAL) ||
        (attacker_item == Item::WiseGlasses &&
            attacker_move_info->category == Category::SPECIAL)
    ) {
        power = std::floor(power * 1.1);
    } else if (ITEM_TO_TYPE[static_cast<int>(attacker_item)][
            static_cast<int>(move_type)
        ] ||
        (PLATE_ITEMS[static_cast<int>(attacker_item)] &&
            PLATE_ITEM_TYPES[static_cast<int>(attacker_item)] == move_type)
    ) {
        power = std::floor(power * 1.2);
    } else if (attacker_item == Item::LightBall &&
        attacker_state.pokemon->name == Pokemon::Pikachu
    ) {
        power = static_cast<int16_t>(power * 2);
    }
    return power;
}

inline int16_t apply_field_location_power_modifiers(
    const PokemonState& attacker_state,
    const MoveInfo* attacker_move_info,
    int16_t power,
    const PokemonState& defender_state,
    const Weather weather,
    const bool is_mid_turn
) {
    auto const attacker_move = attacker_move_info->move;
    auto const attacker_ability = attacker_state.get_ability();
    const bool attacker_faster = attacker_state.outspeeds(
        defender_state,
        nullptr,
        attacker_move_info,
        weather
    );
    if (const auto& defender_field_location =
            defender_state.get_field_location();
        defender_field_location != FieldLocation::ON_FIELD
    ) {
        if (!attacker_faster && !is_mid_turn) {
            return power;
        }
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
    return power;
}

inline uint16_t get_move_power(
    const PokemonState& attacker_state,
    const MoveInfo* attacker_move_info,
    const PokemonType move_type,
    const PokemonState& defender_state,
    const Weather weather,
    const bool is_mid_turn
) {
    int16_t power = get_power_based_on_move(
        attacker_state,
        attacker_move_info,
        defender_state,
        weather
    );
    power = apply_item_power_modifiers(
        attacker_state,
        attacker_move_info,
        move_type,
        power
    );
    const auto attacker_move = attacker_move_info->move;
    const auto defender_ability = defender_state.get_ability();
    power = apply_ability_power_modifiers(
        attacker_state,
        attacker_move,
        move_type,
        power,
        defender_ability
    );
    power = apply_field_location_power_modifiers(
        attacker_state,
        attacker_move_info,
        power,
        defender_state,
        weather,
        is_mid_turn
    );
    return power;
}

inline int32_t apply_damage_modifiers(
    const PokemonState& attacker_state,
    const MoveInfo* attacker_move_info,
    const PokemonType move_type,
    const PokemonState& defender_state,
    const Weather weather,
    int32_t damage,
    double effectiveness
) {
    const auto attacker_ability = attacker_state.get_ability();
    // STAB
    if (attacker_state.has_type(move_type)) {
        if (attacker_ability == Ability::Adaptability) {
            damage = std::floor(damage * 2);
        } else {
            damage = std::floor(damage * 1.5);
        }
    }

    // Random
    if (attacker_state.is_player && attacker_move_info->move != Move::SpitUp) {
        damage = static_cast<int32_t>(std::floor(damage * 85 / 100));
    }

    // Type effectiveness
    const auto attacker_move = attacker_move_info->move;
    if (attacker_move == Move::HiddenPower) {
        if (attacker_state.is_player) {
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
    }
    const auto defender_ability = defender_state.get_ability();
    if (defender_ability == Ability::SolidRock &&
        effectiveness >= 2.0
    ) {
        effectiveness = std::floor(effectiveness * 0.75);
    } else if (defender_ability == Ability::WonderGuard &&
        effectiveness < 2.0
    ) {
        return 0;
    }
    damage = std::floor(damage * effectiveness);
    if (const auto attacker_item = attacker_state.get_item_for_effect();
        attacker_item == Item::ExpertBelt && effectiveness >= 2
    ) {
        damage = std::floor(damage * 1.2);
    } else if (attacker_item == Item::LifeOrb) {
        damage = std::floor(damage * 1.3);
    }

    // Damage reducers
    if ((attacker_move != Move::BrickBreak &&
            (defender_state.has_reflect_up() &&
                attacker_move_info->category == Category::PHYSICAL) ||
            (defender_state.has_light_screen_up() &&
                attacker_move_info->category == Category::SPECIAL)) ||
        (attacker_state.get_status() == Status::BURN &&
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
        damage = std::floor(damage * 2);
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

    if (attacker_move == Move::Assurance && attacker_state.took_damage()) {
        damage *= 2;
    }

    // Berries that reduce damage
    if (const auto defender_item = defender_state.get_item_for_effect();
        DAMAGE_REDUCING_BERRIES.contains(defender_item) &&
        DAMAGE_REDUCING_BERRIES.at(defender_item) == attacker_move_info->type &&
        defender_state.get_subs_health() == 0
    ) {
        damage /= 2;
    }

    return static_cast<int32_t>(std::floor(damage));
}

inline std::array<int16_t, LEVEL + 1> DAMAGE_CACHE = [] {
    std::array<int16_t, LEVEL + 1> array{};
    array.fill(-1.0);
    return array;
}();

inline uint32_t PokemonState::get_damage_of_attacker_move(
    const Item attacker_item,
    const MoveInfo* attacker_move_info,
    PokemonState& defender_state,
    const Weather weather,
    const bool is_mid_turn
) {
    const auto attacker_ability = get_ability();
    const PokemonType move_type = get_move_type(
        attacker_item,
        attacker_move_info,
        attacker_ability,
        weather
    );

    const bool attacker_faster = outspeeds(
        defender_state,
        defender_state.get_chosen_move().move,
        attacker_move_info,
        weather
    );
    int32_t damage = check_for_zero_or_fixed_damage(
        *this,
        attacker_move_info,
        move_type,
        defender_state,
        attacker_faster
    );
    if (damage != -1) {
        return damage;
    }

    damage = DAMAGE_CACHE[level];
    if (damage < 0) {
        damage = static_cast<int16_t>(std::floor(2 * level / 5) + 2);
        DAMAGE_CACHE[level] = damage;
    }

    const auto defender_ability = defender_state.get_ability();
    const auto category = attacker_move_info->category;
    const bool is_special = category == Category::SPECIAL;
    if (category == Category::STATUS) {
        return 0;
    }
    uint16_t attacker_attack =
        is_special
            ? get_special_attack(weather, defender_ability)
            : get_attack(weather, defender_ability);
    if (defender_ability == Ability::ThickFat &&
        (move_type == PokemonType::ICE ||
            move_type == PokemonType::FIRE)
    ) {
        attacker_attack = attacker_attack / 2;
    }
    const uint16_t defender_defense =
        is_special
            ? defender_state.get_special_defense(weather, defender_ability)
            : defender_state.get_defense(weather, defender_ability);
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

    const uint16_t power = get_move_power(
        *this,
        attacker_move_info,
        move_type,
        defender_state,
        weather,
        is_mid_turn
    );
    if (power == 0) {
        return 0;
    }

    assert(defender_defense > 0);
    damage = std::floor(damage * power * attacker_attack / defender_defense);
    damage = static_cast<int32_t>(std::floor(damage / 50) + 2);

    auto defender_types = defender_state.get_types();
    if (defender_state.is_grounded() &&
        attacker_move_info->type == PokemonType::GROUND
    ) {
        if (defender_types[0] == PokemonType::FLYING) {
            defender_types = {defender_types[1]};
        } else if (defender_types[1] == PokemonType::FLYING) {
            defender_types = {defender_types[0]};
        }
    }

    const auto effectiveness =
        get_effectiveness(defender_types, move_type);

    damage = apply_damage_modifiers(
        *this,
        attacker_move_info,
        move_type,
        defender_state,
        weather,
        damage,
        effectiveness
    );

    if (attacker_move_info->move == Move::SpitUp) {
        damage *= stockpiles;
    }

    if (move_has_flag(attacker_move_info->move, MoveFlag::HAS_POWER) ||
        ((attacker_move_info->move == Move::HiddenPower ||
                attacker_move_info->move == Move::WringOut ||
                attacker_move_info->move == Move::Flail ||
                attacker_move_info->move == Move::Eruption ||
                attacker_move_info->move == Move::WaterSpout ||
                attacker_move_info->move == Move::GrassKnot ||
                attacker_move_info->move == Move::Avalanche ||
                attacker_move_info->move == Move::Uproar ||
                attacker_move_info->move == Move::KnockOff) &&
            effectiveness > 0)
    ) {
        damage = std::max(1, damage);
    }

    if (attacker_move_info->move == Move::FalseSwipe) {
        damage = std::min(
            defender_state.get_health() - 1,
            damage
        );
    }

    return damage;
}

#endif //POKEMONSTATE_H
