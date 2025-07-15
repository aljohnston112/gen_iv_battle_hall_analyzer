#ifndef POKEMONSTATE_H
#define POKEMONSTATE_H

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

int calculate_stat_based_on_stage(
    uint16_t stat,
    int stage
);

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

public:
    const bool is_player;
    const CustomPokemon pokemon;
    const uint8_t level;
    const int max_health;

    PokemonState(CustomPokemon pokemon, bool is_player);

    [[nodiscard]] bool has_type(PokemonType type) const;

    void change_type(PokemonType type);

    [[nodiscard]] std::vector<const MoveInfo*>& get_moves();

    void transform(const PokemonState& other);

    [[nodiscard]] Ability get_ability() const;

    void set_ability(Ability ability, bool can_overwrite_truant);

    void clear_ability();

    [[nodiscard]] Item get_item_for_effect() const;

    [[nodiscard]] Item get_item() const;

    bool try_set_item(Item item);

    void clear_item();

    void apply_berry();

    void try_apply_berry(bool eaten);

    void eat_berry(Item item);

    [[nodiscard]] int32_t get_health() const;

    void apply_damage(uint damage);

    void heal(int health_gained);

    [[nodiscard]] int32_t get_attack(Weather weather) const;

    [[nodiscard]] int32_t get_defense() const;

    [[nodiscard]] int32_t get_special_attack(Weather weather) const;

    [[nodiscard]] int32_t get_special_defense(Weather weather) const;

    [[nodiscard]] int32_t get_speed(Weather weather) const;

    void change_stat_modifier(
        Stat stat,
        int change,
        bool from_other
    );

    void clear_negative_stat_changes();

    [[nodiscard]] double get_weight() const;

    [[nodiscard]] FieldLocation& get_field_location();

    [[nodiscard]] Status get_status() const;

    void try_apply_status(
        Status status,
        Weather weather,
        PokemonState& other_state
    );

    void clear_status();

    [[nodiscard]] bool is_confused() const;

    void set_confused();

    [[nodiscard]] bool is_flinched() const;

    void set_flinched();

    void set_infatuated();

    void set_was_hit();

    [[nodiscard]] bool was_hit() const;

    [[nodiscard]] bool was_flash_fired() const;

    void set_flash_fire();

    [[nodiscard]] bool is_charging() const;

    void start_charging();

    void done_charging();

    void used_move_that_requires_recharge();

    [[nodiscard]] bool is_recharging() const;

    void done_recharging();

    void increment_multi_turn_move_counter(uint8_t max_turns);

    [[nodiscard]] int get_metronome_count() const;

    [[nodiscard]] bool is_protected() const;

    void break_protect();

    [[nodiscard]] bool has_reflect_up() const;

    void break_reflect();

    [[nodiscard]] bool has_light_screen_up() const;

    void break_light_screen();

    void apply_end_of_turn_effects(
        Weather weather,
        PokemonState& defender_state
    );

    [[nodiscard]] bool outspeeds(
        const PokemonState& other_state,
        const MoveInfo* other_move,
        const MoveInfo* this_move,
        Weather weather
    ) const ;

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
        int32_t attack,
        int32_t special_attack,
        int32_t defense,
        int32_t special_defense,
        Weather weather,
        bool is_mid_turn
    );

    BestMove get_best_move_against_defender(
        PokemonState& defender_state,
        bool chosen_move_only,
        Weather weather,
        bool is_mid_turn
    );

    [[nodiscard]] BestMove& get_chosen_move();

    [[nodiscard]] BestMove& get_last_used_move();

    void increase_metronome();

    void clear_metronome();

    void set_trapped_counter(int turns);

    [[nodiscard]] bool is_first_turn() const;

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
