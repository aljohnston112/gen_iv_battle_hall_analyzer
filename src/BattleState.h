#ifndef BATTLE_SIMULATOR_H
#define BATTLE_SIMULATOR_H

#include "custom_pokemon.h"
#include "PokemonState.h"

struct MoveDamagePairHash {
    std::size_t operator()(
        const std::pair<const MoveInfo*, int>& p
    ) const noexcept {
        const std::size_t h1 = std::hash<const MoveInfo*>{}(p.first);
        const std::size_t h2 = std::hash<int>{}(p.second);
        return h1 ^ (h2 << 8);
    }
};

struct BattleResultEntry {
    CustomPokemon* player;
    CustomPokemon* opponent;
    bool won;
    std::vector<
        std::pair<Move, int>
    > player_moves;
    std::vector<
        std::pair<Move, int>
    > opponent_moves;
};

BattleResultEntry battle(
    CustomPokemon& player,
    CustomPokemon& opponent
);

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
    PokemonState player_state;
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
    CustomPokemon& player_pokemon,
    CustomPokemon& opponent_pokemon
) :
    player_state{player_pokemon, true},
    opponent_state{opponent_pokemon, false} {}

    [[nodiscard]] PokemonState& get_player_state() {
        return player_state;
    }

    [[nodiscard]] PokemonState& get_opponent_state() {
        return opponent_state;
    }

    BattleResultEntry battle();
};


#endif //BATTLE_SIMULATOR_H
