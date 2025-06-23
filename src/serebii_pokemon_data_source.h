#ifndef SEREBII_POKEMON_DATA_SOURCE_H
#define SEREBII_POKEMON_DATA_SOURCE_H

#include <string>
#include <vector>
#include <unordered_map>

enum class PokemonType {
    NORMAL,
    FIGHTING,
    FLYING,
    POISON,
    GROUND,
    ROCK,
    BUG,
    GHOST,
    STEEL,
    FIRE,
    WATER,
    GRASS,
    ELECTRIC,
    PSYCHIC,
    ICE,
    DRAGON,
    DARK
};

extern std::unordered_map<std::string, PokemonType> pokemon_type_map;

enum class Category {
    PHYSICAL,
    SPECIAL,
    STATUS
};

extern std::unordered_map<std::string, Category> move_category_map;

struct Attack {
    std::string name;
    PokemonType move_type;
    Category category;
    int power;
    int accuracy;
    int effect_percent;
};

enum class Stat {
    HEALTH,
    ATTACK,
    DEFENSE,
    SPECIAL_ATTACK,
    SPECIAL_DEFENSE,
    SPEED
};

extern std::unordered_map<std::string, Stat> stat_map;

struct SerebiiPokemon {
    std::string name;
    std::vector<PokemonType> types;
    int id = -1;
    std::string ability;
    double pounds = 0;
    std::unordered_map<Stat, int> base_stats;
    std::unordered_map<int, std::vector<Attack>> level_attacks;
    std::unordered_map<std::string, Attack> tm_or_hm_attacks;
    std::vector<Attack> egg_moves;
    std::unordered_map<int, std::unordered_map<int, std::vector<Attack>>> pre_evo_moves;
    std::vector<Attack> move_tutor_attacks;
    std::unordered_map<std::string, std::unordered_map<int, std::vector<Attack>>> game_to_level_to_moves;
    std::vector<Attack> special_moves;
    std::unordered_map<std::string, std::unordered_map<Stat, int>> form_to_base_stats;
    std::unordered_map<std::string, std::unordered_map<int, std::vector<Attack>>> form_to_level_up_attacks;
    std::unordered_map<std::string, std::unordered_map<std::string, Attack>> form_to_tm_or_hm_to_attack;
    std::unordered_map<std::string, std::vector<Attack>> form_to_move_tutor_attacks;
};

std::unordered_map<std::string, SerebiiPokemon> get_serebii_pokemon();

void print_serebii_pokemon();


#endif //SEREBII_POKEMON_DATA_SOURCE_H
