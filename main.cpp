#include <chrono>
#include <iostream>

#include "src/all_v_all.h"
#include "src/battle_hall_data_source.h"
#include "src/config.h"
#include "src/hit_analyzer.h"

int main() {
    const auto start =
        std::chrono::high_resolution_clock::now();

    const auto all_serebii_pokemon =
        get_all_serebii_pokemon();
    test(all_serebii_pokemon);
    // analyze_all(all_serebii_pokemon);

    // const auto all_moves =
    //     get_all_pokemon_moves(
    //         all_serebii_pokemon
    //     );
    // const auto all_battle_hall_pokemon =
    //     get_all_battle_hall_pokemon(
    //         all_moves
    //     );
    // const auto group_to_rank_to_over_2 =
    //     get_all_custom_hall_pokemon(
    //         all_serebii_pokemon,
    //         all_battle_hall_pokemon,
    //         all_moves
    //     );
    // // export_battle_hall_pokemon(
    // //     all_battle_hall_pokemon,
    // //     group_to_rank_to_over_2
    // // );
    //
    // const auto player_pokemon_forms =
    //     get_pokemon_forms(all_serebii_pokemon);
    // for (const auto& pokemon_form :
    //      player_pokemon_forms
    // ) {
    //     if (SKIP_RANKS) {
    //         analyze(
    //             group_to_rank_to_over_2,
    //             pokemon_form,
    //             get_type_to_rank_to_skip()
    //         );
    //     } else {
    //         analyze(group_to_rank_to_over_2, pokemon_form);
    //     }
    // }

    const auto end =
        std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed = end - start;
    std::cout << "analyze() took " << elapsed.count() << " seconds.\n";
    return 0;
}
