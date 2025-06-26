#include <chrono>
#include <iostream>

#include "src/battle_hall_data_source.h"
#include "src/hit_analyzer.h"

int main() {
    const auto start =
        std::chrono::high_resolution_clock::now();

    analyze();

    const auto end =
        std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed = end - start;
    std::cout << "analyze() took " << elapsed.count() << " seconds.\n";
    return 0;
}
