#ifndef ITEMS_H
#define ITEMS_H

#include <array>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "moves.h"

/*
Ignored:
    BrightPowder,
    FocusBand,
    KingsRock,
    LaxIncense,
    LightClay,
    RazorClaw,
    RazorFang,
    ScopeLens,
    Stick,
    ZoomLens,
 */

enum class Item {
    ApicotBerry,
    BigRoot,
    BlackBelt,
    BlackGlasses,
    BlackSludge,
    BrightPowder,
    Charcoal,
    ChestoBerry,
    ChoiceBand,
    ChoiceScarf,
    ChoiceSpecs,
    DampRock,
    DragonFang,
    ExpertBelt,
    FlameOrb,
    FocusBand,
    FocusSash,
    GanlonBerry,
    HardStone,
    HeatRock,
    IcyRock,
    IronBall,
    KingsRock,
    LaxIncense,
    Leftovers,
    LiechiBerry,
    LifeOrb,
    LightBall,
    LightClay,
    LumBerry,
    Magnet,
    MentalHerb, // TODO
    MetalCoat,
    Metronome,
    MiracleSeed,
    MuscleBand,
    MysticWater,
    NeverMeltIce,
    PersimBerry,
    PetayaBerry,
    PoisonBarb,
    PowerHerb,
    QuickClaw,
    QuickPowder,
    RazorClaw,
    RazorFang,
    RockyHelmet,
    SalacBerry,
    ScopeLens,
    SharpBeak,
    ShellBell,
    SilkScarf,
    SilverPowder,
    SitrusBerry,
    SmoothRock,
    SoftSand,
    SpellTag,
    Stick,
    StickyBarb,
    TwistedSpoon,
    WhiteHerb,
    WiseGlasses,
    ZoomLens,

    FistPlate,
    SkyPlate,
    ToxicPlate,
    EarthPlate,
    StonePlate,
    InsectPlate,
    SpookyPlate,
    IronPlate,
    FlamePlate,
    SplashPlate,
    MeadowPlate,
    ZapPlate,
    MindPlate,
    IciclePlate,
    DracoPlate,
    DreadPlate,

    None
};

static const std::array<bool, static_cast<int>(Item::None) + 1> BERRIES = [] {
    std::array<bool, static_cast<int>(Item::None) + 1> array{};
    array.fill(false);
    array[static_cast<int>(Item::ApicotBerry)] = true;
    array[static_cast<int>(Item::ChestoBerry)] = true;
    array[static_cast<int>(Item::GanlonBerry)] = true;
    array[static_cast<int>(Item::LiechiBerry)] = true;
    array[static_cast<int>(Item::LumBerry)] = true;
    array[static_cast<int>(Item::PersimBerry)] = true;
    array[static_cast<int>(Item::PetayaBerry)] = true;
    array[static_cast<int>(Item::SalacBerry)] = true;
    array[static_cast<int>(Item::SitrusBerry)] = true;
    return array;
}();

static const std::unordered_set STAT_BERRIES = {
    Item::ApicotBerry,
    Item::GanlonBerry,
    Item::LiechiBerry,
    Item::PetayaBerry,
    Item::SalacBerry,
};

static const std::array<
    std::pair<int16_t, PokemonType>,
    static_cast<int>(Item::None) + 1
> NATURAL_GIFT_POWER = [] {
    std::array<
        std::pair<int16_t, PokemonType>,
        static_cast<int>(Item::None) + 1
    > array{};
    array.fill({0, PokemonType::COUNT});
    array[static_cast<int>(Item::ApicotBerry)] = {80, PokemonType::GROUND};
    array[static_cast<int>(Item::ChestoBerry)] = {60, PokemonType::WATER};
    array[static_cast<int>(Item::GanlonBerry)] = {80, PokemonType::ICE};
    array[static_cast<int>(Item::LiechiBerry)] = {80, PokemonType::GRASS};
    array[static_cast<int>(Item::LumBerry)] = {60, PokemonType::FLYING};
    array[static_cast<int>(Item::PersimBerry)] = {60, PokemonType::GROUND};
    array[static_cast<int>(Item::PetayaBerry)] = {80, PokemonType::POISON};
    array[static_cast<int>(Item::SalacBerry)] = {80, PokemonType::FIGHTING};
    array[static_cast<int>(Item::SitrusBerry)] = {60, PokemonType::PSYCHIC};
    return array;
}();

static constexpr std::array<bool, static_cast<int>(Item::None) + 1> PLATE_ITEMS
    = [] {
        std::array<bool, static_cast<int>(Item::None) + 1> array{};
        array.fill(false);
        array[static_cast<int>(Item::FistPlate)] = true;
        array[static_cast<int>(Item::SkyPlate)] = true;
        array[static_cast<int>(Item::ToxicPlate)] = true;
        array[static_cast<int>(Item::EarthPlate)] = true;
        array[static_cast<int>(Item::StonePlate)] = true;
        array[static_cast<int>(Item::InsectPlate)] = true;
        array[static_cast<int>(Item::SpookyPlate)] = true;
        array[static_cast<int>(Item::IronPlate)] = true;
        array[static_cast<int>(Item::FlamePlate)] = true;
        array[static_cast<int>(Item::SplashPlate)] = true;
        array[static_cast<int>(Item::MeadowPlate)] = true;
        array[static_cast<int>(Item::ZapPlate)] = true;
        array[static_cast<int>(Item::MindPlate)] = true;
        array[static_cast<int>(Item::IciclePlate)] = true;
        array[static_cast<int>(Item::DracoPlate)] = true;
        array[static_cast<int>(Item::DreadPlate)] = true;
        return array;
    }();

static constexpr std::array<
    PokemonType,
static_cast<int>(Item::None) + 1
> PLATE_ITEM_TYPES = [] {
    std::array<PokemonType, static_cast<int>(Item::None) + 1> array{};
    array.fill(PokemonType::NORMAL);
    array[static_cast<int>(Item::FistPlate)] = PokemonType::FIGHTING;
    array[static_cast<int>(Item::SkyPlate)] = PokemonType::FLYING;
    array[static_cast<int>(Item::ToxicPlate)] = PokemonType::POISON;
    array[static_cast<int>(Item::EarthPlate)] = PokemonType::GROUND;
    array[static_cast<int>(Item::StonePlate)] = PokemonType::ROCK;
    array[static_cast<int>(Item::InsectPlate)] = PokemonType::BUG;
    array[static_cast<int>(Item::SpookyPlate)] = PokemonType::GHOST;
    array[static_cast<int>(Item::IronPlate)] = PokemonType::STEEL;
    array[static_cast<int>(Item::FlamePlate)] = PokemonType::FIRE;
    array[static_cast<int>(Item::SplashPlate)] = PokemonType::WATER;
    array[static_cast<int>(Item::MeadowPlate)] = PokemonType::GRASS;
    array[static_cast<int>(Item::ZapPlate)] = PokemonType::ELECTRIC;
    array[static_cast<int>(Item::MindPlate)] = PokemonType::PSYCHIC;
    array[static_cast<int>(Item::IciclePlate)] = PokemonType::ICE;
    array[static_cast<int>(Item::DracoPlate)] = PokemonType::DRAGON;
    array[static_cast<int>(Item::DreadPlate)] = PokemonType::DARK;
    return array;
}();

static const std::unordered_set CHOICE_ITEMS = {
    Item::ChoiceBand,
    Item::ChoiceScarf,
    Item::ChoiceSpecs,
};

static const std::unordered_map<std::string, Item> STRING_TO_ITEM = {
    {"Apicot Berry", Item::ApicotBerry},
    {"Big Root", Item::BigRoot},
    {"Black Belt", Item::BlackBelt},
    {"BlackGlasses", Item::BlackGlasses},
    {"Black Sludge", Item::BlackSludge},
    {"BrightPowder", Item::BrightPowder},
    {"Charcoal", Item::Charcoal},
    {"Chesto Berry", Item::ChestoBerry},
    {"Choice Band", Item::ChoiceBand},
    {"Choice Scarf", Item::ChoiceScarf},
    {"Choice Specs", Item::ChoiceSpecs},
    {"Damp Rock", Item::DampRock},
    {"Dragon Fang", Item::DragonFang},
    {"Expert Belt", Item::ExpertBelt},
    {"Flame Orb", Item::FlameOrb},
    {"Focus Band", Item::FocusBand},
    {"Focus Sash", Item::FocusSash},
    {"Ganlon Berry", Item::GanlonBerry},
    {"Hard Stone", Item::HardStone},
    {"Heat Rock", Item::HeatRock},
    {"Icy Rock", Item::IcyRock},
    {"Iron Ball", Item::IronBall},
    {"King's Rock", Item::KingsRock},
    {"Lax Incense", Item::LaxIncense},
    {"Light Ball", Item::LightBall},
    {"Leftovers", Item::Leftovers},
    {"Liechi Berry", Item::LiechiBerry},
    {"Life Orb", Item::LifeOrb},
    {"Light Clay", Item::LightClay},
    {"Lum Berry", Item::LumBerry},
    {"Magnet", Item::Magnet},
    {"Metal Coat", Item::MetalCoat},
    {"Metronome", Item::Metronome},
    {"Mental Herb", Item::MentalHerb},
    {"Miracle Seed", Item::MiracleSeed},
    {"Muscle Band", Item::MuscleBand},
    {"Mystic Water", Item::MysticWater},
    {"NeverMeltIce", Item::NeverMeltIce},
    {"Persim Berry", Item::PersimBerry},
    {"Petaya Berry", Item::PetayaBerry},
    {"Poison Barb", Item::PoisonBarb},
    {"Power Herb", Item::PowerHerb},
    {"Quick Claw", Item::QuickClaw},
    {"Quick Powder", Item::QuickPowder},
    {"Razor Claw", Item::RazorClaw},
    {"Razor Fang", Item::RazorFang},
    {"Rocky Helmet", Item::RockyHelmet},
    {"Salac Berry", Item::SalacBerry},
    {"Scope Lens", Item::ScopeLens},
    {"Sharp Beak", Item::SharpBeak},
    {"Shell Bell", Item::ShellBell},
    {"Silk Scarf", Item::SilkScarf},
    {"SilverPowder", Item::SilverPowder},
    {"Sitrus Berry", Item::SitrusBerry},
    {"Smooth Rock", Item::SmoothRock},
    {"Soft Sand", Item::SoftSand},
    {"Spell Tag", Item::SpellTag},
    {"Stick", Item::Stick},
    {"Sticky Barb", Item::StickyBarb},
    {"TwistedSpoon", Item::TwistedSpoon},
    {"White Herb", Item::WhiteHerb},
    {"Wise Glasses", Item::WiseGlasses},
    {"Zoom Lens", Item::ZoomLens},
    {"", Item::None}
};

inline std::unordered_map<Item, std::string> initialize_item_to_string() {
    std::unordered_map<Item, std::string> map{};
    for (const auto& [item_name, item] : STRING_TO_ITEM) {
        map[item] = item_name;
    }
    return map;
}

static std::unordered_map<Item, std::string> ITEM_TO_STRING =
    initialize_item_to_string();

// 20% boost in power
static constexpr std::array<
    std::array<bool, static_cast<int>(PokemonType::COUNT) + 1>,
    static_cast<int>(Item::None) + 1
> ITEM_TO_TYPE = [] {
    std::array<
        std::array<bool, static_cast<int>(PokemonType::COUNT) + 1>,
        static_cast<int>(Item::None) + 1
    > flags{};
    for (auto& flag : flags) {
        flag.fill(false);
    }

    flags[static_cast<int>(Item::BlackBelt)][
        static_cast<int>(PokemonType::FIGHTING)
    ] = true;
    flags[static_cast<int>(Item::BlackGlasses)][
        static_cast<int>(PokemonType::DARK)
    ] = true;
    flags[static_cast<int>(Item::Charcoal)][
        static_cast<int>(PokemonType::FIRE)
    ] = true;
    flags[static_cast<int>(Item::DragonFang)][
        static_cast<int>(PokemonType::DRAGON)
    ] = true;
    flags[static_cast<int>(Item::HardStone)][
        static_cast<int>(PokemonType::ROCK)
    ] = true;
    flags[static_cast<int>(Item::Magnet)][
        static_cast<int>(PokemonType::ELECTRIC)
    ] = true;
    flags[static_cast<int>(Item::MetalCoat)][
        static_cast<int>(PokemonType::STEEL)
    ] = true;
    flags[static_cast<int>(Item::MiracleSeed)][
        static_cast<int>(PokemonType::GRASS)
    ] = true;
    flags[static_cast<int>(Item::MysticWater)][
        static_cast<int>(PokemonType::WATER)
    ] = true;
    flags[static_cast<int>(Item::NeverMeltIce)][
        static_cast<int>(PokemonType::ICE)
    ] = true;
    flags[static_cast<int>(Item::PoisonBarb)][
        static_cast<int>(PokemonType::POISON)
    ] = true;
    flags[static_cast<int>(Item::SharpBeak)][
        static_cast<int>(PokemonType::FLYING)
    ] = true;
    flags[static_cast<int>(Item::SilkScarf)][
        static_cast<int>(PokemonType::NORMAL)
    ] = true;
    flags[static_cast<int>(Item::SilverPowder)][
        static_cast<int>(PokemonType::BUG)
    ] = true;
    flags[static_cast<int>(Item::SoftSand)][
        static_cast<int>(PokemonType::GROUND)
    ] = true;
    flags[static_cast<int>(Item::SpellTag)][
        static_cast<int>(PokemonType::GHOST)
    ] = true;
    flags[static_cast<int>(Item::TwistedSpoon)][
        static_cast<int>(PokemonType::PSYCHIC)
    ] = true;
    return flags;
}();

/*
    BigRoot, TODO 30% more HP from Leech Seed, Ingrain and Aqua Ring
    HeatRock, TODO sun is eight turns instead of five turns
    IcyRock, TODO hail is eight turns instead of five turns
    IronBall, TODO Even if the holder has Klutz or is affected by Embargo,
                   its Speed is still halved by the Iron Ball.
                   Due to being grounded, the holder becomes susceptible
                   to Arena Trap; the Spikes, and Toxic Spikes,
                   even if it is Flying-type, has the Ability Levitate,
                   or is under the effect of Telekinesis or Magnet Rise.
                   An Iron Ball does not prevent Magnet Rise or Telekinesis
                   from being used successfully.
    QuickPowder, TODO doubles ditto's speed till it transforms.
    RockyHelmet, TODO multi-hit moves cause damage for each hit
    ShellBell, TODO If the holder uses a multistrike move,
                    it recovers HP after the last strike,
                    considering the damage from all of the strikes at once.
                    The Shell Bell will not activate if the move only hits
                    a Pokémon's substitute. Ignores heal block
    SmoothRock, TODO sandstorm is eight turns instead of five turns
 */

/*
 For fling
    10 	All Berries, all Incenses, all Scarves,
    Big Root, Bright Powder, Choice Band, Choice Scarf, Choice Specs,
    Destiny Knot, Expert Belt, Focus Band, Focus Sash, Lagging Tail, Leftovers,
    Mental Herb, Metal Powder, Muscle Band, Power Herb, Quick Powder,
    Reaper Cloth, Shed Shell, Silk Scarf, Silver Powder, Smooth Rock, Soft Sand,
    Soothe Bell, White Herb, Wide Lens, Wise Glasses, Zoom Lens

    30 	All status condition healing items (besides Berries), all Potions,
    all herbal medicine, all drinks, all Vitamins, all Shards, all Mulches,
    all battle items, all Flutes, Ether, Elixir, Max Ether, Max Elixir, Repel,
    Super Repel, Max Repel, Amulet Coin, Berry Juice, Big Mushroom, Big Pearl,
    Black Belt, Black Glasses, Black Sludge, Charcoal, Cleanse Tag,
    Deep Sea Scale, Dragon Scale, Escape Rope, Everstone, Fire Stone, Flame Orb,
    Fluffy Tail, Heart Scale, Honey, Ice Stone, King's Rock, Lava Cookie,
    Leaf Stone, Life Orb, Light Ball, Light Clay, Lucky Egg, Magnet,
    Max Mushrooms, Max Revive, Metal Coat, Metronome, Miracle Seed, Moon Stone,
    Mystic Water, Never-Melt Ice, Nugget, Old Gateau, Pearl, Poké Doll,
    Prism Scale, Rare Candy, Razor Fang, Revive, Sacred Ash, Scope Lens,
    Shell Bell, Shoal Salt, Shoal Shell, Smoke Ball, Snowball, Soul Dew,
    Spell Tag, Star Piece, Stardust, Sun Stone, Thunder Stone, Tiny Mushroom,
    Toxic Orb, Twisted Spoon, Upgrade, Water Stone

    40 	Icy Rock, Lucky Punch

    50 	Dubious Disc, Sharp Beak

    60 	Adamant Orb, Damp Rock, Griseous Orb, Heat Rock, Leek, Lustrous Orb,
    Macho Brace, Rocky Helmet

    70 	All Drives, all Power items
    Dragon Fang, Poison Barb

    80 	Dawn Stone, Dusk Stone, Electirizer, Magmarizer, Odd Keystone,
    Oval Stone, Protector, Quick Claw, Razor Claw, Shiny Stone, Sticky Barb

    90 	All Plates
    Deep Sea Tooth, Grip Claw, Thick Club

    100 	All Fossils
    Hard Stone, Rare Bone, Room Service

    130 	Iron Ball
*/


#endif //ITEMS_H
