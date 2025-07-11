#include <chrono>
#include <iostream>

#include "src/battle_hall_data_source.h"
#include "src/hit_analyzer.h"

int main() {
    const auto start =
        std::chrono::high_resolution_clock::now();

    analyze();

    const auto all_serebii_pokemon =
        get_all_serebii_pokemon();
    const auto all_moves =
        get_all_pokemon_moves(
            all_serebii_pokemon
        );
    const auto all_battle_hall_pokemon =
        get_all_battle_hall_pokemon(
            all_moves
        );
    export_battle_hall_pokemon(
        all_battle_hall_pokemon,
        get_all_custom_hall_pokemon(
            all_serebii_pokemon,
            all_battle_hall_pokemon,
            all_moves
        )
    );

    const auto end =
        std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed = end - start;
    std::cout << "analyze() took " << elapsed.count() << " seconds.\n";
    return 0;
}
