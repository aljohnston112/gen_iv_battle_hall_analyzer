cmake_minimum_required(VERSION 3.31)
project(gen_iv_battle_hall_analyzer)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=mold")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=mold")

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
        set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
        set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fno-inline -fno-omit-frame-pointer -fstack-check -fstack-protector-all -Wframe-larger-than=8388608")
option(ENABLE_LIBSTDCXX_DEBUG "Enable _GLIBCXX_DEBUG for debug builds" OFF)
if (ENABLE_LIBSTDCXX_DEBUG)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")
endif()
option(ENABLE_ADDRESS_SANITIZER "Enable address sanitizer" OFF)
#if(ENABLE_ADDRESS_SANITIZER)
#    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined")
#endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto -DNDEBUG")

find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fopenmp")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fopenmp")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
add_executable(gen_iv_battle_hall_team_builder main.cpp
        src/battle_hall_data_source.cpp
        src/battle_hall_data_source.h
        src/serebii_pokemon_data_source.cpp
        src/serebii_pokemon_data_source.h
        src/moves.h
        src/custom_pokemon.cpp
        src/custom_pokemon.h
        src/nature.cpp
        src/BattleState.cpp
        src/BattleState.h
        src/abilities.h
        src/type_effectiveness.h
        src/type_effectiveness.cpp
        src/Items.h
        src/Pokemon.h
        src/PokemonState.h
        external/libhungarian/hungarian.c
        external/libhungarian/hungarian.h
        src/round_robin.h
        src/thread_pool.cpp
        src/thread_pool.h
)
target_include_directories(gen_iv_battle_hall_team_builder PRIVATE ${CMAKE_SOURCE_DIR}/external/libhungarian)


add_custom_command(
        TARGET gen_iv_battle_hall_team_builder POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:gen_iv_battle_hall_team_builder>/data
)