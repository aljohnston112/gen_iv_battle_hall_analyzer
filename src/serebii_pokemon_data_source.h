#ifndef SEREBII_POKEMON_DATA_SOURCE_H
#define SEREBII_POKEMON_DATA_SOURCE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "custom_pokemon.h"
#include "moves.h"
#include "nature.h"

struct SerebiiPokemon {
    std::string name;
    std::vector<PokemonType> types;
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

std::unordered_map<std::string, std::unordered_map<Move, const MoveInfo*>>
get_moves_for_serebii_pokemon(const SerebiiPokemon& serebii_pokemon);

std::vector<const MoveInfo*> get_all_pokemon_moves(
    const std::unordered_map<std::string, SerebiiPokemon>& pokedex
);

std::unordered_map<std::string, CustomPokemon> convert_serebii_to_custom(
    const SerebiiPokemon& serebii_pokemon
);


#endif //SEREBII_POKEMON_DATA_SOURCE_H
