#ifndef TYPE_EFFECTIVENESS_H
#define TYPE_EFFECTIVENESS_H
#include <unordered_map>

#include "nature.h"

constexpr double not_effective = 0.0;
constexpr double not_very_effective = 0.5;
constexpr double super_effective = 2.0;

const std::unordered_map<
    PokemonType,
    std::unordered_map<PokemonType, double>
> defender_type_map = {
    {
        PokemonType::NORMAL,
        {
            {PokemonType::FIGHTING, super_effective},
            {PokemonType::GHOST, not_effective},
        }
    },
    {
        PokemonType::FIRE,
        {
            {PokemonType::FIRE, not_very_effective},
            {PokemonType::WATER, super_effective},
            {PokemonType::GRASS, not_very_effective},
            {PokemonType::ICE, not_very_effective},
            {PokemonType::GROUND, super_effective},
            {PokemonType::BUG, not_very_effective},
            {PokemonType::ROCK, super_effective},
            {PokemonType::STEEL, not_very_effective},
        }
    },
    {
        PokemonType::WATER,
        {
            {PokemonType::FIRE, not_very_effective},
            {PokemonType::WATER, not_very_effective},
            {PokemonType::ELECTRIC, super_effective},
            {PokemonType::GRASS, super_effective},
            {PokemonType::ICE, not_very_effective},
            {PokemonType::STEEL, not_very_effective},
        }
    },
    {
        PokemonType::ELECTRIC,
        {
            {PokemonType::ELECTRIC, not_very_effective},
            {PokemonType::GROUND, super_effective},
            {PokemonType::FLYING, not_very_effective},
            {PokemonType::STEEL, not_very_effective},
        }
    },
    {
        PokemonType::GRASS,
        {
            {PokemonType::FIRE, super_effective},
            {PokemonType::WATER, not_very_effective},
            {PokemonType::ELECTRIC, not_very_effective},
            {PokemonType::GRASS, not_very_effective},
            {PokemonType::ICE, super_effective},
            {PokemonType::POISON, super_effective},
            {PokemonType::GROUND, not_very_effective},
            {PokemonType::FLYING, super_effective},
            {PokemonType::BUG, super_effective},
        }
    },
    {
        PokemonType::ICE,
        {
            {PokemonType::FIRE, super_effective},
            {PokemonType::ICE, not_very_effective},
            {PokemonType::FIGHTING, super_effective},
            {PokemonType::ROCK, super_effective},
            {PokemonType::STEEL, super_effective},
        }
    },
    {
        PokemonType::FIGHTING,
        {
            {PokemonType::FLYING, super_effective},
            {PokemonType::PSYCHIC, super_effective},
            {PokemonType::BUG, not_very_effective},
            {PokemonType::ROCK, not_very_effective},
            {PokemonType::DARK, not_very_effective},
        }
    },
    {
        PokemonType::POISON,
        {
            {PokemonType::GRASS, not_very_effective},
            {PokemonType::FIGHTING, not_very_effective},
            {PokemonType::POISON, not_very_effective},
            {PokemonType::GROUND, super_effective},
            {PokemonType::PSYCHIC, super_effective},
            {PokemonType::BUG, not_very_effective},
        }
    },
    {
        PokemonType::GROUND,
        {
            {PokemonType::WATER, super_effective},
            {PokemonType::ELECTRIC, not_effective},
            {PokemonType::GRASS, super_effective},
            {PokemonType::ICE, super_effective},
            {PokemonType::POISON, not_very_effective},
            {PokemonType::ROCK, not_very_effective},
        }
    },
    {
        PokemonType::FLYING,
        {
            {PokemonType::ELECTRIC, super_effective},
            {PokemonType::GRASS, not_very_effective},
            {PokemonType::ICE, super_effective},
            {PokemonType::FIGHTING, not_very_effective},
            {PokemonType::GROUND, not_effective},
            {PokemonType::BUG, not_very_effective},
            {PokemonType::ROCK, super_effective},
        }
    },
    {
        PokemonType::PSYCHIC,
        {
            {PokemonType::FIGHTING, not_very_effective},
            {PokemonType::PSYCHIC, not_very_effective},
            {PokemonType::BUG, super_effective},
            {PokemonType::GHOST, super_effective},
            {PokemonType::DARK, super_effective},
        }
    },
    {
        PokemonType::BUG,
        {
            {PokemonType::FIRE, super_effective},
            {PokemonType::GRASS, not_very_effective},
            {PokemonType::FIGHTING, not_very_effective},
            {PokemonType::GROUND, not_very_effective},
            {PokemonType::FLYING, super_effective},
            {PokemonType::ROCK, super_effective},
        }
    },
    {
        PokemonType::ROCK,
        {
            {PokemonType::NORMAL, not_very_effective},
            {PokemonType::FIRE, not_very_effective},
            {PokemonType::WATER, super_effective},
            {PokemonType::GRASS, super_effective},
            {PokemonType::FIGHTING, super_effective},
            {PokemonType::POISON, not_very_effective},
            {PokemonType::GROUND, super_effective},
            {PokemonType::FLYING, not_very_effective},
            {PokemonType::STEEL, super_effective},
        }
    },
    {
        PokemonType::GHOST,
        {
            {PokemonType::NORMAL, not_effective},
            {PokemonType::FIGHTING, not_effective},
            {PokemonType::POISON, not_very_effective},
            {PokemonType::BUG, not_very_effective},
            {PokemonType::GHOST, super_effective},
            {PokemonType::DARK, super_effective},
        }
    },
    {
        PokemonType::DRAGON,
        {
            {PokemonType::FIRE, not_very_effective},
            {PokemonType::WATER, not_very_effective},
            {PokemonType::ELECTRIC, not_very_effective},
            {PokemonType::GRASS, not_very_effective},
            {PokemonType::ICE, super_effective},
            {PokemonType::DRAGON, super_effective},
        }
    },
    {
        PokemonType::DARK,
        {
            {PokemonType::FIGHTING, super_effective},
            {PokemonType::PSYCHIC, not_effective},
            {PokemonType::BUG, super_effective},
            {PokemonType::GHOST, not_very_effective},
            {PokemonType::DARK, not_very_effective},
        }
    },
    {
        PokemonType::STEEL,
        {
            {PokemonType::NORMAL, not_very_effective},
            {PokemonType::FIRE, super_effective},
            {PokemonType::GRASS, not_very_effective},
            {PokemonType::ICE, not_very_effective},
            {PokemonType::FIGHTING, super_effective},
            {PokemonType::POISON, not_effective},
            {PokemonType::GROUND, super_effective},
            {PokemonType::FLYING, not_very_effective},
            {PokemonType::PSYCHIC, not_very_effective},
            {PokemonType::BUG, not_very_effective},
            {PokemonType::ROCK, not_very_effective},
            {PokemonType::GHOST, not_very_effective},
            {PokemonType::DRAGON, not_very_effective},
            {PokemonType::DARK, not_very_effective},
            {PokemonType::STEEL, not_very_effective},
        }
    },
};

double get_effectiveness(
    const std::array<PokemonType, 2>& defender_types,
    const PokemonType& attack_type
);

#endif //TYPE_EFFECTIVENESS_H
