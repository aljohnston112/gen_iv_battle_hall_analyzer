#ifndef GEN_IV_BATTLE_HALL_ANALYZER_EVSET_H
#define GEN_IV_BATTLE_HALL_ANALYZER_EVSET_H

#include "nature.h"

struct EVSet {
    int health;
    int attack;
    int defense;
    int special_attack;
    int special_defense;
    int speed;
    Stat nature_stat;

    bool is_less_than(const EVSet& b) const {
        return (nature_stat == b.nature_stat || nature_stat == Stat::NO_STAT) &&
            health <= b.health &&
            attack <= b.attack &&
            defense <= b.defense &&
            special_attack <= b.special_attack &&
            special_defense <= b.special_defense &&
            speed <= b.speed;
    }

    bool operator==(const EVSet& b) const {
        return (nature_stat == b.nature_stat) &&
            health == b.health &&
            attack == b.attack &&
            defense == b.defense &&
            special_attack == b.special_attack &&
            special_defense == b.special_defense &&
            speed == b.speed;
    }
};

struct EVSetHash {
    std::size_t operator()(const EVSet& p) const {
        return static_cast<int>(p.health) << 0 |
            static_cast<uint64_t>(p.attack) << 8 |
            static_cast<uint64_t>(p.defense) << 16 |
            static_cast<uint64_t>(p.special_attack) << 24 |
            static_cast<uint64_t>(p.special_defense) << 32 |
            static_cast<uint64_t>(p.speed) << 40 |
            static_cast<uint64_t>(p.nature_stat) << 48;
    }
};


#endif //GEN_IV_BATTLE_HALL_ANALYZER_EVSET_H
