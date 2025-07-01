#include "type_effectiveness.h"

double get_effectiveness(
    const std::array<PokemonType, 2>& defender_types,
    const PokemonType& attack_type
) {
    static constexpr int number_of_types =
        static_cast<int>(PokemonType::COUNT) + 1;
    static constexpr int cache_size =
        number_of_types * number_of_types * number_of_types;
    static std::array<double, cache_size> cache = [] {
        std::array<double, cache_size> arr{};
        arr.fill(-1.0);
        return arr;
    }();

    auto key =
        number_of_types * number_of_types * static_cast<int>(attack_type);
    key += number_of_types * static_cast<int>(defender_types[0]);
    key += static_cast<int>(defender_types[1]);
    if (const auto value = cache[key];
        value != -1.0
    ) {
        return value;
    }

    double multiplier = 1.0;
    for (const auto defender_type : defender_types) {
        if (const auto attacking_type_entry =
            EFFECTIVENESS_MAP.find(defender_type);
            attacking_type_entry != EFFECTIVENESS_MAP.end()
        ) {
            const auto& attacking_type_map = attacking_type_entry->second;
            if (const auto effectiveness_entry =
                attacking_type_map.find(attack_type);
                effectiveness_entry != attacking_type_map.end()
            ) {
                multiplier *= effectiveness_entry->second;
            }
        }
    }
    cache[key] = multiplier;
    return multiplier;
}
