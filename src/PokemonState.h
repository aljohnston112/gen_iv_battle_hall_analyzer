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
    std::array<PokemonType, 2> types;
    Ability ability;
    double pounds;
    Item item;
    std::array<int16_t, 6> current_stats;
    std::array<int8_t, 6> stat_stages = {0, 0, 0, 0, 0, 0};
    FieldLocation field_location = FieldLocation::ON_FIELD;

    Status status = Status::NONE;
    int turns_badly_poisoned = 0;
    bool confused = false;
    bool infatuated = false;

    int trapped_counter = 0;
    bool flinched = false;
    bool was_hit_ = false;

    bool charging = false;
    bool recharging = false;
    uint8_t multi_turn_move_counter = 0;

    bool grounded;
    int metronome = 0;
    bool is_choiced_ = false;

    bool protected_ = false;
    bool reflect = false;
    bool light_screen = false;

    bool flash_fired = false;
    uint slow_start_count = 0;
    bool truant = false;
    bool unburdened = false;

    BestMove last_used_move{};
    BestMove chosen_move{};

    [[nodiscard]] Item get_item() const {
        return item;
    }

    void apply_berry() {
        bool eaten = false;
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
                std::min(max_health, current_health + max_health / 4);
            eaten = true;
        }
        if (!eaten) {
            throw std::runtime_error("Unimplemented berry");
        }
        clear_item();
    }

    void try_apply_berry(const bool eaten) {
        const auto item = get_item_for_effect();
        if (const bool is_berry = BERRIES.contains(item);
            is_berry
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
                (item == Item::PersimBerry && confused) ||
                (item == Item::LumBerry &&
                    (status != Status::NONE || confused)
                );
            const bool eats_sitrus =
                item == Item::SitrusBerry && less_than_half_health;
            const bool eats_berry =
                eaten ||
                eats_stat_berry ||
                eats_status_berry ||
                eats_sitrus;
            if (eats_berry) {
                apply_berry();
            }
        }
    }

    int16_t get_stat(
        const Weather weather,
        const Ability other_ability,
        const Stat stat_enum
    ) {
        const int8_t stat_index = static_cast<int8_t>(stat_enum);
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
    const CustomPokemon pokemon;
    const uint8_t level;
    const int max_health;

    PokemonState(CustomPokemon pokemon, const bool is_player):
        moves(pokemon.moves),
        types(pokemon.types),
        ability(pokemon.ability),
        pounds(pokemon.pounds),
        item(pokemon.item),
        current_stats(
            {
                static_cast<int16_t>(pokemon.stats[HEALTH_INDEX]),
                static_cast<int16_t>(pokemon.stats[ATTACK_INDEX]),
                static_cast<int16_t>(pokemon.stats[DEFENSE_INDEX]),
                static_cast<int16_t>(pokemon.stats[SPECIAL_ATTACK_INDEX]),
                static_cast<int16_t>(pokemon.stats[SPECIAL_DEFENSE_INDEX]),
                static_cast<int16_t>(pokemon.stats[SPEED_INDEX])
            }
        ),
        grounded(item == Item::IronBall),
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

    [[nodiscard]] int16_t get_health() const {
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

    [[nodiscard]] int16_t get_attack(
        const Weather weather,
        const Ability other_ability
    ) {
        int16_t stat = get_stat(weather, other_ability, Stat::ATTACK);
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

    [[nodiscard]] int16_t get_defense(
        const Weather weather,
        const Ability other_ability
    ) {
        int16_t stat = get_stat(weather, other_ability, Stat::DEFENSE);
        if (get_ability() == Ability::MarvelScale &&
            status != Status::NONE
        ) {
            stat = std::floor(stat * 1.5);
        }
        return stat;
    }

    [[nodiscard]] int16_t get_special_attack(
        const Weather weather,
        const Ability other_ability
    ) {
        int16_t stat = get_stat(weather, other_ability, Stat::SPECIAL_ATTACK);
        if (get_ability() == Ability::SolarPower &&
            weather == Weather::SUN
        ) {
            stat = std::floor(stat * 1.5);
        }
        return stat;
    }

    [[nodiscard]] int16_t get_special_defense(
        const Weather weather,
        const Ability other_ability
    ) {
        int16_t stat = get_stat(weather, other_ability, Stat::SPECIAL_DEFENSE);
        if (get_ability() == Ability::FlowerGift &&
            weather == Weather::SUN
        ) {
            stat = std::floor(stat * 1.5);
        }
        return stat;
    }

    [[nodiscard]] int16_t get_speed(const Weather weather) const {
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

    int8_t get_attack_stage() const {
        return stat_stages.at(ATTACK_INDEX);
    }

    int8_t get_defense_stage() const {
        return stat_stages.at(DEFENSE_INDEX);
    }

    int8_t get_special_attack_stage() const {
        return stat_stages.at(SPECIAL_ATTACK_INDEX);
    }

    int8_t get_special_defense_stage() const {
        return stat_stages.at(SPECIAL_DEFENSE_INDEX);
    }

    int8_t get_speed_stage() const {
        return stat_stages.at(SPEED_INDEX);
    }

    void change_stat_stage(
        const Stat stat,
        int8_t stage_change,
        const bool from_other
    ) {
        assert(stage_change != 0 && stage_change >= -6 && stage_change <= 6);
        if (get_ability() == Ability::Simple) {
            stage_change = stage_change * 2;
        }
        const int8_t index = static_cast<int8_t>(stat);
        const auto stage = stat_stages[index];
        const int8_t new_stage = stage + stage_change;
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
            !(weather == Weather::SUN && ability == Ability::LeafGuard)
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

    void set_trapped_counter(const int turns) {
        trapped_counter = turns;
    }

    [[nodiscard]] bool is_flinched() const {
        return flinched;
    }

    void set_flinched() {
        flinched = true;
    }

    [[nodiscard]] bool was_hit() const {
        return was_hit_;
    }

    void set_was_hit() {
        was_hit_ = true;
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

    [[nodiscard]] BestMove get_last_used_move() const {
        return last_used_move;
    }

    [[nodiscard]] BestMove get_chosen_move() const {
        return chosen_move;
    }

    void set_chosen_move(BestMove&& best_move) {
        this->chosen_move = std::move(best_move);
    }

    void update_last_used_move(const bool move_failed) {
        if (!move_failed) {
            set_chosen_move(
                BestMove{
                    .move = chosen_move.move,
                    .damage = 0,
                    .times_to_hit = 1
                }
            );
        }
        last_used_move = chosen_move;
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
        this->is_choiced_ = false;
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
                &new_move,
                defender_state,
                weather,
                is_mid_turn
            );
        return damage;
    }

    uint get_damage_of_attacker_move(
        const MoveInfo* attacker_move_info,
        PokemonState& defender_state,
        Weather weather,
        bool is_mid_turn
    );

    void apply_end_of_turn_effects(
        Weather weather,
        PokemonState& defender_state
    );
};

static bool check_transform(PokemonState& attacker_state) {
    if (const auto& all_attacker_moves =
            attacker_state.get_moves();
        all_attacker_moves.size() == 1 &&
        all_attacker_moves[0]->move == Move::Transform
    ) {
        attacker_state.set_chosen_move(
            BestMove{
                .move = all_attacker_moves[0],
                .damage = 0,
                .times_to_hit = 1
            }
        );
        return true;
    }
    return false;
}

struct BattleStats {
    int32_t attack;
    int32_t special_attack;
    int32_t defense;
    int32_t special_defense;
};

inline BattleStats get_battle_stats(
    PokemonState& attacker_state,
    PokemonState& defender_state,
    const Weather weather
) {
    // Attack stats
    const auto defender_ability = defender_state.get_ability();
    auto attack =
        attacker_state.get_attack(weather, defender_ability);
    auto special_attack =
        attacker_state.get_special_attack(weather, defender_ability);
    if (const auto attacker_item = attacker_state.get_item_for_effect();
        attacker_item == Item::ChoiceBand
    ) {
        attack = std::floor(attack * 1.5);
    } else if (attacker_item == Item::ChoiceSpecs) {
        special_attack = std::floor(special_attack * 1.5);
    }

    // Defense stats
    const auto attacker_ability = attacker_state.get_ability();
    const auto defense =
        defender_state.get_defense(weather, attacker_ability);
    auto special_defense =
        defender_state.get_special_defense(weather, attacker_ability);

    if (weather == Weather::SANDSTORM &&
        defender_state.has_type(PokemonType::ROCK)
    ) {
        special_defense = std::floor(special_defense * 1.5);
    }
    return BattleStats{
        .attack = attack,
        .special_attack = special_attack,
        .defense = defense,
        .special_defense = special_defense
    };
}

inline void recalculate_chosen_move_damage(
    PokemonState& attacker_state,
    PokemonState& defender_state,
    const BestMove& attacker_chosen_move,
    const Weather weather,
    const bool is_mid_turn
) {
    attacker_state.set_chosen_move(
        BestMove{
            .move = attacker_chosen_move.move,
            .damage = attacker_state.get_damage_of_attacker_move(
                attacker_chosen_move.move,
                defender_state,
                weather,
                is_mid_turn
            ),
            .times_to_hit = attacker_chosen_move.times_to_hit
        }
    );
}

inline uint8_t get_times_to_hit(
    const bool attacker_is_player,
    const MoveInfo* const& move
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
    const MoveInfo* const& move,
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
    const BestMove defender_chosen_move
) {
    const bool defender_is_player = !attacker_state.is_player;
    const auto attackers_last_used_move =
        attacker_state.get_last_used_move();
    const auto category = move->category;
    return category == Category::STATUS ||

        (attacker_state.is_choiced() &&
            attackers_last_used_move.move != nullptr &&
            move->move != attackers_last_used_move.move->move) ||

        move->move == Move::DreamEater ||

        (move->move == Move::FocusPunch &&
            ((defender_is_player &&
                    defender_chosen_move.move != nullptr &&
                    defender_chosen_move.move->category != Category::STATUS) ||
                !defender_is_player)
        );
}

inline bool check_fake_out(
    PokemonState& attacker_state,
    const MoveInfo* attacker_move,
    const uint damage,
    const bool is_first_turn
) {
    if (attacker_move->move == Move::FakeOut && is_first_turn) {
        attacker_state.set_chosen_move(
            BestMove{
                .move = attacker_move,
                .damage = damage,
                .times_to_hit = 1
            }
        );
        return true;
    }
    return false;
}

#endif //POKEMONSTATE_H
