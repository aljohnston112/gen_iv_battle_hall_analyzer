#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>

#include "battle_hall_data_source.h"
#include "serebii_pokemon_data_source.h"

constexpr bool MULTI_THREADED = true;
constexpr uint8_t LEVEL = 30;
constexpr bool SKIP_RANKS = false;
constexpr bool ALL_MOVES = false;
constexpr bool USE_HIGHEST_RANK_FOR_WALLS = true;
constexpr bool ANALYZE_ALL = true;
constexpr bool FULL_PRINT = false;


inline std::array<int, NUMBER_OF_TYPES> get_type_to_rank_to_skip() {
    static const std::unordered_map<PokemonType, int> type_to_rank_to_skip = {
        {PokemonType::DARK, 0},
        {PokemonType::WATER, 1},
        {PokemonType::FLYING, 1},
        {PokemonType::POISON, 1},
        {PokemonType::GROUND, 1},
        {PokemonType::GHOST, 1},
        {PokemonType::FIRE, 1},
        {PokemonType::NORMAL, 1},
        {PokemonType::ELECTRIC, 5},
        {PokemonType::STEEL, 2},
        {PokemonType::BUG, 3},
        {PokemonType::PSYCHIC, 2},
        {PokemonType::DRAGON, 5},
        {PokemonType::ROCK, 1},
        {PokemonType::GRASS, 1},
        {PokemonType::FIGHTING, 1},
        {PokemonType::ICE, 1},
    };

    std::array<int, NUMBER_OF_TYPES> out{};
    for (const auto& [type, rank] :
         type_to_rank_to_skip
    ) {
        out[static_cast<int>(type)] = rank;
    }
    return out;
}

inline std::vector<
    std::unordered_map<std::string, std::vector<CustomPokemon>>
> get_pokemon_forms(
    const std::unordered_map<std::string, SerebiiPokemon>& all_serebii_pokemon
) {
    std::vector<std::unordered_map<
        std::string,
        std::vector<CustomPokemon>
    >> pokemon_forms{};
    if (ANALYZE_ALL) {
        for (const auto& serebii_pokemon : all_serebii_pokemon |
             std::views::values
        ) {
            static std::unordered_set<std::string> skip_names = {
                // "Arceus", "Darkrai", "Manaphy", "Cresselia",
                // "Azelf", "Rotom", "Mamoswine", "Leafeon",
                // "Rhyperior", "Togekiss", "Mantyke", "Finneon",
                // "Croagunk", "Drapion", "Lickilicky", "Gabite",
                // "Gible", "Bonsly", "Bronzong", "Bronzor",
                // "Skuntank", "Honchkrow", "Mismagius", "Gastrodon",
                // "Cherrim", "Floatzel", "Buizel", "Combee",
                // "Wormadam", "Burmy", "Shieldon", "Budew",
                // "Luxray", "Munchlax", "Luxio", "Shinx",
                // "Bibarel", "Bidoof", "Prinplup", "Monferno",
                // "Carnivine", "Shellos", "Torterra", "Chingling",
                // "Grotle", "Deoxys", "Jirachi", "Rayquaza",
                // "Kyogre", "Registeel", "Froslass", "Regirock",
                // "Metagross", "Turtwig", "Salamence", "Gorebyss",
                // "Huntail", "Walrein", "Glalie", "Happiny",
                // "Wynaut", "Absol", "Dusclops", "Banette",
                // "Feebas", "Armaldo", "Anorith", "Lileep",
                // "Baltoy", "Crawdaunt", "Whiscash", "Barboach",
                // "Solrock", "Lunatone", "Seviper", "Zangoose",
                // "Swablu", "Cacturne", "Empoleon", "Vibrava",
                // "Grumpig", "Bagon", "Spoink", "Camerupt",
                // "Wailord", "Heatran", "Carvanha", "Swalot",
                // "Gulpin", "Roselia", "Spiritomb", "Drifblim",
                // "Staraptor", "Illumise", "Snover", "Minun",
                // "Plusle", "Manectric", "Electrike", "Numel",
                // "Medicham", "Aron", "Mawile", "Delcatty",
                // "Nosepass", "Drifloon", "Azurill", "Kricketune",
                // "Hariyama", "Vespiquen", "Duskull", "Makuhita",
                // "Exploud", "Aggron", "Whismur", "Latios",
                // "Nincada", "Latias", "Vigoroth", "Breloom",
                // "Dusknoir", "Shroomish", "Ambipom", "Surskit",
                // "Gardevoir", "Metang", "Kirlia", "Ralts",
                // "Pelipper", "Wingull", "Swellow", "Taillow",
                // "Shiftry", "Nuzleaf", "Seedot", "Ludicolo",
                // "Lotad", "Dustox", "Cascoon", "Linoone",
                // "Mightyena", "Marshtomp", "Shaymin", "Starmie",
                // "Clefable", "Kecleon", "Alakazam", "Butterfree",
                // "Hitmonchan", "Weavile", "Magmar", "Piplup",
                // "Exeggcute", "Cradily", "Grimer", "Lairon",
                // "Seadra", "Electrode", "Chatot", "Shuppet",
                // "Drowzee", "Gengar", "Ariados", "Corphish",
                // "Haunter", "Dewgong", "Doduo", "Lucario",
                // "Swinub", "Kingler", "Slowbro", "Beautifly",
                // "Charmander", "Loudred", "Graveler", "Sealeo",
                // "Spheal", "Machop", "Probopass", "Clamperl",
                // "Pikachu", "Staryu", "Quilava", "Magnemite",
                // "Phanpy", "Poliwag", "Arbok", "Paras",
                // "Scyther", "Trapinch", "Donphan", "Fearow",
                // "Poliwhirl", "Spearow", "Tangrowth", "Altaria",
                // "Metapod", "Claydol", "Sandshrew", "Nidoran(F)",
                // "Uxie", "Nidorino", "Slaking", "Lickitung",
                // "Jynx", "Sandslash", "Blastoise", "Weepinbell",
                // "Espeon", "Diglett", "Girafarig", "Pidgeot",
                // "Shellder", "Sunflora", "Natu", "Pidgeotto",
                // "Larvitar", "Vileplume", "Bellsprout", "Venomoth",
                // "Hitmontop", "Porygon-Z", "Infernape", "Raichu",
                // "Goldeen", "Sneasel", "Electabuzz", "Electabuzz",
                // "Regice", "Beedrill", "Ninetales", "Hitmonlee",
                // "Starly", "Geodude", "Gastly", "Mesprit",
                // "Hippopotas", "Victreebel", "Kakuna", "Snorunt",
                // "Weedle", "Toxicroak", "Giratina", "Rattata",
                // "Omanyte", "Celebi", "Ivysaur", "Flygon",
                // "Mew", "Weezing", "Houndoom", "Luvdisc",
                // "Pinsir", "Ponyta", "Garchomp", "Cubone",
                // "Slakoth", "Silcoon", "Nidoran(M)", "Tauros",
                // "Electivire", "Milotic", "Totodile", "Roserade",
                // "Smoochum", "Meowth", "Volbeat", "Umbreon",
                // "Tentacruel", "Poochyena", "Charizard", "Rhydon",
                // "Glameow", "Buneary", "Slowpoke", "Raticate",
                // "Cherubi", "Igglybuff", "Lumineon", "Charmeleon",
                // "Horsea", "Blaziken", "Dugtrio", "Wurmple",
                // "Squirtle", "Psyduck", "Machoke", "Regigigas",
                // "Venonat", "Yanma", "Wailmer", "Exeggutor",
                // "Dialga", "Bastiodon", "Zigzagoon", "Abra",
                // "Gyarados", "Dodrio", "Ninjask", "Nidorina",
                // "Purugly", "Marowak", "Clefairy", "Slugma",
                // "Vulpix", "Slowking", "Golem", "Magmortar",
                // "Poliwrath", "Jigglypuff", "Golbat", "Rapidash",
                // "Chinchou", "Blissey", "Pidgey", "Forretress",
                // "Wigglytuff", "Muk", "Elekid", "Zubat",
                // "Palkia", "Relicanth", "Seel", "Oddish",
                // "Persian", "Hypno", "Gloom", "Koffing",
                // "Wartortle", "Ekans", "Bulbasaur", "Parasect",
                // "Nidoqueen", "Shelgon", "Onix", "Staravia",
                // "Misdreavus", "Arcanine", "Masquerain", "Marill",
                // "Lopunny", "Voltorb", "Tentacool", "Primeape",
                // "Mankey", "Magikarp", "Cranidos", "Lapras",
                // "Aipom", "Ditto", "Gallade", "Cacnea",
                // "Vaporeon", "Snorlax", "Sharpedo", "Flareon",
                // "Mewtwo", "Kabuto", "Quagsire", "Riolu",
                // "Castform", "Golduck", "Aerodactyl", "Tangela",
                // "Farfetch'd", "Zapdos", "Torkoal", "Dratini",
                // "Skitty", "Porygon", "Moltres", "Kadabra",
                // "Venusaur", "Shuckle", "Swampert", "Dragonair",
                // "Dragonite", "Chikorita", "Wooper", "Omastar",
                // "Typhlosion", "Remoraid", "Skarmory", "Seaking",
                // "Croconaw", "Magnezone", "Bayleef", "Feraligatr",
                // "Furret", "Caterpie", "Hoothoot", "Octillery",
                // "Noctowl", "Ledyba", "Phione", "Pachirisu",
                // "Scizor", "Mime Jr.", "Ledian", "Stunky",
                // "Krabby", "Pineco", "Crobat", "Lanturn",
                // "Pichu", "Mudkip", "Cleffa", "Flaaffy",
                // "Glaceon", "Murkrow", "Beldum", "Jolteon",
                // "Piloswine", "Togepi", "Togetic", "Abomasnow",
                // "Xatu", "Cloyster", "Smeargle", "Spinarak",
                // "Mareep", "Ampharos", "Bellossom", "Azumarill",
                // "Kangaskhan", "Heracross", "Pupitar", "Sudowoodo",
                // "Combusken", "Politoed", "Hoppip", "Skiploom",
                // "Dunsparce", "Jumpluff", "Sunkern", "Hippowdon",
                // "Unown", "Wobbuffet", "Tropius", "Gligar",
                // "Chimchar", "Sableye", "Steelix", "Lombre",
                // "Sentret", "Snubbull", "Rhyhorn", "Miltank",
                // "Yanmega", "Spinda", "Cyndaquil", "Granbull",
                // "Magneton", "Qwilfish", "Teddiursa", "Ursaring",
                // "Growlithe", "Porygon2", "Magcargo", "Corsola",
                // "Articuno", "Delibird", "Mantine", "Rampardos",
                // "Groudon", "Meganium", "Kabutops", "Houndour",
                // "Nidoking", "Kingdra", "Skorupi", "Eevee",
                // "Stantler", "Tyrogue", "Mothim", "Kricketot",
                // "Magby", "Raikou", "Gliscor", "Entei",
                // "Lugia", "Suicune", "Meditite", "Shedinja",
                // "Chansey", "Tyranitar" , "Ho-oh", "Treecko",
                // "Chimecho", "Grovyle", "Machamp", "Sceptile",
                // "Mr. Mime", "Torchic"
            };
            if (!skip_names.contains(serebii_pokemon.name)) {
                pokemon_forms.push_back(
                    convert_serebii_to_custom(
                        serebii_pokemon,
                        true,
                        false
                    )
                );
            }
        }
    } else {
        pokemon_forms.push_back(
            convert_serebii_to_custom(
                all_serebii_pokemon.at("Azurill"),
                true,
                true
            )
        );
        for (auto& p_list :
             pokemon_forms.at(0) | std::views::values
        ) {
            static std::unordered_set moves_to_include{
                // Move::ThunderPunch,
                // Move::Fling,
                // Move::Detect,
                // Move::CloseCombat,
                Move::Return,
                Move::Waterfall,
                Move::KnockOff,
                Move::Surf,
            };
            for (auto& p : p_list) {
                std::vector<const MoveInfo*> moves{};
                if constexpr (!ALL_MOVES) {
                    for (const auto& move : p.moves) {
                        if (moves_to_include.contains(move->move)) {
                            moves.push_back(move);
                        }
                    }
                } else {
                    for (const auto& move : p.moves) {
                        if (move->accuracy == 100) {
                            moves.push_back(move);
                        }
                    }
                }
                p.moves = moves;
                p.stats[0] = 70;
                p.stats[1] = 38;
                p.stats[2] = 32;
                p.stats[3] = 15;
                p.stats[4] = 29;
                p.stats[5] = 33;
                p.item = Item::FocusSash;
                p.ability = Ability::HugePower;
            }
        }
    }
    return pokemon_forms;
}

#endif //CONFIG_H
