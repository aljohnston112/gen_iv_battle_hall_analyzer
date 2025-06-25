#include "nature.h"

#include <cmath>

int calculate_hp_stat(
    const uint8_t level,
    const uint8_t base,
    const uint8_t iv,
    const u_int16_t ev
) {
    return ((2 * base + iv + (ev / 4)) * level) / 100 + level + 10;
}

int calculate_non_hp_stat(
    const uint8_t level,
    const Stat stat,
    const uint8_t base,
    const uint8_t iv,
    const u_int16_t ev,
    const Nature nature
) {
    const int intermediate = ((2 * base + iv + (ev / 4)) * level) / 100 + 5;
    double nature_multiplier = 1.0;
    if (nature.down == stat) {
        nature_multiplier = 0.9;
    } else if (nature.up == stat) {
        nature_multiplier = 1.1;
    }
    return static_cast<int>(std::floor(intermediate * nature_multiplier));
}

uint16_t get_stat(
    const uint8_t level,
    const Stat stat,
    const uint8_t base,
    const uint8_t iv,
    const u_int16_t ev,
    const Nature nature
) {
    if (stat == Stat::HEALTH) {
        return calculate_hp_stat(level, base, iv, ev);
    }
    return calculate_non_hp_stat(
        level,
        stat,
        base,
        iv,
        ev,
        nature
    );
}