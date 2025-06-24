#ifndef NATURE_H
#define NATURE_H
#include "serebii_pokemon_data_source.h"

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

#endif //NATURE_H
