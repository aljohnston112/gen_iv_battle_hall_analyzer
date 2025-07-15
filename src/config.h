#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>

#include "serebii_pokemon_data_source.h"

constexpr uint8_t LEVEL = 30;
constexpr bool SKIP_RANKS = false;

const std::unordered_map<PokemonType, int> type_to_rank_to_skip = {
    {PokemonType::DARK, 5},
    {PokemonType::WATER, 5},
    {PokemonType::FLYING, 5},
    {PokemonType::POISON, 5},
    {PokemonType::GROUND, 5},
    {PokemonType::GHOST, 5},
    {PokemonType::FIRE, 8},
    {PokemonType::NORMAL, 5},
    {PokemonType::ELECTRIC, 5},
    {PokemonType::STEEL, 2},
    {PokemonType::BUG, 3},
    {PokemonType::PSYCHIC, 2},
    {PokemonType::DRAGON, 5},
    {PokemonType::ROCK, 2},
    {PokemonType::GRASS, 2},
    {PokemonType::FIGHTING, 2},
    {PokemonType::ICE, 5},
};

static std::vector<
    std::unordered_map<std::string, std::vector<CustomPokemon>>
> get_pokemon_forms(
    const std::unordered_map<std::string, SerebiiPokemon>& all_serebii_pokemon
) {
    std::vector<std::unordered_map<
        std::string,
        std::vector<CustomPokemon>
    >> pokemon_forms{};
    constexpr bool all = true;
    if (all) {
        for (const auto& serebii_pokemon : all_serebii_pokemon |
             std::views::values
        ) {
            pokemon_forms.push_back(
                convert_serebii_to_custom(
                    serebii_pokemon,
                    true,
                    false
                )
            );
        }
    } else {
        pokemon_forms.push_back(
            convert_serebii_to_custom(
                all_serebii_pokemon.at("Azurill"),
                true,
                true
            )
        );
        for (auto& p_list :
             pokemon_forms.at(0) | std::views::values
        ) {
            static std::unordered_set moves_to_include{
                // Move::ThunderPunch,
                // Move::Fling,
                // Move::Detect,
                // Move::CloseCombat,
                Move::Return,
                Move::Waterfall,
            };
            for (auto& p : p_list) {
                std::vector<const MoveInfo*> moves{};
                for (const auto& move : p.moves) {
                    if (moves_to_include.contains(move->move)) {
                        moves.push_back(move);
                    }
                }
                p.moves = moves;

                p.stats[0] = 70;
                p.stats[1] = 38;
                p.stats[2] = 32;
                p.stats[3] = 15;
                p.stats[4] = 29;
                p.stats[5] = 33;
                p.item = Item::FocusSash;
                p.ability = Ability::HugePower;
            }
        }
    }
    return pokemon_forms;
}

#endif //CONFIG_H
