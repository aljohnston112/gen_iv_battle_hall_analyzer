#include "battle_hall_data_source.h"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "config.h"
#include "custom_pokemon.h"

std::string trim(const std::string& string) {
    auto start = string.begin();
    while (start != string.end() && std::isspace(*start)) {
        ++start;
    }
    auto end = string.end();
    do {
        --end;
    } while (std::distance(start, end) > 0 && std::isspace(*end));
    return {start, end + 1};
}

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

BattleHallPokemon parse_pokemon_line(
    const std::vector<const MoveInfo*>& all_moves,
    const std::string& line
) {
    std::istringstream string_stream(line);
    BattleHallPokemon pokemon;
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
        if (token != "- ") {
            pokemon.moves.push_back(
                all_moves.at(static_cast<int>(MOVE_MAP.at(trim(token))))
            );
        }
    }
    // Get nature
    std::string nature;
    std::getline(string_stream, nature, '\t');
    pokemon.nature = NATURE_MAP.at(STRING_TO_NATURE.at(trim(nature)));
    // Get EVs
    pokemon.evs.reserve(NUMBER_OF_EVS);
    for (int i = 0; i < NUMBER_OF_EVS; ++i) {
        std::getline(string_stream, token, '\t'); // EV
        if (token == "- " || token == "-") {
            pokemon.evs[static_cast<Stat>(i)] = 0;
        } else {
            pokemon.evs[static_cast<Stat>(i)] = std::stoi(token);
        }
    }
    return pokemon;
}

AllBattleHallPokemon parse_file(
    const std::string& filename,
    const std::vector<const MoveInfo*>& all_moves
) {
    std::ifstream file(filename);
    std::string line;
    uint8_t current_group_number;
    AllBattleHallPokemon grouped_pokemon;

    while (std::getline(file, line)) {
        if (line.rfind("Group", 0) == 0) {
            current_group_number = std::stoi(line.substr(6));
            grouped_pokemon[current_group_number] = {};
        } else if (!line.empty()) {
            grouped_pokemon[current_group_number].emplace_back(
                parse_pokemon_line(all_moves, line)
            );
        }
    }
    return grouped_pokemon;
}

void write_battle_hall_data(
    const std::string& file_name,
    const AllBattleHallPokemon& grouped_pokemon
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
            const auto moves_size = moves.size();
            for (int i = 0; i < 4; i++) {
                if (i < moves_size) {
                    out << moves[i]->name << ',';
                } else {
                    out << "-,";
                }
            }
            out << NATURE_TO_STRING.at(nature.nature) << ',';
            for (size_t i = 0; i < evs.size() - 1; i++) {
                out << std::to_string(evs.at(static_cast<Stat>(i))) << ',';
            }
            out << std::to_string(
                evs.at(static_cast<Stat>(evs.size() - 1))
            ) << '\n';
        }
    }
}

AllBattleHallPokemon get_all_battle_hall_pokemon(
    const std::vector<const MoveInfo*>& all_moves
) {
    const auto file = "./data/fresh/battle_hall_pokemon";
    if (!std::filesystem::exists(file)) {
        write_battle_hall_data(
            file,
            parse_file(
                "./data/raw/battle_hall_pokemon",
                all_moves
            )
        );
    }
    std::ifstream input_stream(file);
    std::string line;
    AllBattleHallPokemon grouped_pokemon;
    // Skip header
    std::getline(input_stream, line);
    // Read the data
    while (std::getline(input_stream, line)) {
        std::istringstream string_stream(line);
        std::vector<std::string> cells;
        std::string cell;
        while (std::getline(string_stream, cell, ',')) {
            cells.push_back(trim(cell));
        }
        // Make sure all expected data is there
        if (cells.size() < 14) {
            throw std::runtime_error{"Malformed line: " + line};
        }
        // Parse the data
        uint8_t group_number = std::stoi(cells[0]);
        BattleHallPokemon pokemon;
        pokemon.name = cells[1];
        pokemon.item = cells[2];
        pokemon.moves = std::vector<const MoveInfo*>{};
        for (int i = 3; i <= 6; ++i) {
            if (cells[i] != "-") {
                pokemon.moves.push_back(
                    all_moves.at(static_cast<int>(MOVE_MAP.at(cells[i])))
                );
            }
        }
        std::string nature = cells[7];
        pokemon.nature = NATURE_MAP.at(STRING_TO_NATURE.at(nature));
        for (int i = 8; i <= 13; ++i) {
            pokemon.evs[static_cast<Stat>(i - 8)] = (std::stoi(cells[i]));
        }
        grouped_pokemon[group_number].push_back(std::move(pokemon));
    }
    return grouped_pokemon;
}

void print_all_battle_hall_pokemon(const AllBattleHallPokemon& data) {
    for (const auto& [
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
                std::cout << std::setw(15) << moves[i]->name;
            }
            std::cout << std::setw(15) << NATURE_TO_STRING.at(nature.nature);
            for (const auto& ev : evs) {
                std::cout << std::left
                    << std::setw(5) << (std::to_string(ev.second) + " ");
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
}

std::array<uint16_t, static_cast<int>(Stat::NO_STAT)>
get_stats_for_battle_hall_pokemon(
    const std::unordered_map<std::string, SerebiiPokemon>& all_serebii_pokemon,
    const uint8_t level,
    const BattleHallPokemon& hall_pokemon
) {
    std::string name = hall_pokemon.name;
    if (name.contains("Wormadam")) {
        name = "Wormadam";
    }
    const auto& serebii_pokemon = all_serebii_pokemon.at(name);
    std::unordered_map<Stat, uint16_t> base_stats =
        serebii_pokemon.base_stats;
    if (!serebii_pokemon.form_to_base_stats.empty()) {
        if (name.contains("Wormadam")) {
            if (hall_pokemon.name == "WormadamT") {
                base_stats =
                    serebii_pokemon.form_to_base_stats.at("Trash Cloak");
            } else if (hall_pokemon.name == "WormadamS") {
                base_stats =
                    serebii_pokemon.form_to_base_stats.at("Sandy Cloak");
            } else if (hall_pokemon.name != "WormadamP") {
                throw std::runtime_error{""};
            }
        } else if (hall_pokemon.name != "Rotom") {
            throw std::runtime_error{""};
        }
    }
    std::array<uint16_t, static_cast<int>(Stat::NO_STAT)> stats{};
    for (const auto& [stat, value] : base_stats) {
        stats[static_cast<int>(stat)] = get_stat(
            level,
            stat,
            value,
            31,
            hall_pokemon.evs.at(stat),
            hall_pokemon.nature
        );
    }
    return stats;
}

std::vector<CustomPokemon> convert_hall_to_custom(
    const std::unordered_map<std::string, SerebiiPokemon>& all_serebii_pokemon,
    const BattleHallPokemon& hall_pokemon,
    const uint8_t level
) {
    auto serebii_name = hall_pokemon.name;
    if (serebii_name == "WormadamP" ||
        serebii_name == "WormadamS" ||
        serebii_name == "WormadamT"
    ) {
        serebii_name = "Wormadam";
    }
    const auto name = hall_pokemon.name;
    const Pokemon name_enum = STRING_TO_POKEMON.at(name);
    const std::vector<Ability> abilities = ABILITY_MAP.at(name_enum);
    std::vector<CustomPokemon> pokemon{};
    for (const auto& ability : abilities) {
        auto types = all_serebii_pokemon.at(serebii_name).types;
        if (name_enum == Pokemon::WormadamP) {
            types = { PokemonType::BUG, PokemonType::GRASS };
        } else if (name_enum == Pokemon::WormadamS) {
            types = { PokemonType::BUG, PokemonType::GROUND };
        }  else if (name_enum == Pokemon::WormadamT) {
            types = { PokemonType::BUG, PokemonType::GRASS };
        }

        if (serebii_name == "Wormadam"
        ) {
            printf("");
        }
        pokemon.emplace_back(
            CustomPokemon{
                .name = name_enum,
                .ability = ability,
                .level = level,
                .item = STRING_TO_ITEM.at(hall_pokemon.item),
                .types = types,
                .moves = hall_pokemon.moves,
                .stats = get_stats_for_battle_hall_pokemon(
                    all_serebii_pokemon,
                    level,
                    hall_pokemon
                )
            }
        );
    }
    return pokemon;
}

uint8_t get_hall_pokemon_level(
    const uint8_t types_past_2,
    const uint8_t rank
) {
    const double level_sqrt = std::sqrt(LEVEL);
    const double base_level = LEVEL - (3 * level_sqrt);
    const double increment = level_sqrt / 5.0;
    return std::min(
        LEVEL,
        static_cast<uint8_t>(
            std::ceil(
                base_level + (types_past_2 / 2.0) + (rank - 1) * increment
            )
        )
    );
}

void write_all_hall_pokemon_as_custom(
    const std::unordered_map<std::string, SerebiiPokemon>& all_serebii_pokemon,
    const AllBattleHallPokemon& all_battle_hall_pokemon
) {
    for (const auto& [
             group_number,
             group_pokemon
         ] : all_battle_hall_pokemon
    ) {
        std::unordered_map<
            uint8_t,
            std::unordered_map<uint8_t, std::vector<CustomPokemon>>
        > converted;
        for (const auto& ranks =
                 GROUP_TO_RANKS.at(group_number);
             const uint8_t rank : ranks
        ) {
            for (uint8_t types_past_2 = 0;
                 types_past_2 <= NUMBER_OF_TYPES;
                 types_past_2++
            ) {
                const auto level = get_hall_pokemon_level(
                    types_past_2,
                    rank
                );
                for (const auto& hall_pokemon : group_pokemon) {
                    const auto customs = convert_hall_to_custom(
                        all_serebii_pokemon,
                        hall_pokemon,
                        level
                    );
                    for (const auto& custom : customs) {
                        converted[rank][types_past_2].emplace_back(custom);
                    }
                }
            }
        }
        for (const auto& [rank, types_past_2_map] : converted) {
            for (const auto& [
                     types_past_2,
                     pokemon_list
                 ] : types_past_2_map
            ) {
                std::string filename = std::format(
                    "./data/fresh/hall_group_{:02}_{:02}_{:02}.txt",
                    group_number,
                    rank,
                    types_past_2
                );
                save_custom_pokemon(pokemon_list, filename);
            }
        }
    }
}

std::unordered_map<
    uint8_t,
    std::unordered_map<
        uint8_t, std::unordered_map<uint8_t, std::vector<CustomPokemon>>
    >
> get_all_custom_hall_pokemon(
    const std::unordered_map<std::string, SerebiiPokemon>& all_serebii_pokemon,
    const AllBattleHallPokemon& all_battle_hall_pokemon,
    const std::vector<const MoveInfo*>& all_moves
) {
    namespace fs = std::filesystem;
    std::unordered_map<
        uint8_t,
        std::unordered_map<
            uint8_t, std::unordered_map<uint8_t, std::vector<CustomPokemon>>
        >
    > result;

    bool wrote = false;
    for (uint8_t group_number = 1; group_number < 5; group_number++) {
        for (const auto& ranks =
                 GROUP_TO_RANKS.at(group_number);
             const uint8_t rank : ranks
        ) {
            for (uint8_t types_past_2 = 0;
                 types_past_2 <= NUMBER_OF_TYPES;
                 types_past_2++
            ) {
                std::string filename = std::format(
                    "./data/fresh/hall_group_{:02}_{:02}_{:02}.txt",
                    group_number,
                    rank,
                    types_past_2
                );
                if (!std::filesystem::exists(filename) && !wrote) {
                    write_all_hall_pokemon_as_custom(
                        all_serebii_pokemon,
                        all_battle_hall_pokemon
                    );
                    wrote = true;
                } else if (!std::filesystem::exists(filename) && wrote) {
                    throw std::runtime_error("Not all files were generated");
                }
                std::vector<CustomPokemon> loaded = load_custom_pokemon(
                    filename,
                    all_moves
                );
                result[group_number][rank][types_past_2] = std::move(loaded);
            }
        }
    }

    return result;
}
