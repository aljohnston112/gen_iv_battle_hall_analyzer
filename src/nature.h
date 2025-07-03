#ifndef NATURE_H
#define NATURE_H

#include <cstdint>
#include <string>
#include <unordered_map>

#include "moves.h"

static const std::unordered_map<std::string, PokemonType> pokemon_type_map{
    {"normal", PokemonType::NORMAL},
    {"fighting", PokemonType::FIGHTING},
    {"flying", PokemonType::FLYING},
    {"poison", PokemonType::POISON},
    {"ground", PokemonType::GROUND},
    {"rock", PokemonType::ROCK},
    {"bug", PokemonType::BUG},
    {"ghost", PokemonType::GHOST},
    {"steel", PokemonType::STEEL},
    {"fire", PokemonType::FIRE},
    {"water", PokemonType::WATER},
    {"grass", PokemonType::GRASS},
    {"electric", PokemonType::ELECTRIC},
    {"psychic", PokemonType::PSYCHIC},
    {"ice", PokemonType::ICE},
    {"dragon", PokemonType::DRAGON},
    {"dark", PokemonType::DARK}
};

static std::unordered_map<PokemonType, std::string> create_reverse_type_map() {
    std::unordered_map<PokemonType, std::string> reverse_map;
    for (const auto& [str, type] : pokemon_type_map) {
        reverse_map[type] = str;
    }
    return reverse_map;
}

static const auto TYPE_TO_STRING =
    create_reverse_type_map();

static std::string get_type_name(const PokemonType type) {
    const auto it = TYPE_TO_STRING.find(type);
    return (it != TYPE_TO_STRING.end()) ? it->second : "unknown";
}

enum class Stat {
    HEALTH,
    ATTACK,
    DEFENSE,
    SPECIAL_ATTACK,
    SPECIAL_DEFENSE,
    SPEED,
    NO_STAT
};

static const std::unordered_map<std::string, Stat> stat_map{
    {"health", Stat::HEALTH},
    {"attack", Stat::ATTACK},
    {"defense", Stat::DEFENSE},
    {"special_attack", Stat::SPECIAL_ATTACK},
    {"special_defense", Stat::SPECIAL_DEFENSE},
    {"speed", Stat::SPEED}
};

static const std::unordered_map<std::string, Category> move_category_map{
    {"physical", Category::PHYSICAL},
    {"special", Category::SPECIAL},
    {"status", Category::STATUS}
};

enum class NatureEnum {
    HARDY,
    LONELY,
    BRAVE,
    ADAMANT,
    NAUGHTY,
    BOLD,
    DOCILE,
    RELAXED,
    IMPISH,
    LAX,
    TIMID,
    HASTY,
    SERIOUS,
    JOLLY,
    NAIVE,
    MODEST,
    MILD,
    QUIET,
    BASHFUL,
    RASH,
    CALM,
    GENTLE,
    SASSY,
    CAREFUL,
    QUIRKY
};

struct Nature {
    NatureEnum nature;
    Stat up;
    Stat down;
};

const std::unordered_map<NatureEnum, Nature> NATURE_MAP = {
    {
        NatureEnum::HARDY,
        {NatureEnum::HARDY, Stat::NO_STAT, Stat::NO_STAT}
    },
    {
        NatureEnum::LONELY,
        {NatureEnum::LONELY, Stat::ATTACK, Stat::DEFENSE}
    },
    {
        NatureEnum::BRAVE,
        {NatureEnum::BRAVE, Stat::ATTACK, Stat::SPEED}
    },
    {
        NatureEnum::ADAMANT,
        {NatureEnum::ADAMANT, Stat::ATTACK, Stat::SPECIAL_ATTACK}
    },
    {
        NatureEnum::NAUGHTY,
        {NatureEnum::NAUGHTY, Stat::ATTACK, Stat::SPECIAL_DEFENSE}
    },
    {
        NatureEnum::BOLD,
        {NatureEnum::BOLD, Stat::DEFENSE, Stat::ATTACK}
    },
    {
        NatureEnum::DOCILE,
        {NatureEnum::DOCILE, Stat::NO_STAT, Stat::NO_STAT}
    },
    {
        NatureEnum::RELAXED,
        {NatureEnum::RELAXED, Stat::DEFENSE, Stat::SPEED}
    },
    {
        NatureEnum::IMPISH,
        {NatureEnum::IMPISH, Stat::DEFENSE, Stat::SPECIAL_ATTACK}
    },
    {
        NatureEnum::LAX,
        {NatureEnum::LAX, Stat::DEFENSE, Stat::SPECIAL_DEFENSE}
    },
    {
        NatureEnum::TIMID,
        {NatureEnum::TIMID, Stat::SPEED, Stat::ATTACK}
    },
    {
        NatureEnum::HASTY,
        {NatureEnum::HASTY, Stat::SPEED, Stat::DEFENSE}
    },
    {
        NatureEnum::SERIOUS,
        {NatureEnum::SERIOUS, Stat::NO_STAT, Stat::NO_STAT}
    },
    {
        NatureEnum::JOLLY,
        {NatureEnum::JOLLY, Stat::SPEED, Stat::SPECIAL_ATTACK}
    },
    {
        NatureEnum::NAIVE,
        {NatureEnum::NAIVE, Stat::SPEED, Stat::SPECIAL_DEFENSE}
    },
    {
        NatureEnum::MODEST,
        {NatureEnum::MODEST, Stat::SPECIAL_ATTACK, Stat::ATTACK}
    },
    {
        NatureEnum::MILD,
        {NatureEnum::MILD, Stat::SPECIAL_ATTACK, Stat::DEFENSE}
    },
    {
        NatureEnum::QUIET,
        {NatureEnum::QUIET, Stat::SPECIAL_ATTACK, Stat::SPEED}
    },
    {
        NatureEnum::BASHFUL,
        {NatureEnum::BASHFUL, Stat::NO_STAT, Stat::NO_STAT}
    },
    {
        NatureEnum::RASH,
        {NatureEnum::RASH, Stat::SPECIAL_ATTACK, Stat::SPECIAL_DEFENSE}
    },
    {
        NatureEnum::CALM,
        {NatureEnum::CALM, Stat::SPECIAL_DEFENSE, Stat::ATTACK}
    },
    {
        NatureEnum::GENTLE,
        {NatureEnum::GENTLE, Stat::SPECIAL_DEFENSE, Stat::DEFENSE}
    },
    {
        NatureEnum::SASSY,
        {NatureEnum::SASSY, Stat::SPECIAL_DEFENSE, Stat::SPEED}
    },
    {
        NatureEnum::CAREFUL,
        {NatureEnum::CAREFUL, Stat::SPECIAL_DEFENSE, Stat::SPECIAL_ATTACK}
    },
    {
        NatureEnum::QUIRKY,
        {NatureEnum::QUIRKY, Stat::NO_STAT, Stat::NO_STAT}
    }
};

constexpr double BENEFICIAL = 1.1;
constexpr double DETRIMENTAL = 0.9;
const std::unordered_map<Stat, std::unordered_map<NatureEnum, double>>
STAT_NATURE_MULTIPLIER = {
    {
        Stat::ATTACK, {
            {NatureEnum::LONELY, BENEFICIAL},
            {NatureEnum::BRAVE, BENEFICIAL},
            {NatureEnum::ADAMANT, BENEFICIAL},
            {NatureEnum::NAUGHTY, BENEFICIAL},
            {NatureEnum::BOLD, DETRIMENTAL},
            {NatureEnum::MODEST, DETRIMENTAL},
            {NatureEnum::CALM, DETRIMENTAL},
            {NatureEnum::TIMID, DETRIMENTAL}
        }
    },
    {
        Stat::DEFENSE, {
            {NatureEnum::BOLD, BENEFICIAL},
            {NatureEnum::RELAXED, BENEFICIAL},
            {NatureEnum::IMPISH, BENEFICIAL},
            {NatureEnum::LAX, BENEFICIAL},
            {NatureEnum::LONELY, DETRIMENTAL},
            {NatureEnum::MILD, DETRIMENTAL},
            {NatureEnum::GENTLE, DETRIMENTAL},
            {NatureEnum::HASTY, DETRIMENTAL}
        }
    },
    {
        Stat::SPECIAL_ATTACK, {
            {NatureEnum::MODEST, BENEFICIAL},
            {NatureEnum::MILD, BENEFICIAL},
            {NatureEnum::QUIET, BENEFICIAL},
            {NatureEnum::RASH, BENEFICIAL},
            {NatureEnum::ADAMANT, DETRIMENTAL},
            {NatureEnum::IMPISH, DETRIMENTAL},
            {NatureEnum::CAREFUL, DETRIMENTAL},
            {NatureEnum::JOLLY, DETRIMENTAL}
        }
    },
    {
        Stat::SPECIAL_DEFENSE, {
            {NatureEnum::CALM, BENEFICIAL},
            {NatureEnum::GENTLE, BENEFICIAL},
            {NatureEnum::SASSY, BENEFICIAL},
            {NatureEnum::CAREFUL, BENEFICIAL},
            {NatureEnum::NAUGHTY, DETRIMENTAL},
            {NatureEnum::LAX, DETRIMENTAL},
            {NatureEnum::NAIVE, DETRIMENTAL},
            {NatureEnum::RASH, DETRIMENTAL}
        }
    },
    {
        Stat::SPEED, {
            {NatureEnum::TIMID, BENEFICIAL},
            {NatureEnum::HASTY, BENEFICIAL},
            {NatureEnum::JOLLY, BENEFICIAL},
            {NatureEnum::NAIVE, BENEFICIAL},
            {NatureEnum::BRAVE, DETRIMENTAL},
            {NatureEnum::RELAXED, DETRIMENTAL},
            {NatureEnum::QUIET, DETRIMENTAL},
            {NatureEnum::SASSY, DETRIMENTAL}
        }
    },
};

static const std::unordered_map<std::string, NatureEnum> STRING_TO_NATURE = {
    {"Hardy", NatureEnum::HARDY},
    {"Lonely", NatureEnum::LONELY},
    {"Brave", NatureEnum::BRAVE},
    {"Adamant", NatureEnum::ADAMANT},
    {"Naughty", NatureEnum::NAUGHTY},
    {"Bold", NatureEnum::BOLD},
    {"Docile", NatureEnum::DOCILE},
    {"Relaxed", NatureEnum::RELAXED},
    {"Impish", NatureEnum::IMPISH},
    {"Lax", NatureEnum::LAX},
    {"Timid", NatureEnum::TIMID},
    {"Hasty", NatureEnum::HASTY},
    {"Serious", NatureEnum::SERIOUS},
    {"Jolly", NatureEnum::JOLLY},
    {"Naive", NatureEnum::NAIVE},
    {"Modest", NatureEnum::MODEST},
    {"Mild", NatureEnum::MILD},
    {"Quiet", NatureEnum::QUIET},
    {"Bashful", NatureEnum::BASHFUL},
    {"Rash", NatureEnum::RASH},
    {"Calm", NatureEnum::CALM},
    {"Gentle", NatureEnum::GENTLE},
    {"Sassy", NatureEnum::SASSY},
    {"Careful", NatureEnum::CAREFUL},
    {"Quirky", NatureEnum::QUIRKY}
};

static const std::unordered_map<NatureEnum, std::string> NATURE_TO_STRING = [] {
    std::unordered_map<NatureEnum, std::string> map;
    for (const auto& [str, nature] : STRING_TO_NATURE) {
        map[nature] = str;
    }
    return map;
}();

uint16_t get_stat(
    uint8_t level,
    Stat stat,
    uint8_t base,
    uint8_t iv,
    u_int16_t ev,
    Nature nature
);


#endif //NATURE_H
