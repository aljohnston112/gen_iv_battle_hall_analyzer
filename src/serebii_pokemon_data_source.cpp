#include "serebii_pokemon_data_source.h"

#include <fstream>
#include <iostream>
#include <ranges>

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

Attack parse_attack(std::ifstream& in) {
    Attack attack;
    std::string line;
    while (std::getline(in, line)) {
        if (line.find('}') != std::string::npos) {
            break;
        }
        if (line.find("name") != std::string::npos) {
            attack.name = extract_right_string(line);
        } else if (line.find("move_type") != std::string::npos) {
            attack.move_type =
                pokemon_type_map[extract_right_string(line)];
        } else if (line.find("category") != std::string::npos) {
            attack.category =
                move_category_map[extract_right_string(line)];
        } else if (line.find("power") != std::string::npos) {
            attack.power =
                extract_right_int(line);
        } else if (line.find("accuracy") != std::string::npos) {
            attack.accuracy =
                extract_right_int(line);
        } else if (line.find("effect_percent") != std::string::npos) {
            attack.
                effect_percent = extract_right_int(line);
        }
    }
    return attack;
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
                serebii_pokemon.types.push_back(
                    pokemon_type_map[
                        line.substr(start_i, end_i - start_i)
                    ]
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
            } else if (line.find("[") != std::string::npos) {
                current_level = extract_left_int(line);
                serebii_pokemon.level_attacks[current_level] = {};
            } else if (line.find("{") != std::string::npos) {
                serebii_pokemon.level_attacks[current_level].push_back(
                    parse_attack(input_stream)
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
                    serebii_pokemon.tm_or_hm_attacks[key] = parse_attack(
                        input_stream);
                }
            }
        } else if (line.find("\"egg_moves\"") != std::string::npos &&
            line.find("null") == std::string::npos &&
            line.find("{}") == std::string::npos
        ) {
            if (line.find("null") != std::string::npos) {
                serebii_pokemon.egg_moves = std::vector<Attack>{};
            } else {
                serebii_pokemon.egg_moves = std::vector<Attack>{};
                while (std::getline(input_stream, line)) {
                    if (line.find(']') != std::string::npos) {
                        break;
                    }
                    if (line.find('{') != std::string::npos) {
                        serebii_pokemon.egg_moves.push_back(
                            parse_attack(input_stream)
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
                serebii_pokemon.pre_evo_moves =
                    std::unordered_map<
                        int,
                        std::unordered_map<int, std::vector<Attack>>
                    >{};
            } else {
                serebii_pokemon.pre_evo_moves = {};
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
                        serebii_pokemon.pre_evo_moves[pre_idx] = {};
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
                                        serebii_pokemon.pre_evo_moves[pre_idx][
                                            level
                                        ].push_back(
                                            parse_attack(input_stream)
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
                        parse_attack(input_stream));
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
                                ].push_back(parse_attack(input_stream));
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
                serebii_pokemon.special_moves = std::vector<Attack>{};
            } else {
                serebii_pokemon.special_moves = std::vector<Attack>{};
                while (std::getline(input_stream, line)) {
                    if (line.find(']') != std::string::npos) {
                        break;
                    }
                    if (line.find('{') != std::string::npos) {
                        serebii_pokemon.special_moves.push_back(
                            parse_attack(input_stream)
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
                serebii_pokemon.form_to_level_up_attacks = {};
            } else {
                serebii_pokemon.form_to_level_up_attacks = {};
                while (std::getline(input_stream, line)) {
                    if (line.find('}') != std::string::npos) {
                        break;
                    }
                    const std::string form = extract_left_string(line);
                    serebii_pokemon.form_to_level_up_attacks[form] = {};
                    while (std::getline(input_stream, line)) {
                        if (line.find('}') != std::string::npos) {
                            break;
                        }
                        if (line.find(": [") != std::string::npos) {
                            const int lvl = std::stoi(
                                line.substr(line.find('"') + 1)
                            );
                            serebii_pokemon.form_to_level_up_attacks[form][
                                lvl
                            ] = {};
                            while (std::getline(input_stream, line)) {
                                if (line.find(']') != std::string::npos) {
                                    break;
                                }
                                if (line.find('{') != std::string::npos) {
                                    serebii_pokemon.form_to_level_up_attacks[
                                        form
                                    ][lvl].push_back(
                                        parse_attack(input_stream)
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
                serebii_pokemon.form_to_tm_or_hm_to_attack = {};
            } else {
                serebii_pokemon.form_to_tm_or_hm_to_attack = {};
                while (std::getline(input_stream, line)) {
                    if (line.find('}') != std::string::npos) {
                        break;
                    }
                    const std::string form = extract_left_string(line);
                    serebii_pokemon.form_to_tm_or_hm_to_attack[form] = {};
                    while (std::getline(input_stream, line)) {
                        if (line.find('}') != std::string::npos) {
                            break;
                        }
                        const std::string tm = extract_left_string(line);
                        serebii_pokemon.form_to_tm_or_hm_to_attack[form][tm] =
                            parse_attack(input_stream);
                    }
                }
            }
        } else if (line.find("\"form_to_move_tutor_attacks\"") !=
            std::string::npos &&
            line.find("null") == std::string::npos &&
            line.find("{}") == std::string::npos
        ) {
            if (line.find("null") != std::string::npos) {
                serebii_pokemon.form_to_move_tutor_attacks = {};
            } else {
                serebii_pokemon.form_to_move_tutor_attacks = {};
                while (std::getline(input_stream, line)) {
                    if (line.find('}') != std::string::npos) {
                        break;
                    }
                    const std::string form = extract_left_string(line);
                    serebii_pokemon.form_to_move_tutor_attacks[form] = {};
                    while (std::getline(input_stream, line)) {
                        if (line.find(']') != std::string::npos) {
                            break;
                        }
                        if (line.find('{') != std::string::npos) {
                            serebii_pokemon.form_to_move_tutor_attacks[
                                form
                            ].push_back(parse_attack(input_stream));
                        }
                    }
                }
            }
        } else if ((line.find("},") != std::string::npos ||
                line.find("}") != std::string::npos) &&
            line.find("{}") == std::string::npos
        ) {
            break;
        }
    }
    return serebii_pokemon;
}

std::unordered_map<std::string, SerebiiPokemon> get_serebii_pokemon() {
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
        get_serebii_pokemon();
    for (const auto& p : pokedex | std::views::values) {
        std::cout << p.id << ", " << p.name << "\n";
    }
}
