#include "custom_pokemon.h"

#include <fstream>
#include <sstream>

void save_custom_pokemon(
    const std::vector<CustomPokemon>& pokemon_list,
    const std::string& filename
) {
    std::ofstream output_stream(filename);
    for (const auto& [
             pokemon_name,
             ability,
             level,
             item,
             types,
             moves,
             stats,
             pounds
         ] : pokemon_list
    ) {
        output_stream << static_cast<int>(pokemon_name) << '|'
            << static_cast<int>(ability) << '|'
            << std::to_string(level) << '|'
            << static_cast<int>(item) << '\n';

        output_stream << types.size() << '\n';
        for (const auto& type : types) {
            output_stream << static_cast<int>(type) << '\n';
        }

        output_stream << moves.size() << '\n';
        for (const auto move : moves) {
            const auto& [
                move_name,
                move_enum,
                move_type,
                category,
                power,
                accuracy,
                power_points,
                effect_percent
            ] = *move;
            output_stream << static_cast<int>(move_enum) << '\n';
        }
        output_stream << stats.size() << '\n';
        uint8_t i = 0;
        for (const auto& stat : stats) {
            output_stream << static_cast<int>(i++) << ','
                << stat << '\n';
        }
        output_stream << pounds << '\n';
    }
}

std::vector<CustomPokemon> load_custom_pokemon(
    const std::string& filename,
    const std::vector<const MoveInfo*>& all_moves
) {
    std::ifstream input_stream(filename);
    std::vector<CustomPokemon> all_custom_pokemon;
    std::string line;
    while (std::getline(input_stream, line)) {
        CustomPokemon custom_pokemon;
        std::istringstream header(line);
        std::string name_enum;
        std::getline(header, name_enum, '|');
        custom_pokemon.name = static_cast<Pokemon>(std::stoi(name_enum));
        std::string ability_enum;
        std::getline(header, ability_enum, '|');
        custom_pokemon.ability = static_cast<Ability>(std::stoi(ability_enum));
        std::string level;
        std::getline(header, level, '|');
        custom_pokemon.level = std::stoi(level);
        std::string item_enum;
        std::getline(header, item_enum, '|');
        custom_pokemon.item = static_cast<Item>(std::stoi(item_enum));

        size_t type_count;
        input_stream >> type_count;
        input_stream.ignore();
        for (size_t i = 0; i < type_count; ++i) {
            std::getline(input_stream, line);
            std::istringstream stat_string_stream(line);

            int type_int;
            stat_string_stream >> type_int;
            stat_string_stream.ignore();
            custom_pokemon.types[i] = static_cast<PokemonType>(type_int);
        }

        size_t move_count;
        input_stream >> move_count;
        input_stream.ignore();
        for (size_t i = 0; i < move_count; ++i) {
            std::getline(input_stream, line);
            std::istringstream move_string_stream(line);
            int move_int;
            move_string_stream >> move_int;
            move_string_stream.ignore();
            custom_pokemon.moves.emplace_back(all_moves[move_int]);
        }

        size_t stat_count;
        input_stream >> stat_count;
        input_stream.ignore();

        for (size_t i = 0; i < stat_count; ++i) {
            std::getline(input_stream, line);
            std::istringstream stat_string_stream(line);

            int stat_int;
            stat_string_stream >> stat_int;
            stat_string_stream.ignore();

            int value;
            stat_string_stream >> value;
            custom_pokemon.stats[stat_int] = value;
        }

        input_stream >> custom_pokemon.pounds;
        all_custom_pokemon.emplace_back(std::move(custom_pokemon));
        input_stream.ignore();
    }

    return all_custom_pokemon;
}
