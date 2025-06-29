#include "serebii_pokemon_data_source.h"

#include <fstream>
#include <iostream>
#include <ranges>

#include "config.h"
#include "nature.h"

std::unordered_map<std::string, PokemonType> pokemon_type_map{
    {"normal", PokemonType::NORMAL},
    {"fighting", PokemonType::FIGHTING},
    {"flying", PokemonType::FLYING},
    {"poison", PokemonType::POISON},
    {"ground", PokemonType::GROUND},
    {"rock", PokemonType::ROCK},
    {"bug", PokemonType::BUG},
    {"ghost", PokemonType::GHOST},
    {"steel", PokemonType::STEEL},
    {"fire", PokemonType::FIRE},
    {"water", PokemonType::WATER},
    {"grass", PokemonType::GRASS},
    {"electric", PokemonType::ELECTRIC},
    {"psychic", PokemonType::PSYCHIC},
    {"ice", PokemonType::ICE},
    {"dragon", PokemonType::DRAGON},
    {"dark", PokemonType::DARK}
};

std::unordered_map<std::string, Category> move_category_map{
    {"physical", Category::PHYSICAL},
    {"special", Category::SPECIAL},
    {"status", Category::STATUS}
};

std::unordered_map<std::string, Stat> stat_map{
    {"health", Stat::HEALTH},
    {"attack", Stat::ATTACK},
    {"defense", Stat::DEFENSE},
    {"special_attack", Stat::SPECIAL_ATTACK},
    {"special_defense", Stat::SPECIAL_DEFENSE},
    {"speed", Stat::SPEED}
};

std::string extract_left_string(const std::string& line) {
    const auto first = line.find('"') + 1;
    const auto colon_i = line.find(':') - 1;
    return line.substr(first, colon_i - first);
}

std::string extract_right_string(const std::string& line) {
    const auto first = line.find('"', line.find(':')) + 1;
    const auto last = line.find('"', first);
    return line.substr(first, last - first);
}

int extract_left_int(const std::string& line) {
    const auto first = line.find('"') + 1;
    const auto colon_i = line.find(':') - 1;
    return std::stoi(line.substr(first, colon_i - first));
}

int extract_right_int(const std::string& line) {
    const auto colon_i = line.find(':');
    return std::stoi(line.substr(colon_i + 1));
}

double extract_right_double(const std::string& line) {
    const auto colon = line.find(':');
    return std::stod(line.substr(colon + 1));
}

MoveInfo parse_move(
    std::ifstream& in
) {
    MoveInfo move_info;
    std::string line;
    while (std::getline(in, line)) {
        if (line.find('}') != std::string::npos) {
            break;
        }
        if (line.find("name") != std::string::npos) {
            move_info.name = extract_right_string(line);
            move_info.move = MOVE_MAP.at(move_info.name);
        } else if (line.find("move_type") != std::string::npos) {
            move_info.type =
                pokemon_type_map[extract_right_string(line)];
        } else if (line.find("category") != std::string::npos) {
            move_info.category =
                move_category_map[extract_right_string(line)];
        } else if (line.find("power") != std::string::npos) {
            move_info.power =
                extract_right_int(line);
        } else if (line.find("accuracy") != std::string::npos) {
            move_info.accuracy =
                extract_right_int(line);
        } else if (line.find("effect_percent") != std::string::npos) {
            move_info.
                effect_percent = extract_right_int(line);
        }
    }
    return move_info;
}

SerebiiPokemon parse_pokemon(std::ifstream& input_stream) {
    SerebiiPokemon serebii_pokemon;
    std::string line;
    bool in_types = false;
    bool in_base = false;
    bool in_lvl = false;
    int current_level = -1;
    while (std::getline(input_stream, line)) {
        if (line.find("\"name\"") != std::string::npos) {
            serebii_pokemon.name = extract_right_string(line);
        } else if (line.find("\"id\"") != std::string::npos) {
            serebii_pokemon.id = extract_right_int(line);
        } else if (line.find("\"ability\"") != std::string::npos) {
            serebii_pokemon.ability = extract_right_string(line);
        } else if (line.find("\"pounds\"") != std::string::npos) {
            serebii_pokemon.pounds = extract_right_double(line);
        } else if (line.find("\"pokemon_types\"") != std::string::npos) {
            in_types = true;
        } else if (in_types) {
            if (line.find(']') != std::string::npos) {
                in_types = false;
            } else {
                const auto start_i = line.find('"') + 1;
                const auto end_i = line.find('"', start_i);
                serebii_pokemon.types.emplace_back(
                    pokemon_type_map[
                        line.substr(start_i, end_i - start_i)
                    ]
                );
                std::sort(
                    serebii_pokemon.types.begin(),
                    serebii_pokemon.types.end()
                );
            }
        } else if (line.find("\"base_stats\"") != std::string::npos) {
            in_base = true;
        } else if (in_base) {
            if (line.find('}') != std::string::npos) {
                in_base = false;
            } else {
                const auto key_start = line.find('"') + 1;
                const auto key_end = line.find('"', key_start);
                Stat key = stat_map[
                    line.substr(key_start, key_end - key_start)
                ];
                const int value = extract_right_int(line);
                serebii_pokemon.base_stats[key] = value;
            }
        } else if (line.find("\"level_to_attacks\"") != std::string::npos &&
            line.find("null") == std::string::npos &&
            line.find("{}") == std::string::npos
        ) {
            in_lvl = true;
        } else if (in_lvl) {
            if (line.find('}') != std::string::npos) {
                in_lvl = false;
            } else if (line.find('[') != std::string::npos) {
                current_level = extract_left_int(line);
                serebii_pokemon.level_to_moves[current_level] = {};
            } else if (line.find('{') != std::string::npos) {
                serebii_pokemon.level_to_moves[current_level].push_back(
                    parse_move(input_stream)
                );
            }
        } else if (
            line.find("\"tm_or_hm_to_attack\"") != std::string::npos &&
            line.find("null") == std::string::npos &&
            line.find("{}") == std::string::npos
        ) {
            while (std::getline(input_stream, line)) {
                if (line.find('}') != std::string::npos) {
                    break;
                }
                if (line.find('{') != std::string::npos) {
                    const size_t q1 = line.find('"');
                    const size_t q2 = line.find('"', q1 + 1);
                    const std::string key =
                        line.substr(q1 + 1, q2 - q1 - 1);
                    serebii_pokemon.tm_or_hm_to_move[key] = parse_move(
                        input_stream);
                }
            }
        } else if (line.find("\"egg_moves\"") != std::string::npos &&
            line.find("null") == std::string::npos &&
            line.find("{}") == std::string::npos
        ) {
            if (line.find("null") != std::string::npos) {
                serebii_pokemon.egg_moves = std::vector<MoveInfo>{};
            } else {
                serebii_pokemon.egg_moves = std::vector<MoveInfo>{};
                while (std::getline(input_stream, line)) {
                    if (line.find(']') != std::string::npos) {
                        break;
                    }
                    if (line.find('{') != std::string::npos) {
                        serebii_pokemon.egg_moves.push_back(
                            parse_move(input_stream)
                        );
                    }
                }
            }
        } else if (line.find("\"pre_evolution_index_to_level_to_moves\"") !=
            std::string::npos &&
            line.find("null") == std::string::npos &&
            line.find("{}") == std::string::npos
        ) {
            if (line.find("null") != std::string::npos) {
                serebii_pokemon.pre_evolution_moves =
                    std::unordered_map<
                        int,
                        std::unordered_map<int, std::vector<MoveInfo>>
                    >{};
            } else {
                serebii_pokemon.pre_evolution_moves = {};
                std::string key_line;
                while (std::getline(input_stream, key_line)) {
                    if (key_line.find('}') != std::string::npos) {
                        break;
                    }
                    if (key_line.find(':') != std::string::npos &&
                        key_line.find('{') != std::string::npos
                    ) {
                        int pre_idx = std::stoi(
                            key_line.substr(key_line.find('"') + 1)
                        );
                        serebii_pokemon.pre_evolution_moves[pre_idx] = {};
                        while (std::getline(input_stream, line)) {
                            if (line.find('}') != std::string::npos) {
                                break;
                            }
                            if (line.find(": [") != std::string::npos) {
                                const auto level = extract_left_int(line);
                                while (std::getline(input_stream, line)) {
                                    if (line.find(']') != std::string::npos) {
                                        break;
                                    }
                                    if (line.find('{') != std::string::npos) {
                                        serebii_pokemon.pre_evolution_moves[
                                            pre_idx][
                                            level
                                        ].push_back(
                                            parse_move(input_stream)
                                        );
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } else if (line.find("\"move_tutor_attacks\"") != std::string::npos &&
            line.find("null") == std::string::npos &&
            line.find("{}") == std::string::npos
        ) {
            while (std::getline(input_stream, line)) {
                if (line.find(']') != std::string::npos) {
                    break;
                }
                if (line.find('{') != std::string::npos) {
                    serebii_pokemon.move_tutor_attacks.push_back(
                        parse_move(input_stream));
                }
            }
        } else if (line.find("\"game_to_level_to_moves\"") != std::string::npos
            &&
            line.find("null") == std::string::npos &&
            line.find("{}") == std::string::npos
        ) {
            while (std::getline(input_stream, line)) {
                if (line.find('}') != std::string::npos) {
                    break;
                }
                const size_t q1 = line.find('"');
                if (q1 == std::string::npos) {
                    continue;
                }
                const size_t q2 = line.find('"', q1 + 1);
                const std::string game = line.substr(q1 + 1, q2 - q1 - 1);
                serebii_pokemon.game_to_level_to_moves[game] = {};
                while (std::getline(input_stream, line)) {
                    if (line.find('}') != std::string::npos) {
                        break;
                    }
                    if (line.find(": [") != std::string::npos) {
                        const int lvl = extract_left_int(line);
                        serebii_pokemon.game_to_level_to_moves[game][lvl] = {};
                        while (std::getline(input_stream, line)) {
                            if (line.find(']') != std::string::npos) {
                                break;
                            }
                            if (line.find('{') != std::string::npos) {
                                serebii_pokemon.game_to_level_to_moves[game][
                                    lvl
                                ].push_back(parse_move(input_stream));
                            }
                        }
                    }
                }
            }
        } else if (line.find("\"special_moves\"") != std::string::npos &&
            line.find("null") == std::string::npos &&
            line.find("{}") == std::string::npos
        ) {
            if (line.find("null") != std::string::npos) {
                serebii_pokemon.special_moves = std::vector<MoveInfo>{};
            } else {
                serebii_pokemon.special_moves = std::vector<MoveInfo>{};
                while (std::getline(input_stream, line)) {
                    if (line.find(']') != std::string::npos) {
                        break;
                    }
                    if (line.find('{') != std::string::npos) {
                        serebii_pokemon.special_moves.push_back(
                            parse_move(input_stream)
                        );
                    }
                }
            }
        } else if (line.find("\"form_to_base_stats\"") != std::string::npos &&
            line.find("null") == std::string::npos &&
            line.find("{}") == std::string::npos
        ) {
            if (line.find("null") != std::string::npos) {
                serebii_pokemon.form_to_base_stats = {};
            } else {
                serebii_pokemon.form_to_base_stats = {};
                std::string form_line;
                while (std::getline(input_stream, form_line)) {
                    if (form_line.find('}') != std::string::npos) {
                        break;
                    }
                    const std::string form = extract_left_string(form_line);
                    serebii_pokemon.form_to_base_stats[form] = {};
                    while (std::getline(input_stream, line)) {
                        if (line.find('}') != std::string::npos) {
                            break;
                        }
                        const Stat stat = stat_map[extract_left_string(line)];
                        const int val = extract_right_int(line);
                        serebii_pokemon.form_to_base_stats[form][stat] = val;
                    }
                }
            }
        } else if (line.find("\"form_to_level_up_attacks\"") !=
            std::string::npos &&
            line.find("null") == std::string::npos &&
            line.find("{}") == std::string::npos
        ) {
            if (line.find("null") != std::string::npos) {
                serebii_pokemon.form_to_level_up_moves = {};
            } else {
                serebii_pokemon.form_to_level_up_moves = {};
                while (std::getline(input_stream, line)) {
                    if (line.find('}') != std::string::npos) {
                        break;
                    }
                    const std::string form = extract_left_string(line);
                    serebii_pokemon.form_to_level_up_moves[form] = {};
                    while (std::getline(input_stream, line)) {
                        if (line.find('}') != std::string::npos) {
                            break;
                        }
                        if (line.find(": [") != std::string::npos) {
                            const int lvl = std::stoi(
                                line.substr(line.find('"') + 1)
                            );
                            serebii_pokemon.form_to_level_up_moves[form][
                                lvl
                            ] = {};
                            while (std::getline(input_stream, line)) {
                                if (line.find(']') != std::string::npos) {
                                    break;
                                }
                                if (line.find('{') != std::string::npos) {
                                    serebii_pokemon.form_to_level_up_moves[
                                        form
                                    ][lvl].push_back(
                                        parse_move(input_stream)
                                    );
                                }
                            }
                        }
                    }
                }
            }
        } else if (line.find("\"form_to_tm_or_hm_to_attack\"") !=
            std::string::npos &&
            line.find("null") == std::string::npos &&
            line.find("{}") == std::string::npos
        ) {
            if (line.find("null") != std::string::npos) {
                serebii_pokemon.form_to_tm_or_hm_to_move = {};
            } else {
                serebii_pokemon.form_to_tm_or_hm_to_move = {};
                while (std::getline(input_stream, line)) {
                    if (line.find('}') != std::string::npos) {
                        break;
                    }
                    const std::string form = extract_left_string(line);
                    serebii_pokemon.form_to_tm_or_hm_to_move[form] = {};
                    while (std::getline(input_stream, line)) {
                        if (line.find('}') != std::string::npos) {
                            break;
                        }
                        const std::string tm = extract_left_string(line);
                        serebii_pokemon.form_to_tm_or_hm_to_move[form][tm] =
                            parse_move(input_stream);
                    }
                }
            }
        } else if (line.find("\"form_to_move_tutor_attacks\"") !=
            std::string::npos &&
            line.find("null") == std::string::npos &&
            line.find("{}") == std::string::npos
        ) {
            if (line.find("null") != std::string::npos) {
                serebii_pokemon.form_to_move_tutor_moves = {};
            } else {
                serebii_pokemon.form_to_move_tutor_moves = {};
                while (std::getline(input_stream, line)) {
                    if (line.find('}') != std::string::npos) {
                        break;
                    }
                    const std::string form = extract_left_string(line);
                    serebii_pokemon.form_to_move_tutor_moves[form] = {};
                    while (std::getline(input_stream, line)) {
                        if (line.find(']') != std::string::npos) {
                            break;
                        }
                        if (line.find('{') != std::string::npos) {
                            serebii_pokemon.form_to_move_tutor_moves[
                                form
                            ].push_back(parse_move(input_stream));
                        }
                    }
                }
            }
        } else if ((line.find("},") != std::string::npos ||
                line.find('}') != std::string::npos) &&
            line.find("{}") == std::string::npos
        ) {
            break;
        }
    }
    return serebii_pokemon;
}

std::unordered_map<std::string, SerebiiPokemon> get_all_serebii_pokemon() {
    std::ifstream input_stream("./data/fresh/all_pokemon.json");
    std::unordered_map<std::string, SerebiiPokemon> pokedex;

    std::string line;
    // Leading {
    std::getline(input_stream, line);
    // Index_number : {
    while (std::getline(input_stream, line)) {
        if (line.find('{') != std::string::npos) {
            const auto pokemon = parse_pokemon(input_stream);
            pokedex[pokemon.name] = pokemon;
        }
    }
    return pokedex;
}

void print_serebii_pokemon() {
    std::unordered_map<std::string, SerebiiPokemon> pokedex =
        get_all_serebii_pokemon();
    for (const auto& p : pokedex | std::views::values) {
        std::cout << p.id << ", " << p.name << "\n";
    }
}

std::unordered_map<std::string, std::unordered_map<Move, const MoveInfo*>>
get_moves_for_serebii_pokemon(
    const SerebiiPokemon& serebii_pokemon
) {
    std::unordered_map<Move, const MoveInfo*> shared_moves;
    for (const auto& moves : serebii_pokemon.level_to_moves |
         std::views::values) {
        for (const auto& move : moves) {
            shared_moves[move.move] = &move;
        }
    }
    for (const auto& move : serebii_pokemon.tm_or_hm_to_move |
         std::views::values) {
        shared_moves[move.move] = &move;
    }
    for (const auto& move : serebii_pokemon.egg_moves) {
        shared_moves[move.move] = &move;
    }
    for (const auto& move : serebii_pokemon.move_tutor_attacks) {
        shared_moves[move.move] = &move;
    }
    for (const auto& level_map : serebii_pokemon.pre_evolution_moves |
         std::views::values) {
        for (const auto& moves : level_map | std::views::values) {
            for (const auto& move : moves) {
                shared_moves[move.move] = &move;
            }
        }
    }
    for (const auto& move : serebii_pokemon.special_moves) {
        shared_moves[move.move] = &move;
    }
    for (const auto& level_map : serebii_pokemon.game_to_level_to_moves |
         std::views::values) {
        for (const auto& moves : level_map | std::views::values) {
            for (const auto& move : moves) {
                shared_moves[move.move] = &move;
            }
        }
    }
    std::unordered_map<
        std::string,
        std::unordered_map<Move, const MoveInfo*>
    > moves_by_form;
    if (serebii_pokemon.name != "Wormadam" &&
        serebii_pokemon.name != "Deoxys" &&
        serebii_pokemon.name != "Shaymin" &&
        serebii_pokemon.name != "Giratina"
    ) {
        moves_by_form["all"] = shared_moves;
    }
    for (const auto& [form_name, level_map] : serebii_pokemon.
         form_to_level_up_moves) {
        auto& form_moves = moves_by_form[form_name];
        for (const auto& move : shared_moves) {
            form_moves.insert(move);
        }
        for (const auto& moves : level_map | std::views::values) {
            for (const auto& move : moves) {
                form_moves[move.move] = &move;
            }
        }
    }
    for (const auto& [form_name, tm_map] : serebii_pokemon.
         form_to_tm_or_hm_to_move) {
        auto& form_moves = moves_by_form[form_name];
        for (const auto& move : shared_moves) {
            form_moves.insert(move);
        }
        for (const auto& move : tm_map | std::views::values) {
            form_moves[move.move] = &move;
        }
    }
    for (const auto& [form_name, moves] : serebii_pokemon.
         form_to_move_tutor_moves) {
        auto& form_moves = moves_by_form[form_name];
        for (const auto& move : shared_moves) {
            form_moves.insert(move);
        }
        for (const auto& move : moves) {
            form_moves[move.move] = &move;
        }
    }
    return moves_by_form;
}

std::vector<const MoveInfo*> get_all_pokemon_moves(
    const std::unordered_map<std::string, SerebiiPokemon>& pokedex
) {
    std::vector<const MoveInfo*> all_moves{
        static_cast<int>(Move::Count),
        nullptr
    };
    for (const auto& serebii_pokemon :
         pokedex | std::ranges::views::values
    ) {
        std::unordered_map<
            std::string,
            std::unordered_map<Move, const MoveInfo*>
        > form_to_moves =
            get_moves_for_serebii_pokemon(serebii_pokemon);
        for (const auto& moves : form_to_moves |
             std::ranges::views::values
        ) {
            for (const auto& [move_enum, move] : moves) {
                all_moves[static_cast<int>(move_enum)] = move;
            }
        }
    }
    return all_moves;
}

std::array<uint16_t, static_cast<int>(Stat::NO_STAT)> get_stats_for_serebii(
    const std::string& form,
    const SerebiiPokemon& serebii_pokemon
) {
    std::unordered_map<Stat, uint16_t> base_stats;
    if (form == "all" ||
        form == "Plant Cloak" ||
        form == "Normal Forme" ||
        form == "Land Forme" ||
        form == "Altered Forme"
    ) {
        base_stats = serebii_pokemon.base_stats;
    } else {
        base_stats = serebii_pokemon.form_to_base_stats.at(form);
    }
    std::array<uint16_t, static_cast<int>(Stat::NO_STAT)> stats{};
    for (const auto& [stat, value] : base_stats) {
        stats[static_cast<int>(stat)] = get_stat(
            LEVEL,
            stat,
            value,
            0,
            0,
            NATURE_MAP.at(NatureEnum::HARDY)
        );
    }
    return stats;
}

const std::unordered_set IGNORED_MOVES = {
    Move::Snore,
    Move::SolarBeam,
};

std::unordered_map<
    std::string,
    std::unordered_set<const MoveInfo*>
> gather_moves(
    const std::unordered_map<
        std::string,
        std::unordered_map<Move, const MoveInfo*>
    > form_moves,
    const bool is_player
) {
    std::unordered_map<
        std::string,
        std::unordered_set<const MoveInfo*>
    > forms_moves;
    for (const auto& [form, move_map] : form_moves) {
        std::unordered_map<
            Category,
            std::array<const MoveInfo*, static_cast<int>(PokemonType::COUNT)>
        > moves;
        const uint8_t min_accuracy = is_player ? 100 : 0;
        for (const auto& move : move_map | std::views::values) {
            if (move->power == 0 ||
                move->accuracy < min_accuracy ||
                IGNORED_MOVES.contains(move->move)
            ) {
                continue;
            }

            // if (move.power == -1) {
            //     printf(move.name.c_str());
            //     printf("\n");
            //     exit(1);
            //     continue;
            // }
            auto& type_map = moves[move->category];
            if (const auto current_move =
                    type_map[static_cast<int>(move->type)];
                current_move == nullptr ||
                current_move != nullptr && move->power > current_move->power
            ) {
                type_map[static_cast<int>(move->type)] = move;
            }
        }
        for (const auto array : moves | std::views::values) {
            for (const MoveInfo* move : array) {
                if (move != nullptr) {
                    forms_moves[form].emplace(move);
                }
            }
        }
    }
    return forms_moves;
}

std::unordered_map<
    std::string,
    std::vector<CustomPokemon>
> convert_serebii_to_custom(
    const SerebiiPokemon& serebii_pokemon
) {
    std::unordered_map<std::string, std::vector<CustomPokemon>> customs;
    const auto moves =
        get_moves_for_serebii_pokemon(serebii_pokemon);
    const auto serebii_moves =
        gather_moves(
            moves,
            true
        );
    for (const auto& [
             form,
             form_moves
         ] : serebii_moves
    ) {
        std::vector<const MoveInfo*> move_vector{};
        for (const auto& move : form_moves) {
            move_vector.push_back(move);
        }
        auto name = serebii_pokemon.name;
        auto types = serebii_pokemon.types;
        if (name == "Wormadam") {
            if (form == "Plant Cloak") {
                name = "WormadamP";
                types = {PokemonType::BUG, PokemonType::GRASS};
            } else if (form == "Sandy Cloak") {
                name = "WormadamS";
                types = {PokemonType::GROUND, PokemonType::GRASS};
            } else if (form == "Trash Cloak") {
                name = "WormadamT";
                types = {PokemonType::STEEL, PokemonType::GRASS};
            }
        } else if (name == "Deoxys") {
            if (form == "Normal Forme") {
                name = "NormalDeoxys";
            } else if (form == "Attack Forme") {
                name = "AttackDeoxys";
            } else if (form == "Defense Forme") {
                name = "DefenseDeoxys";
            } else if (form == "Speed Forme") {
                name = "SpeedDeoxys";
            }
        } else if (name == "Shaymin") {
            if (form == "Land Forme") {
                name = "LandShaymin";
                types = {PokemonType::GRASS};
            } else if (form == "Sky Forme") {
                name = "SkyShaymin";
                types = {PokemonType::GRASS, PokemonType::FLYING};
            }
        } else if (name == "Giratina") {
            if (form == "Altered Forme") {
                name = "AlteredGiratina";
            } else if (form == "Origin Forme") {
                name = "OriginGiratina";
            }
        }
        const Pokemon name_enum = STRING_TO_POKEMON.at(name);
        const std::vector<Ability> abilities = ABILITY_MAP.at(name_enum);
        std::vector<CustomPokemon> pokemon{};
        for (const auto& ability : abilities) {
            pokemon.emplace_back(
                CustomPokemon{
                    .name = name_enum,
                    .ability = ability,
                    .level = LEVEL,
                    .item = STRING_TO_ITEM.at(""),
                    .types = types,
                    .moves = move_vector,
                    .stats = get_stats_for_serebii(form, serebii_pokemon)
                }
            );
        }
        customs[form] = pokemon;
    }
    return customs;
}
