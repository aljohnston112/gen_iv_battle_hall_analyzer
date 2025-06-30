#ifndef POKEMONSTATE_H
#define POKEMONSTATE_H

#include "custom_pokemon.h"
#include "nature.h"

enum class Status {
    BURN,
    POISON,
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

struct PokemonState {
    const bool is_player;
    const CustomPokemon& pokemon;

    const uint8_t level = pokemon.level;
    const std::array<PokemonType, 2> types = pokemon.types;
    Item item = pokemon.item;
    Ability ability = pokemon.ability;
    FieldLocation field_location = FieldLocation::ON_FIELD;

    int max_health = pokemon.stats.at(HEALTH_INDEX);
    std::array<int, 6> current_stats = {
        pokemon.stats.at(HEALTH_INDEX),
        pokemon.stats.at(ATTACK_INDEX),
        pokemon.stats.at(DEFENSE_INDEX),
        pokemon.stats.at(SPECIAL_ATTACK_INDEX),
        pokemon.stats.at(SPECIAL_DEFENSE_INDEX),
        pokemon.stats.at(SPEED_INDEX)
    };
    std::array<int, 6> stat_stages = {0, 0, 0, 0, 0, 0};

    Status status = Status::NONE;
    bool confused = false;
    bool recharging = false;
    int multi_turn_move_counter = 0;
    bool was_hit = false;
    bool charging = false;
};


#endif //POKEMONSTATE_H
