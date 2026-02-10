#include "factory_data_source.h"

#include <string>
#include <unordered_map>

inline std::unordered_map<std::string, BaseStats>
parse_base_stats_file(const char* path) {
    simdjson::ondemand::parser parser;
    simdjson::padded_string json = simdjson::padded_string::load(path);
    simdjson::ondemand::document doc = parser.iterate(json);

    std::unordered_map<std::string, BaseStats> out;

    for (auto field : doc.get_object()) {
        simdjson::ondemand::object stat_obj = field.value();

        auto key = std::string(stat_obj["name"]);
        BaseStats stats{};
        stats.health = static_cast<int64_t>(stat_obj["health"]);
        stats.attack = static_cast<int64_t>(stat_obj["attack"]);
        stats.defense = static_cast<int64_t>(stat_obj["defense"]);
        stats.special_attack = static_cast<int64_t>(stat_obj["special_attack"]);
        stats.special_defense =
            static_cast<int64_t>(stat_obj["special_defense"]);
        stats.speed = static_cast<int64_t>(stat_obj["speed"]);

        out.emplace(key, std::move(stats));
    }

    return out;
}

inline std::unordered_map<std::string, BattleFactoryPokemon>
parse_battle_factory_file(const char* path) {
    simdjson::ondemand::parser parser;
    simdjson::padded_string json = simdjson::padded_string::load(path);
    simdjson::ondemand::document pokemon = parser.iterate(json);

    std::unordered_map<std::string, BattleFactoryPokemon> out;

    for (auto field : pokemon.get_object()) {
        auto key = std::string(field.key().raw());

        simdjson::ondemand::object poke_obj = field.value();

        BattleFactoryPokemon pset;
        pset.name = std::string(poke_obj["name"]);
        pset.unique_id = std::string(poke_obj["unique_id"]);
        auto nature = std::string(poke_obj["nature"].value());
        nature[0] = static_cast<char>(std::toupper(nature[0]));
        pset.nature = NATURE_MAP.at(
            STRING_TO_NATURE.at(std::string(nature))
        );
        pset.item = std::string(poke_obj["item"]);
        pset.set_number = static_cast<int64_t>(poke_obj["set_number"]);

        uint8_t n_types = 0;
        for (auto t :
             poke_obj["types"].get_array()
        ) {
            pset.types[n_types++] = pokemon_type_map.at(std::string(t));
        }
        if (n_types == 1) {
            pset.types[n_types] = PokemonType::COUNT;
        }

        std::vector<const MoveInfo*> moves;
        for (auto mv :
             poke_obj["moves"].get_array()
        ) {
            auto name = std::string(mv["name"]);
            if (name.compare("Sand-Attack") == 0) {
                name = "Sand-attack";
            } else if (name.compare("Vice Grip") == 0) {
                name = "Vicegrip";
            } else if (name.compare("Double Slap") == 0) {
                name = "Doubleslap";
            } else if (name.compare("Conversion2") == 0) {
                name = "Conversion 2";
            } else if (name.compare("Defense Order") == 0) {
                name = "Defend Order";
            }
            moves.push_back(
                &MOVE_INFO_MAP.at(static_cast<int>(MOVE_MAP.at(name))));
        }
        pset.moves = std::move(moves);

        pset.effort_values[Stat::HEALTH] = 0;
        pset.effort_values[Stat::ATTACK] = 0;
        pset.effort_values[Stat::DEFENSE] = 0;
        pset.effort_values[Stat::SPECIAL_ATTACK] = 0;
        pset.effort_values[Stat::SPECIAL_DEFENSE] = 0;
        pset.effort_values[Stat::SPEED] = 0;

        for (auto ev :
             poke_obj["effort_values"].get_array()
        ) {
            const auto stat_type = std::string(ev["stat_type"]);
            int16_t value = static_cast<int16_t>(ev["value"]);
            pset.effort_values[stat_map.at(stat_type)] = value;
        }

        out.emplace(pset.unique_id, std::move(pset));
    }

    return out;
}

inline std::vector<CustomPokemon> get_custom_batle_factory_pokemon() {
    const auto all_serebii_pokemon =
        get_all_serebii_pokemon();
    const auto all_base_stats =
        parse_base_stats_file("data/fresh/base_stats");
    const auto battle_factory_pokemon =
        parse_battle_factory_file("data/fresh/battle_factory_pokemon");

    std::vector<CustomPokemon> custom_battle_factory_pokemon{};

    /*
    struct CustomPokemon {
        Pokemon name;
        Ability ability;
        uint8_t level;
        Item item;
        std::array<PokemonType, 2> types;
        std::vector<const MoveInfo*> moves;
        std::array<uint16_t, static_cast<int>(Stat::NO_STAT)> stats;
        double pounds;
    };
    */

    /*
    uint16_t get_stat(
        const uint8_t level,
        const Stat stat,
        const uint8_t base,
        const uint8_t iv,
        const u_int16_t ev,
        const Nature nature
    )
    */

    for (const auto& [_, p] : battle_factory_pokemon) {
        auto original_name = p.name;
        auto name = p.name;
        if (name.compare("Wormadam") == 0) {
            name = "WormadamP";
        }
        const auto serebii_pokemon = all_serebii_pokemon.at(original_name);
        const auto abilities = ABILITY_MAP.at(STRING_TO_POKEMON.at(name));
        const auto [
            base_health,
            base_attack,
            base_defense,
            base_special_attack,
            base_special_defense,
            base_speed
        ] = all_base_stats.at(original_name);
        const int iv = get_iv_for_battle_factory(p.set_number);
        const auto nature = p.nature;
        const auto hp = get_stat(
            BATTLE_FACTORY_LEVEL,
            Stat::HEALTH,
            base_health,
            iv,
            p.effort_values.at(Stat::HEALTH),
            nature
        );
        const auto attack = get_stat(
            BATTLE_FACTORY_LEVEL,
            Stat::ATTACK,
            base_attack,
            iv,
            p.effort_values.at(Stat::ATTACK),
            nature
        );
        const auto defense = get_stat(
            BATTLE_FACTORY_LEVEL,
            Stat::DEFENSE,
            base_defense,
            iv,
            p.effort_values.at(Stat::DEFENSE),
            nature
        );
        const auto special_attack = get_stat(
            BATTLE_FACTORY_LEVEL,
            Stat::SPECIAL_ATTACK,
            base_special_attack,
            iv,
            p.effort_values.at(Stat::SPECIAL_ATTACK),
            nature
        );
        const auto special_defense = get_stat(
            BATTLE_FACTORY_LEVEL,
            Stat::SPECIAL_DEFENSE,
            base_special_defense,
            iv,
            p.effort_values.at(Stat::SPECIAL_DEFENSE),
            nature
        );
        const auto speed = get_stat(
            BATTLE_FACTORY_LEVEL,
            Stat::SPEED,
            base_speed,
            iv,
            p.effort_values.at(Stat::SPEED),
            nature
        );
        for (const auto& ability : abilities) {
            custom_battle_factory_pokemon.emplace_back(
                CustomPokemon{
                    .name = STRING_TO_POKEMON.at(name),
                    .ability = ability,
                    .level = BATTLE_FACTORY_LEVEL,
                    .item = STRING_TO_ITEM.at(p.item),
                    .types = p.types,
                    .moves = p.moves,
                    .stats = {
                        hp,
                        attack,
                        defense,
                        special_attack,
                        special_defense,
                        speed
                    },
                    .pounds = serebii_pokemon.pounds
                }
            );
        }
    }

    return custom_battle_factory_pokemon;
}

