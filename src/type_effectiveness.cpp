#include "type_effectiveness.h"

double get_effectiveness(
    const std::array<PokemonType, 2>& defender_types,
    const PokemonType& attack_type
) {
    static constexpr int number_of_types =
        static_cast<int>(PokemonType::COUNT) + 1;
    auto key = number_of_types * number_of_types * static_cast<int>(
        attack_type);
    if (auto it = defender_types.begin();
        it != defender_types.end()
    ) {
        key += number_of_types * static_cast<int>(*it);
        ++it;
        if (it != defender_types.end()) {
            key += static_cast<int>(*it);
        }
    }
    static constexpr int cache_size = number_of_types * number_of_types *
        number_of_types;
    static std::array<double, cache_size> cache = [] {
        std::array<double, cache_size> arr{};
        arr.fill(-1.0);
        return arr;
    }();
    if (const auto value = cache[key];
        value != -1.0
    ) {
        return value;
    }

    double multiplier = 1.0;
    for (const auto defender_type : defender_types) {
        if (const auto defender_it = defender_type_map.find(defender_type);
            defender_it != defender_type_map.end()
        ) {
            const auto& inner_map = defender_it->second;
            if (const auto attacker_it = inner_map.find(attack_type);
                attacker_it != inner_map.end()
            ) {
                multiplier *= attacker_it->second;
            }
        }
    }
    cache[key] = multiplier;
    return multiplier;
}
