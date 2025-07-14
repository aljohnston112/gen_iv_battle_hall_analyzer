#include <chrono>
#include <iostream>

#include "src/battle_hall_data_source.h"
#include "src/hit_analyzer.h"

int main() {
    const auto start =
        std::chrono::high_resolution_clock::now();

    const auto all_serebii_pokemon =
        get_all_serebii_pokemon();
    const auto all_moves =
        get_all_pokemon_moves(
            all_serebii_pokemon
        );
    const auto group_to_rank_to_over_2 =
        get_all_custom_hall_pokemon(
            all_serebii_pokemon,
            get_all_battle_hall_pokemon(
                all_moves
            ),
            all_moves
        );

    constexpr bool all = false;
    if (all) {
        for (const auto& serebii_pokemon : all_serebii_pokemon |
             std::views::values) {
            const auto& player_pokemon_forms =
                convert_serebii_to_custom(serebii_pokemon);
            analyze(group_to_rank_to_over_2, player_pokemon_forms);
        }
    } else {
        std::unordered_map<PokemonType, int> type_to_rank = {
            {PokemonType::NORMAL, 5},
            {PokemonType::ELECTRIC, 4},
            {PokemonType::GHOST, 5},
            {PokemonType::ICE, 5},
            {PokemonType::DRAGON, 4},
            {PokemonType::STEEL, 2},
            {PokemonType::GRASS, 1},
            {PokemonType::FIGHTING, 2},
            {PokemonType::POISON, 2},
            {PokemonType::WATER, 2},
            {PokemonType::BUG, 2},
            {PokemonType::ROCK, 2},
            {PokemonType::GROUND, 2},
            {PokemonType::DARK, 2},
            {PokemonType::FLYING, 2},
            {PokemonType::PSYCHIC, -1},
            {PokemonType::FIRE, 2}
        };
        auto player_pokemon_forms =
            convert_serebii_to_custom(all_serebii_pokemon.at("Azurill"));
        for (auto& [
                 form,
                 p_list
             ] : player_pokemon_forms
        ) {
            static std::unordered_set<std::string> moves_to_include{
                "Ice Beam",
                "Surf",
                "Return",
                "Waterfall",
                // "Facade"
            };
            for (auto& p : p_list) {
                std::vector<const MoveInfo*> moves{};
                for (const auto& move : p.moves) {
                    if (moves_to_include.contains(move->name)) {
                        moves.push_back(move);
                    }
                }
                p.moves = moves;

                p.stats[0] = 88;
                p.stats[1] = 20;
                p.stats[2] = 40;
                p.stats[3] = 15;
                p.stats[4] = 33;
                p.stats[5] = 19;
                p.item = Item::SitrusBerry;
                p.ability = Ability::HugePower;
            }
        }
        analyze(group_to_rank_to_over_2, player_pokemon_forms, type_to_rank);
    }

    // const auto all_battle_hall_pokemon =
    //     get_all_battle_hall_pokemon(
    //         all_moves
    //     );
    // export_battle_hall_pokemon(
    //     all_battle_hall_pokemon,
    //     get_all_custom_hall_pokemon(
    //         all_serebii_pokemon,
    //         all_battle_hall_pokemon,
    //         all_moves
    //     )
    // );

    const auto end =
        std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed = end - start;
    std::cout << "analyze() took " << elapsed.count() << " seconds.\n";
    return 0;
}
