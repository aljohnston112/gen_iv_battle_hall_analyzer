#ifndef BATTLE_SIMULATOR_H
#define BATTLE_SIMULATOR_H

#include <future>
#include <memory>

#include "custom_pokemon.h"
#include "PokemonState.h"

struct PokemonPairHash {
    std::size_t operator()(const std::pair<Pokemon, Ability>& p) const {
        return (static_cast<int>(p.first) << bits_for_ability) |
            static_cast<int>(p.second);
    }
};

struct MoveDamagePairHash {
    std::size_t operator()(
        const std::pair<const MoveInfo*, int>& p
    ) const noexcept {
        const std::size_t h1 = std::hash<const MoveInfo*>{}(p.first);
        const std::size_t h2 = std::hash<int>{}(p.second);
        return h1 ^ (h2 << 8);
    }
};

struct BattleEntry {
    size_t player_index;
    CustomPokemon player;
    size_t opponent_index;
    CustomPokemon opponent;
};

struct BattleResultEntry {
    size_t player_index;
    size_t opponent_index;
    bool won;
    std::vector<
        std::tuple<Move, int, int>
    > player_moves;
    std::vector<
        std::tuple<Move, int, int>
    > opponent_moves;

    BattleResultEntry(
        const size_t player_,
        const size_t opponent_,
        const bool won_,
        std::vector<std::tuple<Move, int, int>> player_moves_,
        std::vector<std::tuple<Move, int, int>> opponent_moves_
    )
        : player_index(player_),
          opponent_index(opponent_),
          won(won_),
          player_moves(std::move(player_moves_)),
          opponent_moves(std::move(opponent_moves_)) {}

    BattleResultEntry(const BattleResultEntry& other) :
        won(other.won),
        player_moves(other.player_moves),
        opponent_moves(other.opponent_moves) {
        player_index = other.player_index;
        opponent_index = other.opponent_index;
    }

    BattleResultEntry& operator=(const BattleResultEntry& other) {
        if (this == &other) return *this;

        won = other.won;
        player_moves = other.player_moves;
        opponent_moves = other.opponent_moves;

        player_index = other.player_index;
        opponent_index = other.opponent_index;
        return *this;
    }

    BattleResultEntry(BattleResultEntry&&) = default;
    BattleResultEntry& operator=(BattleResultEntry&&) = default;
};

BattleResultEntry battle(
    size_t player_index,
    CustomPokemon* player,
    size_t opponent_index,
    CustomPokemon* opponent
);

inline void battle_all(
    BattleEntry& battle_entry,
    std::promise<BattleResultEntry>&& promise
) {
    promise.set_value(
        battle(
            battle_entry.player_index,
            &battle_entry.player,
            battle_entry.opponent_index,
            &battle_entry.opponent
        )
    );
}

inline void check_download(PokemonState& state0, PokemonState& state1) {
    if (const auto ability0 = state0.get_ability();
        ability0 == Ability::Download
    ) {
        if (state1.get_special_defense(Weather::CLEAR, ability0) >
            state1.get_defense(Weather::CLEAR, ability0)
        ) {
            state0.change_stat_stage(
                Stat::ATTACK,
                1,
                false
            );
        } else {
            state0.change_stat_stage(
                Stat::SPECIAL_ATTACK,
                1,
                false
            );
        }
    }
}

inline void check_intimidate(
    const PokemonState& state0,
    PokemonState& state1
) {
    if (state0.get_ability() == Ability::Intimidate) {
        state1.change_stat_stage(Stat::ATTACK, -1, true);
    }
}

inline void check_mold_breaker(
    const PokemonState& state0,
    PokemonState& state1
) {
    if (state0.get_ability() == Ability::MoldBreaker &&
        ability_is_ignorable(state1.get_ability())
    ) {
        state1.disable_ability();
    }
}

inline void check_trace(
    PokemonState& state0,
    const PokemonState& state1
) {
    if (state0.get_ability() == Ability::Trace) {
        state0.set_ability(state1.get_ability(), true);
    }
}

class BattleState {
    size_t player_index;
    PokemonState player_state;
    size_t opponent_index;
    PokemonState opponent_state;

    bool first_turn = true;
    bool mid_turn = false;

    Weather weather = Weather::CLEAR;
    int weather_turns = 0;

    [[nodiscard]] bool is_first_turn() const {
        return first_turn;
    }

    [[nodiscard]] bool is_mid_turn() const {
        return mid_turn;
    }

    void set_mid_turn() {
        mid_turn = true;
    }

    void end_turn() {
        first_turn = false;
        mid_turn = false;
        if (weather_turns != -1 && weather_turns != 0) {
            weather_turns--;
            if (weather_turns == 0) {
                weather = Weather::CLEAR;
            }
        }
    }

    void check_forecast(PokemonState& pokemon_state) const {
        if (const auto ability = pokemon_state.get_ability();
            ability == Ability::Forecast
        ) {
            switch (weather) {
            case Weather::SUN:
                pokemon_state.change_type(PokemonType::FIRE);
                break;
            case Weather::RAIN:
                pokemon_state.change_type(PokemonType::WATER);
                break;
            case Weather::HAIL:
                pokemon_state.change_type(PokemonType::ICE);
                break;
            default:
                pokemon_state.change_type(PokemonType::NORMAL);
            }
        }
    }

    [[nodiscard]] Weather get_weather() const {
        const auto player_ability = player_state.get_ability();
        const auto opponent_ability = opponent_state.get_ability();
        if (player_ability != Ability::AirLock &&
            opponent_ability != Ability::AirLock &&
            player_ability != Ability::CloudNine &&
            opponent_ability == Ability::CloudNine
        ) {
            return Weather::CLEAR;
        }
        return weather;
    }

    void check_drizzle(
        const PokemonState& pokemon_state
    ) {
        if (pokemon_state.get_ability() == Ability::Drizzle) {
            set_weather(Weather::RAIN, -1);
        }
    }

    void check_drought(
        const PokemonState& pokemon_state
    ) {
        if (pokemon_state.get_ability() == Ability::Drought) {
            set_weather(Weather::SUN, -1);
        }
    }

    void check_sandstorm(
        const PokemonState& pokemon_state
    ) {
        if (pokemon_state.get_ability() == Ability::SandStream) {
            set_weather(Weather::SANDSTORM, -1);
        }
    }

    void check_snow_warning(
        const PokemonState& pokemon_state
    ) {
        if (pokemon_state.get_ability() == Ability::SnowWarning) {
            set_weather(Weather::HAIL, -1);
        }
    }

    /**
     * @param weather
     * @param turns -1 for weather that should not end.
     */
    void set_weather(
        const Weather weather,
        const int turns
    ) {
        this->weather = weather;
        this->weather_turns = turns;
        check_forecast(player_state);
        check_forecast(opponent_state);
    }

    BestMove choose_move_against_defender(
        bool attacker_is_player,
        bool chosen_move_only,
        Weather weather,
        bool is_mid_turn,
        bool checking_future
    );
    void execute_move(
        bool attacker_is_player,
        Weather weather,
        bool is_mid_turn
    );
    void apply_post_move_effects(
        PokemonState& attacker_state,
        const BestMove& attacker_move,
        PokemonState& defender_state
    ) const;

    void apply_end_of_turn_effects() {
        player_state.apply_end_of_turn_effects(
            get_weather(),
            opponent_state
        );
        opponent_state.apply_end_of_turn_effects(
            get_weather(),
            player_state
        );
    }

    [[nodiscard]] bool is_battle_over() const {
        return player_state.get_health() <= 0 ||
            opponent_state.get_health() <= 0;
    }

public:
    BattleState(
        size_t player_index,
        CustomPokemon* player_pokemon,
        size_t opponent_index,
        CustomPokemon* opponent_pokemon
    ) :
        player_index{player_index},
        player_state{player_pokemon, true},
        opponent_index{opponent_index},
        opponent_state{opponent_pokemon, false} {}

    void check_abilities(const bool player_goes_first) {
        // May suppress abilities
        check_mold_breaker(player_state, opponent_state);
        check_mold_breaker(opponent_state, player_state);
        // May change ability
        check_trace(player_state, opponent_state);
        check_trace(opponent_state, player_state);
        // May change stats
        check_download(player_state, opponent_state);
        check_download(opponent_state, player_state);
        check_intimidate(player_state, opponent_state);
        check_intimidate(opponent_state, player_state);

        if (player_goes_first) {
            check_drizzle(player_state);
            check_drought(player_state);
            check_sandstorm(player_state);
            check_snow_warning(player_state);

            check_drizzle(opponent_state);
            check_drought(opponent_state);
            check_sandstorm(opponent_state);
            check_snow_warning(opponent_state);
        } else {
            check_drizzle(opponent_state);
            check_drought(opponent_state);
            check_sandstorm(opponent_state);
            check_snow_warning(opponent_state);

            check_drizzle(player_state);
            check_drought(player_state);
            check_sandstorm(player_state);
            check_snow_warning(player_state);
        }
    }

    [[nodiscard]] PokemonState& get_player_state() {
        return player_state;
    }

    [[nodiscard]] PokemonState& get_opponent_state() {
        return opponent_state;
    }

    BattleResultEntry battle_loop();
};


#endif //BATTLE_SIMULATOR_H
