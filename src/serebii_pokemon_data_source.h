#ifndef SEREBII_POKEMON_DATA_SOURCE_H
#define SEREBII_POKEMON_DATA_SOURCE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "moves.h"

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
    DARK,
    COUNT
};

extern std::unordered_map<std::string, PokemonType> pokemon_type_map;

enum class Category {
    PHYSICAL,
    SPECIAL,
    STATUS
};

extern std::unordered_map<std::string, Category> move_category_map;

struct MoveInfo {
    std::string name;
    Move move;
    PokemonType type;
    Category category;
    int power;
    int accuracy;
    int effect_percent;

    bool operator==(const MoveInfo& other) const {
        return name == other.name;
    }
};

template <>
struct std::hash<MoveInfo> {
    size_t operator()(const MoveInfo& a) const noexcept {
        return static_cast<size_t>(a.move);
    }
};

enum class Stat {
    HEALTH,
    ATTACK,
    DEFENSE,
    SPECIAL_ATTACK,
    SPECIAL_DEFENSE,
    SPEED,
    NO_STAT
};

extern std::unordered_map<std::string, Stat> stat_map;

struct SerebiiPokemon {
    std::string name;
    std::unordered_set<PokemonType> types;
    int id = -1;
    std::string ability;
    double pounds = 0;
    std::unordered_map<Stat, uint16_t> base_stats;
    std::unordered_map<int, std::vector<MoveInfo>> level_to_moves;
    std::unordered_map<std::string, MoveInfo> tm_or_hm_to_move;
    std::vector<MoveInfo> egg_moves;
    std::unordered_map<
        int,
        std::unordered_map<int, std::vector<MoveInfo>>
    > pre_evolution_moves;
    std::vector<MoveInfo> move_tutor_attacks;
    std::unordered_map<
        std::string,
        std::unordered_map<int, std::vector<MoveInfo>>
    > game_to_level_to_moves;
    std::vector<MoveInfo> special_moves;
    std::unordered_map<
        std::string,
        std::unordered_map<Stat, uint16_t>
    > form_to_base_stats;
    std::unordered_map<
        std::string,
        std::unordered_map<int, std::vector<MoveInfo>>
    > form_to_level_up_moves;
    std::unordered_map<
        std::string,
        std::unordered_map<std::string, MoveInfo>
    > form_to_tm_or_hm_to_move;
    std::unordered_map<
        std::string,
        std::vector<MoveInfo>
    > form_to_move_tutor_moves;
};

std::unordered_map<std::string, SerebiiPokemon> get_all_serebii_pokemon();

void print_serebii_pokemon();

std::unordered_map<std::string, std::unordered_set<MoveInfo>>
get_moves_for_serebii_pokemon(const SerebiiPokemon& serebii_pokemon);

std::unordered_map<Move, MoveInfo> get_all_pokemon_moves(
    std::unordered_map<std::string, SerebiiPokemon> pokedex
);


#endif //SEREBII_POKEMON_DATA_SOURCE_H
