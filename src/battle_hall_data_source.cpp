#include "battle_hall_data_source.h"

#include <filesystem>
#include <fstream>
#include <iostream>

std::string replace_gender_symbols(const std::string& pokemon_name) {
    std::string result = pokemon_name;
    size_t i;
    while ((i = result.find("♀")) != std::string::npos) {
        result.replace(i, 3, "(F)");
    }
    while ((i = result.find("♂")) != std::string::npos) {
        result.replace(i, 3, "(M)");
    }
    return result;
}

Pokemon parse_pokemon_line(const std::string& line) {
    std::istringstream string_stream(line);
    Pokemon pokemon;
    std::string token;
    // Skip ID
    std::getline(string_stream, token, '\t');
    // Skip name with "(Pokémon)"
    std::getline(string_stream, token, '\t');
    // Get the name
    std::getline(string_stream, token, '\t');
    pokemon.name = replace_gender_symbols(token);
    // Get item
    std::getline(string_stream, token, '\t');
    pokemon.item = token.substr(0, token.size() / 2);

    // Get moves
    pokemon.moves.reserve(NUMBER_OF_MOVES);
    for (int i = 0; i < NUMBER_OF_MOVES; ++i) {
        std::getline(string_stream, token, '\t');
        pokemon.moves.push_back(token);
    }
    // Get nature
    std::getline(string_stream, pokemon.nature, '\t');
    // Get EVs
    pokemon.evs.reserve(NUMBER_OF_EVS);
    for (int i = 0; i < NUMBER_OF_EVS; ++i) {
        std::getline(string_stream, token, '\t'); // EV
        if (token == "- " || token == "-") {
            pokemon.evs.push_back(0);
        } else {
            pokemon.evs.push_back(std::stoi(token));
        }
    }
    return pokemon;
}

GroupedPokemon parse_file(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    uint8_t current_group_number;
    GroupedPokemon grouped_pokemon;

    while (std::getline(file, line)) {
        if (line.rfind("Group", 0) == 0) {
            current_group_number = std::stoi(line.substr(6));
            grouped_pokemon[current_group_number] = {};
        } else if (!line.empty()) {
            grouped_pokemon[current_group_number].emplace_back(
                parse_pokemon_line(line)
            );
        }
    }
    return grouped_pokemon;
}

void write_battle_hall_data(
    const std::string& file_name,
    const GroupedPokemon& grouped_pokemon
) {
    std::ofstream out(file_name);
    out <<
        "Group,Name,Item,Move1,Move2,Move3,Move4,Nature,HP,ATK,DEF,SA,SD,SPD\n";
    for (const auto& [
             group_number,
             pokemon_group
         ] : grouped_pokemon
    ) {
        for (const auto& [
                 name,
                 item,
                 moves,
                 nature,
                 evs
             ] : pokemon_group
        ) {
            out << static_cast<int>(group_number) << ','
                << name << ','
                << item << ',';
            for (int i = 0; i < 4; i++) {
                out << moves[i] << ',';
            }
            out << nature << ',';
            for (size_t i = 0; i < evs.size() - 1; i++) {
                out << std::to_string(evs[i]) << ',';
            }
            out << std::to_string(evs[evs.size() - 1]) << '\n';
        }
    }
}

GroupedPokemon get_battle_hall_pokemon() {
    const auto file = "./data/fresh/battle_hall_pokemon";
    if (!std::filesystem::exists(file)) {
        write_battle_hall_data(
            file,
            parse_file(
                "./data/raw/battle_hall_pokemon"
            )
        );
    }
    std::ifstream input_stream(file);
    std::string line;
    GroupedPokemon grouped_pokemon;
    // Skip header
    std::getline(input_stream, line);
    // Read the data
    while (std::getline(input_stream, line)) {
        std::istringstream string_stream(line);
        std::vector<std::string> cells;
        std::string cell;
        while (std::getline(string_stream, cell, ',')) {
            cells.push_back(std::move(cell));
        }
        // Make sure all expected data is there
        if (cells.size() < 14) {
            throw new std::runtime_error("Malformed line: " + line);
        }
        // Parse the data
        uint8_t group_number = std::stoi(cells[0]);
        Pokemon pokemon;
        pokemon.name = cells[1];
        pokemon.item = cells[2];
        pokemon.moves = {cells[3], cells[4], cells[5], cells[6]};
        pokemon.nature = cells[7];
        for (int i = 8; i <= 13; ++i) {
            pokemon.evs.push_back(std::stoi(cells[i]));
        }
        grouped_pokemon[group_number].push_back(std::move(pokemon));
    }
    return grouped_pokemon;
}

void print_battle_hall_pokemon(const GroupedPokemon& data) {
    for (
        const auto& [
            group_number,
            pokemon_group
        ] : data
    ) {
        std::cout << "Group: " << static_cast<int>(group_number) << ":\n";
        std::cout << std::left
            << std::setw(15) << "Name"
            << std::setw(15) << "Item"
            << std::setw(15) << "Move 1"
            << std::setw(15) << "Move 2"
            << std::setw(15) << "Move 3"
            << std::setw(15) << "Move 4"
            << std::setw(15) << "Nature"
            << std::setw(5) << "HP"
            << std::setw(5) << "ATK"
            << std::setw(5) << "DEF"
            << std::setw(5) << "SA"
            << std::setw(5) << "SD"
            << std::setw(5) << "SPD"
            << "\n";
        for (const auto& [
                 name,
                 item,
                 moves,
                 nature,
                 evs
             ] : pokemon_group
        ) {
            std::cout << std::left
                << std::setw(15) << name
                << std::setw(15) << item;
            for (int i = 0; i < 4; ++i) {
                std::cout << std::setw(15) << moves[i];
            }
            std::cout << std::setw(15) << nature;
            for (const auto& ev : evs) {
                std::cout << std::left
                    << std::setw(5) << (std::to_string(ev) + " ");
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
}
