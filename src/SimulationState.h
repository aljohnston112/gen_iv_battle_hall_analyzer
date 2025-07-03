#ifndef POKEMONSTATE_H
#define POKEMONSTATE_H

#include "custom_pokemon.h"
#include "nature.h"

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

constexpr int HEALTH_INDEX = static_cast<int>(Stat::HEALTH);
constexpr int ATTACK_INDEX = static_cast<int>(Stat::ATTACK);
constexpr int DEFENSE_INDEX = static_cast<int>(Stat::DEFENSE);
constexpr int SPECIAL_ATTACK_INDEX = static_cast<int>(Stat::SPECIAL_ATTACK);
constexpr int SPECIAL_DEFENSE_INDEX = static_cast<int>(Stat::SPECIAL_DEFENSE);
constexpr int SPEED_INDEX = static_cast<int>(Stat::SPEED);
constexpr int MAX_STAT_STAGE = 6;
constexpr int MIN_STAT_STAGE = -6;

int calculate_stat_based_on_stage(const uint16_t stat, const int stage) {
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
    Item item = pokemon.item;

    Ability ability = pokemon.ability;
    FieldLocation field_location = FieldLocation::ON_FIELD;

    std::array<uint16_t, 6> current_stats = {
        pokemon.stats.at(HEALTH_INDEX),
        pokemon.stats.at(ATTACK_INDEX),
        pokemon.stats.at(DEFENSE_INDEX),
        pokemon.stats.at(SPECIAL_ATTACK_INDEX),
        pokemon.stats.at(SPECIAL_DEFENSE_INDEX),
        pokemon.stats.at(SPEED_INDEX)
    };
    std::array<int8_t, 6> stat_stages = {0, 0, 0, 0, 0, 0};

    Status status = Status::NONE;
    int turns_badly_poisoned = 0;
    bool confused = false;
    bool recharging = false;
    int multi_turn_move_counter = 0;
    bool was_hit = false;
    bool charging = false;
    int trapped_counter = 0;
    bool reflect = false;
    bool light_screen = false;
    bool is_protected = false;
    bool is_choiced = false;
    bool grounded = item == Item::IronBall;
    int metronome = 0;
    bool flinched = false;
    bool infatuated = false;

public:
    const bool is_player;
    const CustomPokemon& pokemon;
    const uint8_t level = pokemon.level;
    const std::array<PokemonType, 2> types = pokemon.types;
    const int max_health = pokemon.stats.at(HEALTH_INDEX);

    Ability get_ability() const {
        return ability;
    }

    Status get_status() const {
        return status;
    }

    bool is_confused() const {
        return confused;
    }

    Item get_item() const {
        return item;
    }

    uint16_t get_health() const {
        return current_stats[HEALTH_INDEX];
    }

    uint16_t get_attack() const {
        return current_stats[ATTACK_INDEX];
    }

    uint16_t get_defense() const {
        return current_stats[DEFENSE_INDEX];
    }

    uint16_t get_special_attack() const {
        return current_stats[SPECIAL_ATTACK_INDEX];
    }

    uint16_t get_special_defense() const {
        return current_stats[SPECIAL_DEFENSE_INDEX];
    }

    uint16_t get_speed() const {
        return current_stats[SPEED_INDEX];
    }

    void change_stat_modifier(
        const Stat stat,
        const int change
    ) {
        assert(change != 0);
        const int index = static_cast<int>(stat);
        const auto stage = stat_stages[index];
        const int new_stage = stage + change;
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

    void apply_damage(const int damage) {
        current_stats[HEALTH_INDEX] -= damage;
        try_apply_berry( false);
    }

    void try_apply_status(const Status status) {
        if (status == Status::NONE) {
            this->status = status;
            try_apply_berry( false);
        }
    }

    void heal(const int health_gained) {
        current_stats[HEALTH_INDEX] += health_gained;
        if (current_stats[HEALTH_INDEX] > max_health) {
            current_stats[HEALTH_INDEX] = max_health;
        }
    }

};

enum class Weather {
    CLEAR,
    SUN,
    RAIN,
    HAIL,
    SANDSTORM,
    FOG
};

struct BattleState {
    Weather weather = Weather::CLEAR;
    bool mid_turn = false;
    uint weather_turns = 0;
};


#endif //POKEMONSTATE_H
