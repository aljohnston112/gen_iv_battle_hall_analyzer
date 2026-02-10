#pragma once

#include "../../external/simdjson.h"
#include "../config.h"

struct BaseStats {
    int health;
    int attack;
    int defense;
    int special_attack;
    int special_defense;
    int speed;
};

struct BattleFactoryPokemon {
    std::string name;
    std::string unique_id;
    Nature nature;
    std::array<PokemonType, 2> types;
    std::string item;
    std::vector<const MoveInfo*> moves;
    int set_number;
    std::unordered_map<Stat, int16_t> effort_values;
};

inline std::unordered_map<std::string, BaseStats>
parse_base_stats_file(const char* path);

inline std::unordered_map<std::string, BattleFactoryPokemon>
parse_battle_factory_file(const char* path);

inline std::vector<CustomPokemon> get_custom_batle_factory_pokemon();
