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
             level,
             item,
             types,
             moves,
             stats
         ] : pokemon_list
    ) {
        output_stream << pokemon_name << '|'
        << std::to_string(level) << '|'
        << item << '\n';

        output_stream << types.size() << '\n';
        for (const auto& type : types) {
            output_stream << static_cast<int>(type) << '\n';
        }

        output_stream << moves.size() << '\n';
        for (const auto& [
                 move_name,
                 move,
                 move_type,
                 category,
                 power,
                 accuracy,
                 effect_percent
             ] : moves
        ) {
            output_stream << move_name << ','
                << static_cast<int>(move) << ','
                << static_cast<int>(move_type) << ','
                << static_cast<int>(category) << ','
                << power << ','
                << accuracy << ','
                << effect_percent << '\n';
        }
        output_stream << stats.size() << '\n';
        for (const auto& [stat, value] : stats) {
            output_stream << static_cast<int>(stat) << ','
                << value << '\n';
        }
    }
}

std::vector<CustomPokemon> load_custom_pokemon(const std::string& filename) {
    std::ifstream input_stream(filename);
    std::vector<CustomPokemon> all_custom_pokemon;
    std::string line;
    while (std::getline(input_stream, line)) {
        CustomPokemon custom_pokemon;
        std::istringstream header(line);
        std::getline(header, custom_pokemon.name, '|');
        std::string level;
        std::getline(header, level, '|');
        custom_pokemon.level = std::stoi(level);
        std::getline(header, custom_pokemon.item);

        size_t type_count;
        input_stream >> type_count;
        input_stream.ignore();
        for (size_t i = 0; i < type_count; ++i) {
            std::getline(input_stream, line);
            std::istringstream stat_string_stream(line);

            int type_int;
            stat_string_stream >> type_int;
            stat_string_stream.ignore();
            custom_pokemon.types.emplace(std::move(static_cast<PokemonType>(type_int)));
        }

        size_t move_count;
        input_stream >> move_count;
        input_stream.ignore();
        for (size_t i = 0; i < move_count; ++i) {
            MoveInfo move_info;
            std::getline(input_stream, line);
            std::istringstream move_string_stream(line);
            std::getline(move_string_stream, move_info.name, ',');

            int move_int;
            move_string_stream >> move_int;
            move_string_stream.ignore();
            move_info.move = static_cast<Move>(move_int);

            int type_int;
            move_string_stream >> type_int;
            move_string_stream.ignore();
            move_info.move_type = static_cast<PokemonType>(type_int);

            int category_int;
            move_string_stream >> category_int;
            move_string_stream.ignore();
            move_info.category = static_cast<Category>(category_int);

            move_string_stream >> move_info.power;
            move_string_stream.ignore();

            move_string_stream >> move_info.accuracy;
            move_string_stream.ignore();

            move_string_stream >> move_info.effect_percent;

            custom_pokemon.moves.emplace(std::move(move_info));
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
            custom_pokemon.stats[static_cast<Stat>(stat_int)] = value;
        }

        all_custom_pokemon.push_back(std::move(custom_pokemon));
    }

    return all_custom_pokemon;
}
