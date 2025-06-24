cmake_minimum_required(VERSION 3.31)
project(gen_iv_battle_hall_analyzer)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Og -fno-inline")

add_executable(gen_iv_battle_hall_team_builder main.cpp
        src/battle_hall_data_source.cpp
        src/battle_hall_data_source.h
        src/serebii_pokemon_data_source.cpp
        src/serebii_pokemon_data_source.h
        src/hit_analyzer.cpp
        src/hit_analyzer.h
        src/moves.h
)

add_custom_command(
        TARGET gen_iv_battle_hall_team_builder POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:gen_iv_battle_hall_team_builder>/data
)