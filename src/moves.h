#ifndef ATTACKS_H
#define ATTACKS_H

#include <bitset>
#include <ranges>
#include <string>
#include <unordered_map>
#include <unordered_set>

enum class Move {
    Pound,
    KarateChop,
    DoubleSlap,
    CometPunch,
    MegaPunch,
    PayDay,
    FirePunch,
    IcePunch,
    ThunderPunch,
    Scratch,
    ViceGrip,
    Guillotine,
    RazorWind,
    SwordsDance,
    Cut,
    Gust,
    WingAttack,
    Whirlwind,
    Fly,
    Bind,
    Slam,
    VineWhip,
    Stomp,
    DoubleKick,
    MegaKick,
    JumpKick,
    RollingKick,
    SandAttack,
    Headbutt,
    HornAttack,
    FuryAttack,
    HornDrill,
    Tackle,
    BodySlam,
    Wrap,
    TakeDown,
    Thrash,
    DoubleEdge,
    TailWhip,
    PoisonSting,
    Twineedle,
    PinMissile,
    Leer,
    Bite,
    Growl,
    Roar,
    Sing,
    Supersonic,
    SonicBoom,
    Disable,
    Acid,
    Ember,
    Flamethrower,
    Mist,
    WaterGun,
    HydroPump,
    Surf,
    IceBeam,
    Blizzard,
    Psybeam,
    Bubblebeam,
    AuroraBeam,
    HyperBeam,
    Peck,
    DrillPeck,
    Submission,
    LowKick,
    Counter,
    SeismicToss,
    Strength,
    Absorb,
    MegaDrain,
    LeechSeed,
    Growth,
    RazorLeaf,
    SolarBeam,
    PoisonPowder,
    StunSpore,
    SleepPowder,
    PetalDance,
    StringShot,
    DragonRage,
    FireSpin,
    Thundershock,
    Thunderbolt,
    ThunderWave,
    Thunder,
    RockThrow,
    Earthquake,
    Fissure,
    Dig,
    Toxic,
    Confusion,
    Psychic,
    Hypnosis,
    Meditate,
    Agility,
    QuickAttack,
    Rage,
    Teleport,
    NightShade,
    Mimic,
    Screech,
    DoubleTeam,
    Recover,
    Harden,
    Minimize,
    Smokescreen,
    ConfuseRay,
    Withdraw,
    DefenseCurl,
    Barrier,
    LightScreen,
    Haze,
    Reflect,
    FocusEnergy,
    Bide,
    Metronome,
    MirrorMove,
    Selfdestruct,
    EggBomb,
    Lick,
    Smog,
    Sludge,
    BoneClub,
    FireBlast,
    Waterfall,
    Clamp,
    Swift,
    SkullBash,
    SpikeCannon,
    Constrict,
    Amnesia,
    Kinesis,
    Softboiled,
    HighJumpKick,
    Glare,
    DreamEater,
    PoisonGas,
    Barrage,
    LeechLife,
    LovelyKiss,
    SkyAttack,
    Transform,
    Bubble,
    DizzyPunch,
    Spore,
    Flash,
    Psywave,
    Splash,
    AcidArmor,
    Crabhammer,
    Explosion,
    FurySwipes,
    Bonemerang,
    Rest,
    RockSlide,
    HyperFang,
    Sharpen,
    Conversion,
    TriAttack,
    SuperFang,
    Slash,
    Substitute,
    Struggle,
    Sketch,
    TripleKick,
    Thief,
    SpiderWeb,
    MindReader,
    Nightmare,
    FlameWheel,
    Snore,
    Curse,
    Flail,
    Conversion2,
    Aeroblast,
    CottonSpore,
    Reversal,
    Spite,
    PowderSnow,
    Protect,
    MachPunch,
    ScaryFace,
    FaintAttack,
    SweetKiss,
    BellyDrum,
    SludgeBomb,
    MudSlap,
    Octazooka,
    Spikes,
    ZapCannon,
    Foresight,
    DestinyBond,
    PerishSong,
    IcyWind,
    Detect,
    BoneRush,
    LockOn,
    Outrage,
    Sandstorm,
    GigaDrain,
    Endure,
    Charm,
    Rollout,
    FalseSwipe,
    Swagger,
    MilkDrink,
    Spark,
    FuryCutter,
    SteelWing,
    MeanLook,
    Attract,
    SleepTalk,
    HealBell,
    Return,
    Present,
    Frustration,
    Safeguard,
    PainSplit,
    SacredFire,
    Magnitude,
    Dynamicpunch,
    Megahorn,
    DragonBreath,
    BatonPass,
    Encore,
    Pursuit,
    RapidSpin,
    SweetScent,
    IronTail,
    MetalClaw,
    VitalThrow,
    MorningSun,
    Synthesis,
    Moonlight,
    HiddenPower,
    CrossChop,
    Twister,
    RainDance,
    SunnyDay,
    Crunch,
    MirrorCoat,
    PsychUp,
    ExtremeSpeed,
    Ancientpower,
    ShadowBall,
    FutureSight,
    RockSmash,
    Whirlpool,
    BeatUp,
    FakeOut,
    Uproar,
    Stockpile,
    SpitUp,
    Swallow,
    HeatWave,
    Hail,
    Torment,
    Flatter,
    WillOWisp,
    Memento,
    Facade,
    FocusPunch,
    Smellingsalt,
    FollowMe,
    NaturePower,
    Charge,
    Taunt,
    HelpingHand,
    Trick,
    RolePlay,
    Wish,
    Assist,
    Ingrain,
    Superpower,
    MagicCoat,
    Recycle,
    Revenge,
    BrickBreak,
    Yawn,
    KnockOff,
    Endeavor,
    Eruption,
    SkillSwap,
    Imprison,
    Refresh,
    Grudge,
    Snatch,
    SecretPower,
    Dive,
    ArmThrust,
    Camouflage,
    TailGlow,
    LusterPurge,
    MistBall,
    Featherdance,
    TeeterDance,
    BlazeKick,
    MudSport,
    IceBall,
    NeedleArm,
    SlackOff,
    HyperVoice,
    PoisonFang,
    CrushClaw,
    BlastBurn,
    HydroCannon,
    MeteorMash,
    Astonish,
    WeatherBall,
    Aromatherapy,
    FakeTears,
    AirCutter,
    Overheat,
    OdorSleuth,
    RockTomb,
    SilverWind,
    MetalSound,
    Grasswhistle,
    Tickle,
    CosmicPower,
    WaterSpout,
    SignalBeam,
    ShadowPunch,
    Extrasensory,
    SkyUppercut,
    SandTomb,
    SheerCold,
    MuddyWater,
    BulletSeed,
    AerialAce,
    IcicleSpear,
    IronDefense,
    Block,
    Howl,
    DragonClaw,
    FrenzyPlant,
    BulkUp,
    Bounce,
    MudShot,
    PoisonTail,
    Covet,
    VoltTackle,
    MagicalLeaf,
    WaterSport,
    CalmMind,
    LeafBlade,
    DragonDance,
    RockBlast,
    ShockWave,
    WaterPulse,
    DoomDesire,
    PsychoBoost,
    Roost,
    Gravity,
    MiracleEye,
    WakeUpSlap,
    HammerArm,
    GyroBall,
    HealingWish,
    Brine,
    NaturalGift,
    Feint,
    Pluck,
    Tailwind,
    Acupressure,
    MetalBurst,
    UTurn,
    CloseCombat,
    Payback,
    Assurance,
    Embargo,
    Fling,
    PsychoShift,
    TrumpCard,
    HealBlock,
    WringOut,
    PowerTrick,
    GastroAcid,
    LuckyChant,
    MeFirst,
    Copycat,
    PowerSwap,
    GuardSwap,
    Punishment,
    LastResort,
    WorrySeed,
    SuckerPunch,
    ToxicSpikes,
    HeartSwap,
    AquaRing,
    MagnetRise,
    FlareBlitz,
    ForcePalm,
    AuraSphere,
    RockPolish,
    PoisonJab,
    DarkPulse,
    NightSlash,
    AquaTail,
    SeedBomb,
    AirSlash,
    XScissor,
    BugBuzz,
    DragonPulse,
    DragonRush,
    PowerGem,
    DrainPunch,
    VacuumWave,
    FocusBlast,
    EnergyBall,
    BraveBird,
    EarthPower,
    Switcheroo,
    GigaImpact,
    NastyPlot,
    BulletPunch,
    Avalanche,
    IceShard,
    ShadowClaw,
    ThunderFang,
    IceFang,
    FireFang,
    ShadowSneak,
    MudBomb,
    PsychoCut,
    ZenHeadbutt,
    MirrorShot,
    FlashCannon,
    RockClimb,
    Defog,
    TrickRoom,
    DracoMeteor,
    Discharge,
    LavaPlume,
    LeafStorm,
    PowerWhip,
    RockWrecker,
    CrossPoison,
    GunkShot,
    IronHead,
    MagnetBomb,
    StoneEdge,
    Captivate,
    StealthRock,
    GrassKnot,
    Chatter,
    Judgment,
    BugBite,
    ChargeBeam,
    WoodHammer,
    AquaJet,
    AttackOrder,
    DefendOrder,
    HealOrder,
    HeadSmash,
    DoubleHit,
    RoarOfTime,
    SpacialRend,
    LunarDance,
    CrushGrip,
    MagmaStorm,
    DarkVoid,
    SeedFlare,
    OminousWind,
    ShadowForce,
    Count
};

enum class MoveFlag {
    BOOSTS_ATTACKER_STAT,
    AFFECTED_BY_KINGS_ROCK,
    BADLY_POISONS,
    BOOSTS_ATTACKERS_ATTACK,
    BOOSTS_ATTACKERS_CRIT_RATE,
    BOOSTS_ATTACKERS_DEFENSE,
    BOOSTS_ATTACKERS_EVASION,
    BOOSTS_ATTACKERS_SPECIAL_ATTACK,
    BOOSTS_ATTACKERS_SPECIAL_DEFENSE,
    BOOSTS_ATTACKERS_SPEED,
    BREAKS_PROTECT,
    BURNS_DEFENDER,
    BYPASSES_PROTECT,
    CAN_BE_REFLECTED_BY_MIRROR_MOVE,
    CAN_BE_SNATCHED,
    CHANGES_WEATHER,
    CONFUSES_DEFENDER,
    CONTINUES,
    DEALS_DOUBLE_AFTER_MINIMIZE,
    FLINCHES_DEFENDER,
    FORCES_USER_OUT,
    FREEZES_DEFENDER,
    GOES_INTO_AIR,
    GOES_UNDER_GROUND,
    GOES_UNDER_WATER,
    HAS_FIXED_DAMAGE,
    HEALS_ATTACKER,
    HITS_DEFENDER_IN_AIR,
    HITS_DEFENDER_UNDER_GROUND,
    HITS_DEFENDER_UNDER_WATER,
    LOWERS_ATTACKERS_ATTACK,
    LOWERS_ATTACKERS_DEFENSE,
    LOWERS_ATTACKERS_SPECIAL_ATTACK,
    LOWERS_ATTACKERS_SPECIAL_DEFENSE,
    LOWERS_ATTACKERS_SPEED,
    LOWERS_DEFENDER_ACCURACY,
    LOWERS_DEFENDER_ATTACK,
    LOWERS_DEFENDER_DEFENSE,
    LOWERS_DEFENDER_EVASION,
    LOWERS_DEFENDER_SPECIAL_ATTACK,
    LOWERS_DEFENDER_SPECIAL_DEFENSE,
    LOWERS_DEFENDER_SPEED,
    MAKES_ATTACKER_FAINT,
    MAKES_CONTACT,
    MAKES_DEFENDER_SLEEP,
    BYPASSES_ACCURACY,
    OMNI_BOOSTS_ATTACKER,
    PARALYZES_DEFENDER,
    POISONS_DEFENDER,
    RAISES_DEFENDER_ATTACK,
    RAISES_DEFENDER_SPECIAL_ATTACK,
    REQUIRES_CHARGING_TURN,
    REQUIRES_RECHARGE_TURN,
    GOES_INTO_VOID,
    HAS_INCREASED_CRIT_CHANCE,
    HAS_RECOIL,
    HITS_MULTIPLE_TIMES,
    LASTS_MULTIPLE_TURNS,
    KOS_IN_ONE_HIT,
    IS_OTHER,
    HAS_POWER,
    PROTECTS_USER,
    IS_SOUND_BASED,
    CANT_BE_USED_DURING_GRAVITY,
    POWERS_IRON_FIST,
    COUNT
};

using MoveFlagSet = std::array<bool, static_cast<size_t>(MoveFlag::COUNT)>;
static constexpr std::array<
    MoveFlagSet,
    static_cast<int>(Move::Count)
> MOVE_FLAGS = [] {
    std::array<MoveFlagSet, static_cast<int>(Move::Count)> flags{};
    for (auto& flag : flags) {
        flag.fill(false);
    }

    flags[static_cast<int>(Move::BulletPunch)][
        static_cast<int>(MoveFlag::POWERS_IRON_FIST)
    ] = true;
    flags[static_cast<int>(Move::CometPunch)][
        static_cast<int>(MoveFlag::POWERS_IRON_FIST)
    ] = true;
    flags[static_cast<int>(Move::DizzyPunch)][
        static_cast<int>(MoveFlag::POWERS_IRON_FIST)
    ] = true;
    flags[static_cast<int>(Move::DrainPunch)][
        static_cast<int>(MoveFlag::POWERS_IRON_FIST)
    ] = true;
    flags[static_cast<int>(Move::Dynamicpunch)][
        static_cast<int>(MoveFlag::POWERS_IRON_FIST)
    ] = true;
    flags[static_cast<int>(Move::FirePunch)][
        static_cast<int>(MoveFlag::POWERS_IRON_FIST)
    ] = true;
    flags[static_cast<int>(Move::FocusPunch)][
        static_cast<int>(MoveFlag::POWERS_IRON_FIST)
    ] = true;
    flags[static_cast<int>(Move::HammerArm)][
        static_cast<int>(MoveFlag::POWERS_IRON_FIST)
    ] = true;
    flags[static_cast<int>(Move::IcePunch)][
        static_cast<int>(MoveFlag::POWERS_IRON_FIST)
    ] = true;
    flags[static_cast<int>(Move::MachPunch)][
        static_cast<int>(MoveFlag::POWERS_IRON_FIST)
    ] = true;
    flags[static_cast<int>(Move::ShadowPunch)][
        static_cast<int>(MoveFlag::POWERS_IRON_FIST)
    ] = true;
    flags[static_cast<int>(Move::SkyUppercut)][
        static_cast<int>(MoveFlag::POWERS_IRON_FIST)
    ] = true;
    flags[static_cast<int>(Move::ThunderPunch)][
        static_cast<int>(MoveFlag::POWERS_IRON_FIST)
    ] = true;

    flags[static_cast<int>(Move::Pound)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::DoubleSlap)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::CometPunch)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::MegaPunch)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::PayDay)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::FirePunch)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::IcePunch)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::ThunderPunch)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Scratch)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::ViceGrip)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::RazorWind)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Cut)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Gust)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::WingAttack)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Fly)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Bind)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Slam)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::VineWhip)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Stomp)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::DoubleKick)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::MegaKick)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::JumpKick)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::RollingKick)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Headbutt)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::HornAttack)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::FuryAttack)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Tackle)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::BodySlam)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Wrap)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::TakeDown)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Thrash)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::DoubleEdge)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::PoisonSting)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Twineedle)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::PinMissile)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Bite)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Acid)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Ember)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Flamethrower)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::WaterGun)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::HydroPump)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Surf)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::IceBeam)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Blizzard)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Psybeam)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Bubblebeam)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::AuroraBeam)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::HyperBeam)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Peck)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::DrillPeck)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Submission)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Strength)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Absorb)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::MegaDrain)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::RazorLeaf)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::SolarBeam)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::PetalDance)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::FireSpin)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Thundershock)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Thunderbolt)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::RockThrow)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Earthquake)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Dig)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Confusion)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Psychic)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::QuickAttack)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Selfdestruct)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::EggBomb)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Lick)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Smog)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Sludge)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::BoneClub)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::FireBlast)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Waterfall)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Clamp)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Swift)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::SkullBash)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::SpikeCannon)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Constrict)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::HighJumpKick)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Barrage)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::LeechLife)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::SkyAttack)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Bubble)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::DizzyPunch)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Crabhammer)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Explosion)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::FuryAttack)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Bonemerang)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::RockSlide)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::HyperFang)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::TriAttack)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Slash)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::FlameWheel)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Aeroblast)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::PowderSnow)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::MachPunch)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::FaintAttack)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::SludgeBomb)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::MudSlap)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Octazooka)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::ZapCannon)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::IcyWind)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::BoneRush)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Outrage)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::GigaDrain)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Spark)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::SteelWing)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Return)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Frustration)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::SacredFire)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Dynamicpunch)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Megahorn)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::DragonBreath)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::IronTail)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::MetalClaw)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::VitalThrow)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::CrossChop)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Twister)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Crunch)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::ExtremeSpeed)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Ancientpower)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::ShadowBall)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::RockSmash)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Whirlpool)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::HeatWave)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Superpower)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Dive)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::ArmThrust)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::LusterPurge)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::MistBall)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::BlazeKick)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::NeedleArm)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::HyperVoice)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::PoisonFang)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::CrushClaw)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::BlastBurn)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::HydroCannon)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::MeteorMash)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Astonish)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::AirCutter)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Overheat)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::RockTomb)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::SilverWind)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::SignalBeam)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::ShadowPunch)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Extrasensory)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::SkyUppercut)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::SandTomb)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::MuddyWater)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::BulletSeed)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::AerialAce)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::IcicleSpear)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::DragonClaw)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::FrenzyPlant)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Bounce)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::MudShot)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::PoisonTail)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::VoltTackle)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::MagicalLeaf)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::LeafBlade)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::RockBlast)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::ShockWave)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::WaterPulse)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::PsychoBoost)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::HammerArm)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::CloseCombat)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::FlareBlitz)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::ForcePalm)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::AuraSphere)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::PoisonJab)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::DarkPulse)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::NightSlash)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::AquaTail)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::SeedBomb)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::AirSlash)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::XScissor)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::BugBuzz)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::DragonPulse)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::DragonRush)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::PowerGem)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::DrainPunch)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::VacuumWave)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::FocusBlast)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::EnergyBall)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::BraveBird)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::EarthPower)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::GigaImpact)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::BulletPunch)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::IceShard)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::ShadowClaw)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::ThunderFang)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::IceFang)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::FireFang)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::ShadowSneak)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::MudBomb)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::PsychoCut)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::ZenHeadbutt)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::MirrorShot)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::FlashCannon)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::RockClimb)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::DracoMeteor)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::Discharge)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::LavaPlume)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::LeafStorm)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::PowerWhip)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::RockWrecker)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::CrossPoison)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::GunkShot)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::IronHead)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::MagnetBomb)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::StoneEdge)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::ChargeBeam)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::WoodHammer)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::AquaJet)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::AttackOrder)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::HeadSmash)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::DoubleHit)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::RoarOfTime)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::SpacialRend)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::SeedFlare)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::OminousWind)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;
    flags[static_cast<int>(Move::ShadowForce)][
        static_cast<int>(MoveFlag::HAS_POWER)
    ] = true;

    flags[static_cast<int>(Move::Absorb)][
        static_cast<int>(MoveFlag::HEALS_ATTACKER)
    ] = true;
    flags[static_cast<int>(Move::MegaDrain)][
        static_cast<int>(MoveFlag::HEALS_ATTACKER)
    ] = true;
    flags[static_cast<int>(Move::Recover)][
        static_cast<int>(MoveFlag::HEALS_ATTACKER)
    ] = true;
    flags[static_cast<int>(Move::Softboiled)][
        static_cast<int>(MoveFlag::HEALS_ATTACKER)
    ] = true;
    flags[static_cast<int>(Move::LeechLife)][
        static_cast<int>(MoveFlag::HEALS_ATTACKER)
    ] = true;
    flags[static_cast<int>(Move::GigaDrain)][
        static_cast<int>(MoveFlag::HEALS_ATTACKER)
    ] = true;
    flags[static_cast<int>(Move::MilkDrink)][
        static_cast<int>(MoveFlag::HEALS_ATTACKER)
    ] = true;
    flags[static_cast<int>(Move::MorningSun)][
        static_cast<int>(MoveFlag::HEALS_ATTACKER)
    ] = true;
    flags[static_cast<int>(Move::Synthesis)][
        static_cast<int>(MoveFlag::HEALS_ATTACKER)
    ] = true;
    flags[static_cast<int>(Move::Moonlight)][
        static_cast<int>(MoveFlag::HEALS_ATTACKER)
    ] = true;
    flags[static_cast<int>(Move::SlackOff)][
        static_cast<int>(MoveFlag::HEALS_ATTACKER)
    ] = true;
    flags[static_cast<int>(Move::DrainPunch)][
        static_cast<int>(MoveFlag::HEALS_ATTACKER)
    ] = true;
    flags[static_cast<int>(Move::HealOrder)][
        static_cast<int>(MoveFlag::HEALS_ATTACKER)
    ] = true;

    flags[static_cast<int>(Move::SwordsDance)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT)
    ] = true;
    flags[static_cast<int>(Move::Growth)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT)
    ] = true;
    flags[static_cast<int>(Move::Meditate)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT)
    ] = true;
    flags[static_cast<int>(Move::Agility)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT)
    ] = true;
    flags[static_cast<int>(Move::DoubleTeam)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT)
    ] = true;
    flags[static_cast<int>(Move::Harden)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT)
    ] = true;
    flags[static_cast<int>(Move::Minimize)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT)
    ] = true;
    flags[static_cast<int>(Move::Withdraw)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT)
    ] = true;
    flags[static_cast<int>(Move::Amnesia)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT)
    ] = true;
    flags[static_cast<int>(Move::AcidArmor)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT)
    ] = true;
    flags[static_cast<int>(Move::Sharpen)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT)
    ] = true;
    flags[static_cast<int>(Move::TailGlow)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT)
    ] = true;
    flags[static_cast<int>(Move::CosmicPower)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT)
    ] = true;
    flags[static_cast<int>(Move::IronDefense)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT)
    ] = true;
    flags[static_cast<int>(Move::Howl)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT)
    ] = true;
    flags[static_cast<int>(Move::BulkUp)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT)
    ] = true;
    flags[static_cast<int>(Move::CalmMind)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT)
    ] = true;
    flags[static_cast<int>(Move::DragonDance)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT)
    ] = true;
    flags[static_cast<int>(Move::RockPolish)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT)
    ] = true;
    flags[static_cast<int>(Move::NastyPlot)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT)
    ] = true;
    flags[static_cast<int>(Move::DefendOrder)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT)
    ] = true;

    flags[static_cast<int>(Move::SwordsDance)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_ATTACK)
    ] = true;
    flags[static_cast<int>(Move::Meditate)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_ATTACK)
    ] = true;
    flags[static_cast<int>(Move::Sharpen)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_ATTACK)
    ] = true;
    flags[static_cast<int>(Move::BellyDrum)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_ATTACK)
    ] = true;
    flags[static_cast<int>(Move::MetalClaw)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_ATTACK)
    ] = true;
    flags[static_cast<int>(Move::MeteorMash)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_ATTACK)
    ] = true;
    flags[static_cast<int>(Move::Howl)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_ATTACK)
    ] = true;
    flags[static_cast<int>(Move::BulkUp)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_ATTACK)
    ] = true;
    flags[static_cast<int>(Move::DragonDance)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_ATTACK)
    ] = true;

    flags[static_cast<int>(Move::Superpower)][
        static_cast<int>(MoveFlag::LOWERS_ATTACKERS_ATTACK)
    ] = true;

    flags[static_cast<int>(Move::Harden)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::Withdraw)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::SkullBash)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::AcidArmor)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::SteelWing)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::CosmicPower)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::IronDefense)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::BulkUp)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::DefendOrder)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_DEFENSE)
    ] = true;

    flags[static_cast<int>(Move::Superpower)][
        static_cast<int>(MoveFlag::LOWERS_ATTACKERS_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::CloseCombat)][
        static_cast<int>(MoveFlag::LOWERS_ATTACKERS_DEFENSE)
    ] = true;

    flags[static_cast<int>(Move::Overheat)][
        static_cast<int>(MoveFlag::LOWERS_ATTACKERS_SPECIAL_ATTACK)
    ] = true;
    flags[static_cast<int>(Move::PsychoBoost)][
        static_cast<int>(MoveFlag::LOWERS_ATTACKERS_SPECIAL_ATTACK)
    ] = true;
    flags[static_cast<int>(Move::DracoMeteor)][
        static_cast<int>(MoveFlag::LOWERS_ATTACKERS_SPECIAL_ATTACK)
    ] = true;
    flags[static_cast<int>(Move::LeafStorm)][
        static_cast<int>(MoveFlag::LOWERS_ATTACKERS_SPECIAL_ATTACK)
    ] = true;

    flags[static_cast<int>(Move::Growth)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPECIAL_ATTACK)
    ] = true;
    flags[static_cast<int>(Move::TailGlow)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPECIAL_ATTACK)
    ] = true;
    flags[static_cast<int>(Move::CalmMind)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPECIAL_ATTACK)
    ] = true;
    flags[static_cast<int>(Move::NastyPlot)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPECIAL_ATTACK)
    ] = true;
    flags[static_cast<int>(Move::ChargeBeam)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPECIAL_ATTACK)
    ] = true;

    flags[static_cast<int>(Move::Amnesia)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPECIAL_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::CosmicPower)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPECIAL_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::CalmMind)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPECIAL_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::DefendOrder)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPECIAL_DEFENSE)
    ] = true;

    flags[static_cast<int>(Move::CloseCombat)][
        static_cast<int>(MoveFlag::LOWERS_ATTACKERS_SPECIAL_DEFENSE)
    ] = true;

    flags[static_cast<int>(Move::Agility)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPEED)
    ] = true;
    flags[static_cast<int>(Move::DragonDance)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPEED)
    ] = true;
    flags[static_cast<int>(Move::RockPolish)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPEED)
    ] = true;

    flags[static_cast<int>(Move::HammerArm)][
        static_cast<int>(MoveFlag::LOWERS_ATTACKERS_SPEED)
    ] = true;

    flags[static_cast<int>(Move::DoubleTeam)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_EVASION)
    ] = true;
    flags[static_cast<int>(Move::Minimize)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_EVASION)
    ] = true;

    flags[static_cast<int>(Move::FocusEnergy)][
        static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_CRIT_RATE)
    ] = true;

    flags[static_cast<int>(Move::Ancientpower)][
        static_cast<int>(MoveFlag::OMNI_BOOSTS_ATTACKER)
    ] = true;
    flags[static_cast<int>(Move::SilverWind)][
        static_cast<int>(MoveFlag::OMNI_BOOSTS_ATTACKER)
    ] = true;
    flags[static_cast<int>(Move::OminousWind)][
        static_cast<int>(MoveFlag::OMNI_BOOSTS_ATTACKER)
    ] = true;

    flags[static_cast<int>(Move::Swagger)][
        static_cast<int>(MoveFlag::RAISES_DEFENDER_ATTACK)
    ] = true;

    flags[static_cast<int>(Move::Growl)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_ATTACK)
    ] = true;
    flags[static_cast<int>(Move::AuroraBeam)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_ATTACK)
    ] = true;
    flags[static_cast<int>(Move::Charm)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_ATTACK)
    ] = true;
    flags[static_cast<int>(Move::Memento)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_ATTACK)
    ] = true;
    flags[static_cast<int>(Move::Featherdance)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_ATTACK)
    ] = true;
    flags[static_cast<int>(Move::Tickle)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_ATTACK)
    ] = true;

    flags[static_cast<int>(Move::Flatter)][
        static_cast<int>(MoveFlag::RAISES_DEFENDER_SPECIAL_ATTACK)
    ] = true;

    flags[static_cast<int>(Move::Memento)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_ATTACK)
    ] = true;
    flags[static_cast<int>(Move::MistBall)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_ATTACK)
    ] = true;

    flags[static_cast<int>(Move::TailWhip)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::Leer)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::Screech)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::IronTail)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::Crunch)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::RockSmash)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::CrushClaw)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::Tickle)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_DEFENSE)
    ] = true;

    flags[static_cast<int>(Move::Acid)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::Psychic)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::ShadowBall)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::LusterPurge)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::FakeTears)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::MetalSound)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::BugBuzz)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::FocusBlast)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::EnergyBall)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::EarthPower)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::FlashCannon)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE)
    ] = true;
    flags[static_cast<int>(Move::SeedFlare)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE)
    ] = true;

    flags[static_cast<int>(Move::Bubblebeam)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPEED)
    ] = true;
    flags[static_cast<int>(Move::StringShot)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPEED)
    ] = true;
    flags[static_cast<int>(Move::Constrict)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPEED)
    ] = true;
    flags[static_cast<int>(Move::Bubble)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPEED)
    ] = true;
    flags[static_cast<int>(Move::CottonSpore)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPEED)
    ] = true;
    flags[static_cast<int>(Move::ScaryFace)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPEED)
    ] = true;
    flags[static_cast<int>(Move::IcyWind)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPEED)
    ] = true;
    flags[static_cast<int>(Move::RockTomb)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPEED)
    ] = true;
    flags[static_cast<int>(Move::MudShot)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPEED)
    ] = true;

    flags[static_cast<int>(Move::SandAttack)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_ACCURACY)
    ] = true;
    flags[static_cast<int>(Move::Smokescreen)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_ACCURACY)
    ] = true;
    flags[static_cast<int>(Move::Kinesis)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_ACCURACY)
    ] = true;
    flags[static_cast<int>(Move::Flash)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_ACCURACY)
    ] = true;
    flags[static_cast<int>(Move::MudSlap)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_ACCURACY)
    ] = true;
    flags[static_cast<int>(Move::Octazooka)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_ACCURACY)
    ] = true;
    flags[static_cast<int>(Move::MuddyWater)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_ACCURACY)
    ] = true;
    flags[static_cast<int>(Move::MudBomb)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_ACCURACY)
    ] = true;
    flags[static_cast<int>(Move::MirrorShot)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_ACCURACY)
    ] = true;

    flags[static_cast<int>(Move::SweetScent)][
        static_cast<int>(MoveFlag::LOWERS_DEFENDER_EVASION)
    ] = true;

    flags[static_cast<int>(Move::Blizzard)][
        static_cast<int>(MoveFlag::BYPASSES_ACCURACY)
    ] = true;
    flags[static_cast<int>(Move::Thunder)][
        static_cast<int>(MoveFlag::BYPASSES_ACCURACY)
    ] = true;
    flags[static_cast<int>(Move::Swift)][
        static_cast<int>(MoveFlag::BYPASSES_ACCURACY)
    ] = true;
    flags[static_cast<int>(Move::FaintAttack)][
        static_cast<int>(MoveFlag::BYPASSES_ACCURACY)
    ] = true;
    flags[static_cast<int>(Move::VitalThrow)][
        static_cast<int>(MoveFlag::BYPASSES_ACCURACY)
    ] = true;
    flags[static_cast<int>(Move::ShadowPunch)][
        static_cast<int>(MoveFlag::BYPASSES_ACCURACY)
    ] = true;
    flags[static_cast<int>(Move::AerialAce)][
        static_cast<int>(MoveFlag::BYPASSES_ACCURACY)
    ] = true;
    flags[static_cast<int>(Move::MagicalLeaf)][
        static_cast<int>(MoveFlag::BYPASSES_ACCURACY)
    ] = true;
    flags[static_cast<int>(Move::ShockWave)][
        static_cast<int>(MoveFlag::BYPASSES_ACCURACY)
    ] = true;
    flags[static_cast<int>(Move::AuraSphere)][
        static_cast<int>(MoveFlag::BYPASSES_ACCURACY)
    ] = true;
    flags[static_cast<int>(Move::MagnetBomb)][
        static_cast<int>(MoveFlag::BYPASSES_ACCURACY)
    ] = true;

    flags[static_cast<int>(Move::Bounce)][
        static_cast<int>(MoveFlag::CANT_BE_USED_DURING_GRAVITY)
    ] = true;
    flags[static_cast<int>(Move::Fly)][
        static_cast<int>(MoveFlag::CANT_BE_USED_DURING_GRAVITY)
    ] = true;
    flags[static_cast<int>(Move::HighJumpKick)][
        static_cast<int>(MoveFlag::CANT_BE_USED_DURING_GRAVITY)
    ] = true;
    flags[static_cast<int>(Move::JumpKick)][
        static_cast<int>(MoveFlag::CANT_BE_USED_DURING_GRAVITY)
    ] = true;
    flags[static_cast<int>(Move::MagnetRise)][
        static_cast<int>(MoveFlag::CANT_BE_USED_DURING_GRAVITY)
    ] = true;
    flags[static_cast<int>(Move::Splash)][
        static_cast<int>(MoveFlag::CANT_BE_USED_DURING_GRAVITY)
    ] = true;

    flags[static_cast<int>(Move::Detect)][
        static_cast<int>(MoveFlag::PROTECTS_USER)
    ] = true;
    flags[static_cast<int>(Move::Protect)][
        static_cast<int>(MoveFlag::PROTECTS_USER)
    ] = true;

    flags[static_cast<int>(Move::Disable)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Mist)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::LowKick)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Counter)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::LeechSeed)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Rage)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Teleport)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Mimic)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::DefenseCurl)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Barrier)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::LightScreen)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Haze)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Reflect)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Bide)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Metronome)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::MirrorMove)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::DreamEater)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Transform)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Psywave)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Splash)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Rest)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Conversion)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::TriAttack)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::SuperFang)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Substitute)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Struggle)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Sketch)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::TripleKick)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Thief)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::SpiderWeb)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::MindReader)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Nightmare)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Snore)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Curse)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Flail)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Conversion2)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Reversal)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Spite)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Spikes)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Foresight)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::DestinyBond)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::PerishSong)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::LockOn)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Rollout)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::FalseSwipe)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::FuryCutter)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::MeanLook)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Attract)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::SleepTalk)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::HealBell)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Present)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Safeguard)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::PainSplit)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Magnitude)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::BatonPass)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Encore)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Pursuit)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::RapidSpin)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::HiddenPower)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::MirrorCoat)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::PsychUp)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::FutureSight)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::BeatUp)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::FakeOut)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Uproar)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Stockpile)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::SpitUp)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Swallow)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Torment)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Facade)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::FocusPunch)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Smellingsalt)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::FollowMe)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::NaturePower)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Charge)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Taunt)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::HelpingHand)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Trick)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::RolePlay)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Wish)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Assist)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Ingrain)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::MagicCoat)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Recycle)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Revenge)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::BrickBreak)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Yawn)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::KnockOff)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Endeavor)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Eruption)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::SkillSwap)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Imprison)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Refresh)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Grudge)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Snatch)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::SecretPower)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Camouflage)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::MudSport)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::IceBall)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::WeatherBall)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Aromatherapy)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::OdorSleuth)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::WaterSpout)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Block)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Covet)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::WaterSport)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::DoomDesire)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Roost)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Gravity)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::MiracleEye)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::WakeUpSlap)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::GyroBall)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::HealingWish)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Brine)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::NaturalGift)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Feint)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Pluck)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Tailwind)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Acupressure)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::MetalBurst)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::UTurn)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Payback)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Assurance)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Embargo)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Fling)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::PsychoShift)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::TrumpCard)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::HealBlock)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::WringOut)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::PowerTrick)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::GastroAcid)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::LuckyChant)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::MeFirst)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Copycat)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::PowerSwap)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::GuardSwap)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Punishment)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::LastResort)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::WorrySeed)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::SuckerPunch)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::ToxicSpikes)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::HeartSwap)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::AquaRing)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::MagnetRise)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Switcheroo)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Avalanche)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Defog)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::TrickRoom)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Captivate)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::StealthRock)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::GrassKnot)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Chatter)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::Judgment)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::BugBite)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::LunarDance)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;
    flags[static_cast<int>(Move::CrushGrip)][
        static_cast<int>(MoveFlag::IS_OTHER)
    ] = true;

    flags[static_cast<int>(Move::Sandstorm)][
        static_cast<int>(MoveFlag::CHANGES_WEATHER)
    ] = true;
    flags[static_cast<int>(Move::RainDance)][
        static_cast<int>(MoveFlag::CHANGES_WEATHER)
    ] = true;
    flags[static_cast<int>(Move::SunnyDay)][
        static_cast<int>(MoveFlag::CHANGES_WEATHER)
    ] = true;
    flags[static_cast<int>(Move::Hail)][
        static_cast<int>(MoveFlag::CHANGES_WEATHER)
    ] = true;

    flags[static_cast<int>(Move::Whirlwind)][
        static_cast<int>(MoveFlag::FORCES_USER_OUT)
    ] = true;
    flags[static_cast<int>(Move::Roar)][
        static_cast<int>(MoveFlag::FORCES_USER_OUT)
    ] = true;

    flags[static_cast<int>(Move::Guillotine)][
        static_cast<int>(MoveFlag::KOS_IN_ONE_HIT)
    ] = true;
    flags[static_cast<int>(Move::HornDrill)][
        static_cast<int>(MoveFlag::KOS_IN_ONE_HIT)
    ] = true;
    flags[static_cast<int>(Move::Fissure)][
        static_cast<int>(MoveFlag::KOS_IN_ONE_HIT)
    ] = true;
    flags[static_cast<int>(Move::SheerCold)][
        static_cast<int>(MoveFlag::KOS_IN_ONE_HIT)
    ] = true;

    flags[static_cast<int>(Move::Selfdestruct)][
        static_cast<int>(MoveFlag::MAKES_ATTACKER_FAINT)
    ] = true;
    flags[static_cast<int>(Move::Explosion)][
        static_cast<int>(MoveFlag::MAKES_ATTACKER_FAINT)
    ] = true;
    flags[static_cast<int>(Move::Memento)][
        static_cast<int>(MoveFlag::MAKES_ATTACKER_FAINT)
    ] = true;

    flags[static_cast<int>(Move::KarateChop)][
        static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE)
    ] = true;
    flags[static_cast<int>(Move::RazorWind)][
        static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE)
    ] = true;
    flags[static_cast<int>(Move::RazorLeaf)][
        static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE)
    ] = true;
    flags[static_cast<int>(Move::SkyAttack)][
        static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE)
    ] = true;
    flags[static_cast<int>(Move::Crabhammer)][
        static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE)
    ] = true;
    flags[static_cast<int>(Move::Slash)][
        static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE)
    ] = true;
    flags[static_cast<int>(Move::Aeroblast)][
        static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE)
    ] = true;
    flags[static_cast<int>(Move::CrossChop)][
        static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE)
    ] = true;
    flags[static_cast<int>(Move::BlazeKick)][
        static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE)
    ] = true;
    flags[static_cast<int>(Move::AirCutter)][
        static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE)
    ] = true;
    flags[static_cast<int>(Move::PoisonTail)][
        static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE)
    ] = true;
    flags[static_cast<int>(Move::LeafBlade)][
        static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE)
    ] = true;
    flags[static_cast<int>(Move::NightSlash)][
        static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE)
    ] = true;
    flags[static_cast<int>(Move::ShadowClaw)][
        static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE)
    ] = true;
    flags[static_cast<int>(Move::PsychoCut)][
        static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE)
    ] = true;
    flags[static_cast<int>(Move::CrossPoison)][
        static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE)
    ] = true;
    flags[static_cast<int>(Move::StoneEdge)][
        static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE)
    ] = true;
    flags[static_cast<int>(Move::AttackOrder)][
        static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE)
    ] = true;
    flags[static_cast<int>(Move::SpacialRend)][
        static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE)
    ] = true;

    flags[static_cast<int>(Move::RazorWind)][
        static_cast<int>(MoveFlag::REQUIRES_CHARGING_TURN)
    ] = true;
    flags[static_cast<int>(Move::SolarBeam)][
        static_cast<int>(MoveFlag::REQUIRES_CHARGING_TURN)
    ] = true;
    flags[static_cast<int>(Move::SkullBash)][
        static_cast<int>(MoveFlag::REQUIRES_CHARGING_TURN)
    ] = true;
    flags[static_cast<int>(Move::SkyAttack)][
        static_cast<int>(MoveFlag::REQUIRES_CHARGING_TURN)
    ] = true;

    flags[static_cast<int>(Move::HyperBeam)][
        static_cast<int>(MoveFlag::REQUIRES_RECHARGE_TURN)
    ] = true;
    flags[static_cast<int>(Move::BlastBurn)][
        static_cast<int>(MoveFlag::REQUIRES_RECHARGE_TURN)
    ] = true;
    flags[static_cast<int>(Move::HydroCannon)][
        static_cast<int>(MoveFlag::REQUIRES_RECHARGE_TURN)
    ] = true;
    flags[static_cast<int>(Move::FrenzyPlant)][
        static_cast<int>(MoveFlag::REQUIRES_RECHARGE_TURN)
    ] = true;
    flags[static_cast<int>(Move::GigaImpact)][
        static_cast<int>(MoveFlag::REQUIRES_RECHARGE_TURN)
    ] = true;
    flags[static_cast<int>(Move::RockWrecker)][
        static_cast<int>(MoveFlag::REQUIRES_RECHARGE_TURN)
    ] = true;
    flags[static_cast<int>(Move::RoarOfTime)][
        static_cast<int>(MoveFlag::REQUIRES_RECHARGE_TURN)
    ] = true;

    flags[static_cast<int>(Move::Stomp)][
        static_cast<int>(MoveFlag::DEALS_DOUBLE_AFTER_MINIMIZE)
    ] = true;

    flags[static_cast<int>(Move::Bind)][
        static_cast<int>(MoveFlag::CONTINUES)
    ] = true;
    flags[static_cast<int>(Move::Wrap)][
        static_cast<int>(MoveFlag::CONTINUES)
    ] = true;
    flags[static_cast<int>(Move::FireSpin)][
        static_cast<int>(MoveFlag::CONTINUES)
    ] = true;
    flags[static_cast<int>(Move::Clamp)][
        static_cast<int>(MoveFlag::CONTINUES)
    ] = true;
    flags[static_cast<int>(Move::Whirlpool)][
        static_cast<int>(MoveFlag::CONTINUES)
    ] = true;
    flags[static_cast<int>(Move::SandTomb)][
        static_cast<int>(MoveFlag::CONTINUES)
    ] = true;
    flags[static_cast<int>(Move::MagmaStorm)][
        static_cast<int>(MoveFlag::CONTINUES)
    ] = true;

    flags[static_cast<int>(Move::FirePunch)][
        static_cast<int>(MoveFlag::BURNS_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Ember)][
        static_cast<int>(MoveFlag::BURNS_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Flamethrower)][
        static_cast<int>(MoveFlag::BURNS_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::FireBlast)][
        static_cast<int>(MoveFlag::BURNS_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::FlameWheel)][
        static_cast<int>(MoveFlag::BURNS_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::SacredFire)][
        static_cast<int>(MoveFlag::BURNS_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::HeatWave)][
        static_cast<int>(MoveFlag::BURNS_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::WillOWisp)][
        static_cast<int>(MoveFlag::BURNS_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::BlazeKick)][
        static_cast<int>(MoveFlag::BURNS_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::FlareBlitz)][
        static_cast<int>(MoveFlag::BURNS_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::FireFang)][
        static_cast<int>(MoveFlag::BURNS_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::LavaPlume)][
        static_cast<int>(MoveFlag::BURNS_DEFENDER)
    ] = true;

    flags[static_cast<int>(Move::IcePunch)][
        static_cast<int>(MoveFlag::FREEZES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::IceBeam)][
        static_cast<int>(MoveFlag::FREEZES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Blizzard)][
        static_cast<int>(MoveFlag::FREEZES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::PowderSnow)][
        static_cast<int>(MoveFlag::FREEZES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::IceFang)][
        static_cast<int>(MoveFlag::FREEZES_DEFENDER)
    ] = true;

    flags[static_cast<int>(Move::ThunderPunch)][
        static_cast<int>(MoveFlag::PARALYZES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::BodySlam)][
        static_cast<int>(MoveFlag::PARALYZES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::StunSpore)][
        static_cast<int>(MoveFlag::PARALYZES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Thundershock)][
        static_cast<int>(MoveFlag::PARALYZES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Thunderbolt)][
        static_cast<int>(MoveFlag::PARALYZES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::ThunderWave)][
        static_cast<int>(MoveFlag::PARALYZES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Thunder)][
        static_cast<int>(MoveFlag::PARALYZES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Lick)][
        static_cast<int>(MoveFlag::PARALYZES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Glare)][
        static_cast<int>(MoveFlag::PARALYZES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::ZapCannon)][
        static_cast<int>(MoveFlag::PARALYZES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Spark)][
        static_cast<int>(MoveFlag::PARALYZES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::DragonBreath)][
        static_cast<int>(MoveFlag::PARALYZES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Bounce)][
        static_cast<int>(MoveFlag::PARALYZES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::VoltTackle)][
        static_cast<int>(MoveFlag::PARALYZES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::ForcePalm)][
        static_cast<int>(MoveFlag::PARALYZES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::ThunderFang)][
        static_cast<int>(MoveFlag::PARALYZES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Discharge)][
        static_cast<int>(MoveFlag::PARALYZES_DEFENDER)
    ] = true;

    flags[static_cast<int>(Move::PoisonSting)][
        static_cast<int>(MoveFlag::POISONS_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Twineedle)][
        static_cast<int>(MoveFlag::POISONS_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::PoisonPowder)][
        static_cast<int>(MoveFlag::POISONS_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Smog)][
        static_cast<int>(MoveFlag::POISONS_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Sludge)][
        static_cast<int>(MoveFlag::POISONS_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::PoisonGas)][
        static_cast<int>(MoveFlag::POISONS_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::SludgeBomb)][
        static_cast<int>(MoveFlag::POISONS_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::PoisonTail)][
        static_cast<int>(MoveFlag::POISONS_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::PoisonJab)][
        static_cast<int>(MoveFlag::POISONS_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::CrossPoison)][
        static_cast<int>(MoveFlag::POISONS_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::GunkShot)][
        static_cast<int>(MoveFlag::POISONS_DEFENDER)
    ] = true;

    flags[static_cast<int>(Move::Toxic)][
        static_cast<int>(MoveFlag::BADLY_POISONS)
    ] = true;
    flags[static_cast<int>(Move::PoisonFang)][
        static_cast<int>(MoveFlag::BADLY_POISONS)
    ] = true;

    flags[static_cast<int>(Move::Sing)][
        static_cast<int>(MoveFlag::MAKES_DEFENDER_SLEEP)
    ] = true;
    flags[static_cast<int>(Move::SleepPowder)][
        static_cast<int>(MoveFlag::MAKES_DEFENDER_SLEEP)
    ] = true;
    flags[static_cast<int>(Move::Hypnosis)][
        static_cast<int>(MoveFlag::MAKES_DEFENDER_SLEEP)
    ] = true;
    flags[static_cast<int>(Move::LovelyKiss)][
        static_cast<int>(MoveFlag::MAKES_DEFENDER_SLEEP)
    ] = true;
    flags[static_cast<int>(Move::Spore)][
        static_cast<int>(MoveFlag::MAKES_DEFENDER_SLEEP)
    ] = true;
    flags[static_cast<int>(Move::Grasswhistle)][
        static_cast<int>(MoveFlag::MAKES_DEFENDER_SLEEP)
    ] = true;
    flags[static_cast<int>(Move::DarkVoid)][
        static_cast<int>(MoveFlag::MAKES_DEFENDER_SLEEP)
    ] = true;

    flags[static_cast<int>(Move::Supersonic)][
        static_cast<int>(MoveFlag::CONFUSES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Psybeam)][
        static_cast<int>(MoveFlag::CONFUSES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Confusion)][
        static_cast<int>(MoveFlag::CONFUSES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::ConfuseRay)][
        static_cast<int>(MoveFlag::CONFUSES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::DizzyPunch)][
        static_cast<int>(MoveFlag::CONFUSES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::SweetKiss)][
        static_cast<int>(MoveFlag::CONFUSES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Swagger)][
        static_cast<int>(MoveFlag::CONFUSES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Dynamicpunch)][
        static_cast<int>(MoveFlag::CONFUSES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Flatter)][
        static_cast<int>(MoveFlag::CONFUSES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::TeeterDance)][
        static_cast<int>(MoveFlag::CONFUSES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::SignalBeam)][
        static_cast<int>(MoveFlag::CONFUSES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::WaterPulse)][
        static_cast<int>(MoveFlag::CONFUSES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::RockClimb)][
        static_cast<int>(MoveFlag::CONFUSES_DEFENDER)
    ] = true;

    flags[static_cast<int>(Move::Stomp)][
        static_cast<int>(MoveFlag::FLINCHES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::RollingKick)][
        static_cast<int>(MoveFlag::FLINCHES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Headbutt)][
        static_cast<int>(MoveFlag::FLINCHES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Bite)][
        static_cast<int>(MoveFlag::FLINCHES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::BoneClub)][
        static_cast<int>(MoveFlag::FLINCHES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Waterfall)][
        static_cast<int>(MoveFlag::FLINCHES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::SkyAttack)][
        static_cast<int>(MoveFlag::FLINCHES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::HyperFang)][
        static_cast<int>(MoveFlag::FLINCHES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Twister)][
        static_cast<int>(MoveFlag::FLINCHES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::NeedleArm)][
        static_cast<int>(MoveFlag::FLINCHES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Astonish)][
        static_cast<int>(MoveFlag::FLINCHES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::Extrasensory)][
        static_cast<int>(MoveFlag::FLINCHES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::DarkPulse)][
        static_cast<int>(MoveFlag::FLINCHES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::AirSlash)][
        static_cast<int>(MoveFlag::FLINCHES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::DragonRush)][
        static_cast<int>(MoveFlag::FLINCHES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::ThunderFang)][
        static_cast<int>(MoveFlag::FLINCHES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::IceFang)][
        static_cast<int>(MoveFlag::FLINCHES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::FireFang)][
        static_cast<int>(MoveFlag::FLINCHES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::ZenHeadbutt)][
        static_cast<int>(MoveFlag::FLINCHES_DEFENDER)
    ] = true;
    flags[static_cast<int>(Move::IronHead)][
        static_cast<int>(MoveFlag::FLINCHES_DEFENDER)
    ] = true;

    flags[static_cast<int>(Move::JumpKick)][
        static_cast<int>(MoveFlag::HAS_RECOIL)
    ] = true;
    flags[static_cast<int>(Move::TakeDown)][
        static_cast<int>(MoveFlag::HAS_RECOIL)
    ] = true;
    flags[static_cast<int>(Move::DoubleEdge)][
        static_cast<int>(MoveFlag::HAS_RECOIL)
    ] = true;
    flags[static_cast<int>(Move::Submission)][
        static_cast<int>(MoveFlag::HAS_RECOIL)
    ] = true;
    flags[static_cast<int>(Move::HighJumpKick)][
        static_cast<int>(MoveFlag::HAS_RECOIL)
    ] = true;
    flags[static_cast<int>(Move::BellyDrum)][
        static_cast<int>(MoveFlag::HAS_RECOIL)
    ] = true;
    flags[static_cast<int>(Move::VoltTackle)][
        static_cast<int>(MoveFlag::HAS_RECOIL)
    ] = true;
    flags[static_cast<int>(Move::FlareBlitz)][
        static_cast<int>(MoveFlag::HAS_RECOIL)
    ] = true;
    flags[static_cast<int>(Move::BraveBird)][
        static_cast<int>(MoveFlag::HAS_RECOIL)
    ] = true;
    flags[static_cast<int>(Move::WoodHammer)][
        static_cast<int>(MoveFlag::HAS_RECOIL)
    ] = true;
    flags[static_cast<int>(Move::HeadSmash)][
        static_cast<int>(MoveFlag::HAS_RECOIL)
    ] = true;

    flags[static_cast<int>(Move::DoubleSlap)][
        static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES)
    ] = true;
    flags[static_cast<int>(Move::CometPunch)][
        static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES)
    ] = true;
    flags[static_cast<int>(Move::DoubleKick)][
        static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES)
    ] = true;
    flags[static_cast<int>(Move::FuryAttack)][
        static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES)
    ] = true;
    flags[static_cast<int>(Move::Twineedle)][
        static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES)
    ] = true;
    flags[static_cast<int>(Move::PinMissile)][
        static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES)
    ] = true;
    flags[static_cast<int>(Move::SpikeCannon)][
        static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES)
    ] = true;
    flags[static_cast<int>(Move::Barrage)][
        static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES)
    ] = true;
    flags[static_cast<int>(Move::FuryAttack)][
        static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES)
    ] = true;
    flags[static_cast<int>(Move::Bonemerang)][
        static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES)
    ] = true;
    flags[static_cast<int>(Move::BoneRush)][
        static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES)
    ] = true;
    flags[static_cast<int>(Move::ArmThrust)][
        static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES)
    ] = true;
    flags[static_cast<int>(Move::BulletSeed)][
        static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES)
    ] = true;
    flags[static_cast<int>(Move::IcicleSpear)][
        static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES)
    ] = true;
    flags[static_cast<int>(Move::RockBlast)][
        static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES)
    ] = true;
    flags[static_cast<int>(Move::DoubleHit)][
        static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES)
    ] = true;

    flags[static_cast<int>(Move::Thrash)][
        static_cast<int>(MoveFlag::LASTS_MULTIPLE_TURNS)
    ] = true;
    flags[static_cast<int>(Move::PetalDance)][
        static_cast<int>(MoveFlag::LASTS_MULTIPLE_TURNS)
    ] = true;
    flags[static_cast<int>(Move::Outrage)][
        static_cast<int>(MoveFlag::LASTS_MULTIPLE_TURNS)
    ] = true;

    flags[static_cast<int>(Move::SonicBoom)][
        static_cast<int>(MoveFlag::HAS_FIXED_DAMAGE)
    ] = true;
    flags[static_cast<int>(Move::SeismicToss)][
        static_cast<int>(MoveFlag::HAS_FIXED_DAMAGE)
    ] = true;
    flags[static_cast<int>(Move::DragonRage)][
        static_cast<int>(MoveFlag::HAS_FIXED_DAMAGE)
    ] = true;
    flags[static_cast<int>(Move::NightShade)][
        static_cast<int>(MoveFlag::HAS_FIXED_DAMAGE)
    ] = true;

    flags[static_cast<int>(Move::Bounce)][
        static_cast<int>(MoveFlag::GOES_INTO_AIR)
    ] = true;
    flags[static_cast<int>(Move::Fly)][
        static_cast<int>(MoveFlag::GOES_INTO_AIR)
    ] = true;

    flags[static_cast<int>(Move::Gust)][
        static_cast<int>(MoveFlag::HITS_DEFENDER_IN_AIR)
    ] = true;
    flags[static_cast<int>(Move::SkyUppercut)][
        static_cast<int>(MoveFlag::HITS_DEFENDER_IN_AIR)
    ] = true;
    flags[static_cast<int>(Move::Thunder)][
        static_cast<int>(MoveFlag::HITS_DEFENDER_IN_AIR)
    ] = true;
    flags[static_cast<int>(Move::Twister)][
        static_cast<int>(MoveFlag::HITS_DEFENDER_IN_AIR)
    ] = true;

    flags[static_cast<int>(Move::Dig)][
        static_cast<int>(MoveFlag::GOES_UNDER_GROUND)
    ] = true;

    flags[static_cast<int>(Move::Earthquake)][
        static_cast<int>(MoveFlag::HITS_DEFENDER_UNDER_GROUND)
    ] = true;
    flags[static_cast<int>(Move::Magnitude)][
        static_cast<int>(MoveFlag::HITS_DEFENDER_UNDER_GROUND)
    ] = true;
    flags[static_cast<int>(Move::Fissure)][
        static_cast<int>(MoveFlag::HITS_DEFENDER_UNDER_GROUND)
    ] = true;

    flags[static_cast<int>(Move::Dive)][
        static_cast<int>(MoveFlag::GOES_UNDER_WATER)
    ] = true;

    flags[static_cast<int>(Move::Surf)][
        static_cast<int>(MoveFlag::HITS_DEFENDER_UNDER_WATER)
    ] = true;
    flags[static_cast<int>(Move::Whirlpool)][
        static_cast<int>(MoveFlag::HITS_DEFENDER_UNDER_WATER)
    ] = true;

    flags[static_cast<int>(Move::ShadowForce)][
        static_cast<int>(MoveFlag::GOES_INTO_VOID)
    ] = true;

    flags[static_cast<int>(Move::Pound)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::KarateChop)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::DoubleSlap)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::CometPunch)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::MegaPunch)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::FirePunch)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::IcePunch)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::ThunderPunch)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Scratch)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::ViceGrip)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Guillotine)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Cut)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::WingAttack)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Fly)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Bind)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Slam)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::VineWhip)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Stomp)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::DoubleKick)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::MegaKick)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::JumpKick)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::RollingKick)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Headbutt)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::HornAttack)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::FuryAttack)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::HornDrill)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Tackle)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::BodySlam)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Wrap)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::TakeDown)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Thrash)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::DoubleEdge)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Bite)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Peck)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::DrillPeck)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Submission)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::LowKick)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Counter)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::SeismicToss)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Strength)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::PetalDance)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Dig)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::QuickAttack)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Rage)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Bide)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Lick)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Waterfall)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Clamp)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::SkullBash)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Constrict)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::HighJumpKick)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::LeechLife)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::DizzyPunch)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Crabhammer)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::FurySwipes)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::HyperFang)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::SuperFang)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Slash)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Struggle)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::TripleKick)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Thief)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::FlameWheel)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Flail)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Reversal)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::MachPunch)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::FaintAttack)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Outrage)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Rollout)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::FalseSwipe)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Spark)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::FuryCutter)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::SteelWing)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Return)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Frustration)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Dynamicpunch)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Megahorn)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Pursuit)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::RapidSpin)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::IronTail)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::MetalClaw)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::VitalThrow)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::CrossChop)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Crunch)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::ExtremeSpeed)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::RockSmash)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::FakeOut)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Facade)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::FocusPunch)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Smellingsalt)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Superpower)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Revenge)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::BrickBreak)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::KnockOff)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Endeavor)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Dive)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::ArmThrust)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::BlazeKick)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::IceBall)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::NeedleArm)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::PoisonFang)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::CrushClaw)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::MeteorMash)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Astonish)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::ShadowPunch)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::SkyUppercut)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::AerialAce)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::DragonClaw)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Bounce)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::PoisonTail)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Covet)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::VoltTackle)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::LeafBlade)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::WakeUpSlap)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::HammerArm)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::GyroBall)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Pluck)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::UTurn)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::CloseCombat)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Payback)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Assurance)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Punishment)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::LastResort)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::SuckerPunch)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::FlareBlitz)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::ForcePalm)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::PoisonJab)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::NightSlash)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::AquaTail)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::XScissor)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::DragonRush)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::DrainPunch)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::BraveBird)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::GigaImpact)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::BulletPunch)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::Avalanche)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::ShadowClaw)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::ThunderFang)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::IceFang)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::FireFang)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::ShadowSneak)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::ZenHeadbutt)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::RockClimb)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::PowerWhip)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::CrossPoison)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::IronHead)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::BugBite)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::WoodHammer)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::AquaJet)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::HeadSmash)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::DoubleHit)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::CrushGrip)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;
    flags[static_cast<int>(Move::ShadowForce)][
        static_cast<int>(MoveFlag::MAKES_CONTACT)
    ] = true;

    flags[static_cast<int>(Move::Acupressure)][
        static_cast<int>(MoveFlag::BYPASSES_PROTECT)
    ] = true;
    flags[static_cast<int>(Move::Curse)][
        static_cast<int>(MoveFlag::BYPASSES_PROTECT)
    ] = true;
    flags[static_cast<int>(Move::DoomDesire)][
        static_cast<int>(MoveFlag::BYPASSES_PROTECT)
    ] = true;
    flags[static_cast<int>(Move::Feint)][
        static_cast<int>(MoveFlag::BYPASSES_PROTECT)
    ] = true;
    flags[static_cast<int>(Move::FutureSight)][
        static_cast<int>(MoveFlag::BYPASSES_PROTECT)
    ] = true;
    flags[static_cast<int>(Move::PerishSong)][
        static_cast<int>(MoveFlag::BYPASSES_PROTECT)
    ] = true;
    flags[static_cast<int>(Move::PsychUp)][
        static_cast<int>(MoveFlag::BYPASSES_PROTECT)
    ] = true;
    flags[static_cast<int>(Move::RolePlay)][
        static_cast<int>(MoveFlag::BYPASSES_PROTECT)
    ] = true;
    flags[static_cast<int>(Move::ShadowForce)][
        static_cast<int>(MoveFlag::BYPASSES_PROTECT)
    ] = true;
    flags[static_cast<int>(Move::Sketch)][
        static_cast<int>(MoveFlag::BYPASSES_PROTECT)
    ] = true;
    flags[static_cast<int>(Move::Transform)][
        static_cast<int>(MoveFlag::BYPASSES_PROTECT)
    ] = true;

    flags[static_cast<int>(Move::Feint)][
        static_cast<int>(MoveFlag::BREAKS_PROTECT)
    ] = true;
    flags[static_cast<int>(Move::ShadowForce)][
        static_cast<int>(MoveFlag::BREAKS_PROTECT)
    ] = true;

    flags[static_cast<int>(Move::Attract)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::Block)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::Captivate)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::Charm)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::ConfuseRay)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::CottonSpore)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::DarkVoid)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::FakeTears)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::Featherdance)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::Flash)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::Flatter)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::GastroAcid)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::Glare)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::Grasswhistle)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::Growl)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::Hypnosis)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::Kinesis)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::LeechSeed)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::Leer)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::LovelyKiss)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::MeanLook)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::MetalSound)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::PoisonGas)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::PoisonPowder)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::SandAttack)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::ScaryFace)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::Screech)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::Sing)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::SleepPowder)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::Smokescreen)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::SpiderWeb)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::Spore)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::StringShot)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::StunSpore)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::Supersonic)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::Swagger)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::SweetKiss)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::SweetScent)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::TailWhip)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::ThunderWave)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::Tickle)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::Toxic)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::WillOWisp)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::WorrySeed)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;
    flags[static_cast<int>(Move::Yawn)][
        static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_MOVE)
    ] = true;

    flags[static_cast<int>(Move::AcidArmor)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Acupressure)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Agility)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Amnesia)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Aromatherapy)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Barrier)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::BellyDrum)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::BulkUp)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::CalmMind)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Camouflage)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Charge)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::CosmicPower)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::DefendOrder)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::DefenseCurl)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::DoubleTeam)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::DragonDance)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::FocusEnergy)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Growth)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Harden)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::HealBell)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::HealOrder)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Howl)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Ingrain)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::IronDefense)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::LightScreen)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Meditate)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::MilkDrink)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Minimize)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Mist)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Moonlight)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::MorningSun)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::NastyPlot)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::PsychUp)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Recover)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Reflect)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Refresh)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Rest)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::RockPolish)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Roost)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Safeguard)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Sharpen)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::SlackOff)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Softboiled)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Stockpile)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Substitute)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Swallow)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::SwordsDance)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Synthesis)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::TailGlow)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Tailwind)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;
    flags[static_cast<int>(Move::Withdraw)][
        static_cast<int>(MoveFlag::CAN_BE_SNATCHED)
    ] = true;

    flags[static_cast<int>(Move::AerialAce)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Aeroblast)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::AirCutter)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::AirSlash)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::AquaJet)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::AquaTail)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::ArmThrust)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Assurance)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::AttackOrder)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::AuraSphere)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Avalanche)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Barrage)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::BeatUp)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Bide)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Bind)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::BlastBurn)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::BoneRush)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Bonemerang)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Bounce)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::BraveBird)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::BrickBreak)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Brine)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::BugBite)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::BulletPunch)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::BulletSeed)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::ChargeBeam)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Clamp)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::CloseCombat)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::CometPunch)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Crabhammer)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::CrossChop)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::CrossPoison)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::CrushGrip)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Cut)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::DarkPulse)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Dig)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Discharge)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Dive)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::DoubleHit)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::DoubleKick)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::DoubleSlap)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::DoubleEdge)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::DracoMeteor)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::DragonBreath)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::DragonClaw)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::DragonPulse)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::DragonRush)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::DrainPunch)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::DrillPeck)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::EarthPower)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Earthquake)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::EggBomb)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Endeavor)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Eruption)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Explosion)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::ExtremeSpeed)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::FalseSwipe)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::FaintAttack)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::FireFang)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::FireSpin)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Flail)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::FlashCannon)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Fly)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::ForcePalm)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::FrenzyPlant)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Frustration)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::FuryAttack)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::FuryCutter)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::FurySwipes)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::GigaImpact)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::GrassKnot)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::GunkShot)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::GyroBall)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::HammerArm)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::HeadSmash)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::HiddenPower)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::HighJumpKick)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::HornAttack)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::HydroCannon)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::HydroPump)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::HyperBeam)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::IceBall)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::IceFang)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::IceShard)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::IcicleSpear)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::IronHead)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Judgment)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::JumpKick)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::KarateChop)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::LastResort)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::LavaPlume)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::LeafBlade)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::LeafStorm)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::LowKick)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::MachPunch)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::MagicalLeaf)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::MagmaStorm)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::MagnetBomb)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Magnitude)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::MegaKick)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::MegaPunch)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Megahorn)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::MeteorMash)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::MirrorCoat)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::MirrorShot)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::MudBomb)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::MudShot)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::MuddyWater)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::NightShade)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::NightSlash)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::OminousWind)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Outrage)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Overheat)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::PayDay)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Payback)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Peck)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::PetalDance)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::PinMissile)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Pluck)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::PoisonJab)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::PoisonTail)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::PowerGem)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::PowerWhip)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::PsychoBoost)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::PsychoCut)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Psywave)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Punishment)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::QuickAttack)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Rage)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::RapidSpin)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::RazorLeaf)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::RazorWind)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Return)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Revenge)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Reversal)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::RoarOfTime)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::RockBlast)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::RockClimb)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::RockThrow)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::RockWrecker)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::RollingKick)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Rollout)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::SandTomb)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Scratch)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::SeedBomb)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::SeedFlare)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::SeismicToss)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::ShadowClaw)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::ShadowForce)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::ShadowPunch)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::ShadowSneak)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::ShockWave)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::SignalBeam)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::SilverWind)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::SkullBash)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::SkyAttack)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::SkyUppercut)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Slam)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Slash)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Snore)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::SolarBeam)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::SonicBoom)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::SpacialRend)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::SpikeCannon)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::SpitUp)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::SteelWing)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::StoneEdge)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Strength)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Struggle)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Submission)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::SuckerPunch)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Surf)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Swift)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Tackle)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::TakeDown)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Thrash)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::ThunderFang)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::TripleKick)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::TrumpCard)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Twister)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::UTurn)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Uproar)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::VacuumWave)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::ViceGrip)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::VineWhip)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::VitalThrow)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::VoltTackle)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::WakeUpSlap)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::WaterGun)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::WaterPulse)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Waterfall)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::WeatherBall)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Whirlpool)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::WingAttack)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::WoodHammer)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::Wrap)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::WringOut)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;
    flags[static_cast<int>(Move::XScissor)][
        static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK)
    ] = true;

    flags[static_cast<int>(Move::Growl)][
        static_cast<int>(MoveFlag::IS_SOUND_BASED)
    ] = true;
    flags[static_cast<int>(Move::Roar)][
        static_cast<int>(MoveFlag::IS_SOUND_BASED)
    ] = true;
    flags[static_cast<int>(Move::Sing)][
        static_cast<int>(MoveFlag::IS_SOUND_BASED)
    ] = true;
    flags[static_cast<int>(Move::Supersonic)][
        static_cast<int>(MoveFlag::IS_SOUND_BASED)
    ] = true;
    flags[static_cast<int>(Move::Screech)][
        static_cast<int>(MoveFlag::IS_SOUND_BASED)
    ] = true;
    flags[static_cast<int>(Move::Snore)][
        static_cast<int>(MoveFlag::IS_SOUND_BASED)
    ] = true;
    flags[static_cast<int>(Move::PerishSong)][
        static_cast<int>(MoveFlag::IS_SOUND_BASED)
    ] = true;
    flags[static_cast<int>(Move::HealBell)][
        static_cast<int>(MoveFlag::IS_SOUND_BASED)
    ] = true;
    flags[static_cast<int>(Move::Uproar)][
        static_cast<int>(MoveFlag::IS_SOUND_BASED)
    ] = true;
    flags[static_cast<int>(Move::HyperVoice)][
        static_cast<int>(MoveFlag::IS_SOUND_BASED)
    ] = true;
    flags[static_cast<int>(Move::MetalSound)][
        static_cast<int>(MoveFlag::IS_SOUND_BASED)
    ] = true;
    flags[static_cast<int>(Move::Grasswhistle)][
        static_cast<int>(MoveFlag::IS_SOUND_BASED)
    ] = true;
    flags[static_cast<int>(Move::Howl)][
        static_cast<int>(MoveFlag::IS_SOUND_BASED)
    ] = true;
    flags[static_cast<int>(Move::BugBuzz)][
        static_cast<int>(MoveFlag::IS_SOUND_BASED)
    ] = true;
    flags[static_cast<int>(Move::Chatter)][
        static_cast<int>(MoveFlag::IS_SOUND_BASED)
    ] = true;

    return flags;
}();

inline bool move_has_flag(const Move move, const MoveFlag move_flag) {
    return MOVE_FLAGS[static_cast<int>(move)][
        static_cast<size_t>(move_flag)];
}

static constexpr std::array<int8_t, static_cast<int>(Move::Count)> MOVE_PRIORITIES
    = [] {
        std::array<int8_t, static_cast<int>(Move::Count)> array{};
        array[static_cast<int>(Move::HelpingHand)] = 5;

        array[static_cast<int>(Move::MagicCoat)] = 4;
        array[static_cast<int>(Move::Snatch)] = 4;

        array[static_cast<int>(Move::Detect)] = 3;
        array[static_cast<int>(Move::Endure)] = 3;
        array[static_cast<int>(Move::FollowMe)] = 3;
        array[static_cast<int>(Move::Protect)] = 3;

        array[static_cast<int>(Move::Feint)] = 2;

        array[static_cast<int>(Move::AquaJet)] = 1;
        array[static_cast<int>(Move::Bide)] = 1;
        array[static_cast<int>(Move::BulletPunch)] = 1;
        array[static_cast<int>(Move::ExtremeSpeed)] = 1;
        array[static_cast<int>(Move::FakeOut)] = 1;
        array[static_cast<int>(Move::IceShard)] = 1;
        array[static_cast<int>(Move::MachPunch)] = 1;
        array[static_cast<int>(Move::QuickAttack)] = 1;
        array[static_cast<int>(Move::ShadowSneak)] = 1;
        array[static_cast<int>(Move::SuckerPunch)] = 1;
        array[static_cast<int>(Move::VacuumWave)] = 1;

        array[static_cast<int>(Move::VitalThrow)] = -1;

        array[static_cast<int>(Move::FocusPunch)] = -3;

        array[static_cast<int>(Move::Avalanche)] = -4;
        array[static_cast<int>(Move::Revenge)] = -4;

        array[static_cast<int>(Move::Counter)] = -5;
        array[static_cast<int>(Move::MirrorCoat)] = -5;

        array[static_cast<int>(Move::Roar)] = -6;
        array[static_cast<int>(Move::Whirlwind)] = -6;

        array[static_cast<int>(Move::TrickRoom)] = -7;

        return array;
    }();

enum class PokemonType {
    NORMAL,
    FIGHTING,
    FLYING,
    POISON,
    GROUND,
    ROCK,
    BUG,
    GHOST,
    STEEL,
    FIRE,
    WATER,
    GRASS,
    ELECTRIC,
    PSYCHIC,
    ICE,
    DRAGON,
    DARK,
    COUNT
};

enum class Category {
    PHYSICAL,
    SPECIAL,
    STATUS
};

struct MoveInfo {
    std::string name;
    Move move;
    PokemonType type;
    Category category;
    int16_t power;
    int accuracy;
    int power_points;
    int effect_percent;

    bool operator==(const MoveInfo& other) const {
        return move == other.move;
    }
};

template <>
struct std::hash<MoveInfo> {
    size_t operator()(const MoveInfo& a) const noexcept {
        return static_cast<size_t>(a.move);
    }
};

inline int8_t get_move_priority(const MoveInfo* move_info) {
    if (move_info == nullptr) {
        return 0;
    }
    const auto move = static_cast<int>(move_info->move);
    return MOVE_PRIORITIES[move];
}

static const std::unordered_map<std::string, Move> MOVE_MAP = {
    {"Pound", Move::Pound},
    {"Karate Chop", Move::KarateChop},
    {"Doubleslap", Move::DoubleSlap},
    {"Comet Punch", Move::CometPunch},
    {"Mega Punch", Move::MegaPunch},
    {"Pay Day", Move::PayDay},
    {"Fire Punch", Move::FirePunch},
    {"Ice Punch", Move::IcePunch},
    {"Thunderpunch", Move::ThunderPunch},
    {"Thunder Punch", Move::ThunderPunch},
    {"Scratch", Move::Scratch},
    {"Vicegrip", Move::ViceGrip},
    {"Guillotine", Move::Guillotine},
    {"Razor Wind", Move::RazorWind},
    {"Swords Dance", Move::SwordsDance},
    {"Cut", Move::Cut},
    {"Gust", Move::Gust},
    {"Wing Attack", Move::WingAttack},
    {"Whirlwind", Move::Whirlwind},
    {"Fly", Move::Fly},
    {"Bind", Move::Bind},
    {"Slam", Move::Slam},
    {"Vine Whip", Move::VineWhip},
    {"Stomp", Move::Stomp},
    {"Double Kick", Move::DoubleKick},
    {"Mega Kick", Move::MegaKick},
    {"Jump Kick", Move::JumpKick},
    {"Rolling Kick", Move::RollingKick},
    {"Sand-attack", Move::SandAttack},
    {"Sand Attack", Move::SandAttack},
    {"Headbutt", Move::Headbutt},
    {"Horn Attack", Move::HornAttack},
    {"Fury Attack", Move::FuryAttack},
    {"Horn Drill", Move::HornDrill},
    {"Tackle", Move::Tackle},
    {"Body Slam", Move::BodySlam},
    {"Wrap", Move::Wrap},
    {"Take Down", Move::TakeDown},
    {"Thrash", Move::Thrash},
    {"Double-edge", Move::DoubleEdge},
    {"Double-Edge", Move::DoubleEdge},
    {"Tail Whip", Move::TailWhip},
    {"Poison Sting", Move::PoisonSting},
    {"Twineedle", Move::Twineedle},
    {"Pin Missile", Move::PinMissile},
    {"Leer", Move::Leer},
    {"Bite", Move::Bite},
    {"Growl", Move::Growl},
    {"Roar", Move::Roar},
    {"Sing", Move::Sing},
    {"Supersonic", Move::Supersonic},
    {"Sonicboom", Move::SonicBoom},
    {"Disable", Move::Disable},
    {"Acid", Move::Acid},
    {"Ember", Move::Ember},
    {"Flamethrower", Move::Flamethrower},
    {"Mist", Move::Mist},
    {"Water Gun", Move::WaterGun},
    {"Hydro Pump", Move::HydroPump},
    {"Surf", Move::Surf},
    {"Ice Beam", Move::IceBeam},
    {"Blizzard", Move::Blizzard},
    {"Psybeam", Move::Psybeam},
    {"Bubblebeam", Move::Bubblebeam},
    {"Bubble Beam", Move::Bubblebeam},
    {"Aurora Beam", Move::AuroraBeam},
    {"Hyper Beam", Move::HyperBeam},
    {"Peck", Move::Peck},
    {"Drill Peck", Move::DrillPeck},
    {"Submission", Move::Submission},
    {"Low Kick", Move::LowKick},
    {"Counter", Move::Counter},
    {"Seismic Toss", Move::SeismicToss},
    {"Strength", Move::Strength},
    {"Absorb", Move::Absorb},
    {"Mega Drain", Move::MegaDrain},
    {"Leech Seed", Move::LeechSeed},
    {"Growth", Move::Growth},
    {"Razor Leaf", Move::RazorLeaf},
    {"Solarbeam", Move::SolarBeam},
    {"Solar Beam", Move::SolarBeam},
    {"Poisonpowder", Move::PoisonPowder},
    {"Poison Powder", Move::PoisonPowder},
    {"Stun Spore", Move::StunSpore},
    {"Sleep Powder", Move::SleepPowder},
    {"Petal Dance", Move::PetalDance},
    {"String Shot", Move::StringShot},
    {"Dragon Rage", Move::DragonRage},
    {"Fire Spin", Move::FireSpin},
    {"Thundershock", Move::Thundershock},
    {"Thunderbolt", Move::Thunderbolt},
    {"Thunder Wave", Move::ThunderWave},
    {"Thunder", Move::Thunder},
    {"Rock Throw", Move::RockThrow},
    {"Earthquake", Move::Earthquake},
    {"Fissure", Move::Fissure},
    {"Dig", Move::Dig},
    {"Toxic", Move::Toxic},
    {"Confusion", Move::Confusion},
    {"Psychic", Move::Psychic},
    {"Hypnosis", Move::Hypnosis},
    {"Meditate", Move::Meditate},
    {"Agility", Move::Agility},
    {"Quick Attack", Move::QuickAttack},
    {"Rage", Move::Rage},
    {"Teleport", Move::Teleport},
    {"Night Shade", Move::NightShade},
    {"Mimic", Move::Mimic},
    {"Screech", Move::Screech},
    {"Double Team", Move::DoubleTeam},
    {"Recover", Move::Recover},
    {"Harden", Move::Harden},
    {"Minimize", Move::Minimize},
    {"Smokescreen", Move::Smokescreen},
    {"SmokeScreen", Move::Smokescreen},
    {"Confuse Ray", Move::ConfuseRay},
    {"Withdraw", Move::Withdraw},
    {"Defense Curl", Move::DefenseCurl},
    {"Barrier", Move::Barrier},
    {"Light Screen", Move::LightScreen},
    {"Haze", Move::Haze},
    {"Reflect", Move::Reflect},
    {"Focus Energy", Move::FocusEnergy},
    {"Bide", Move::Bide},
    {"Metronome", Move::Metronome},
    {"Mirror Move", Move::MirrorMove},
    {"Selfdestruct", Move::Selfdestruct},
    {"Egg Bomb", Move::EggBomb},
    {"Lick", Move::Lick},
    {"Smog", Move::Smog},
    {"Sludge", Move::Sludge},
    {"Bone Club", Move::BoneClub},
    {"Fire Blast", Move::FireBlast},
    {"Waterfall", Move::Waterfall},
    {"Clamp", Move::Clamp},
    {"Swift", Move::Swift},
    {"Skull Bash", Move::SkullBash},
    {"Spike Cannon", Move::SpikeCannon},
    {"Constrict", Move::Constrict},
    {"Amnesia", Move::Amnesia},
    {"Kinesis", Move::Kinesis},
    {"Softboiled", Move::Softboiled},
    {"Soft-Boiled", Move::Softboiled},
    {"Hi Jump Kick", Move::HighJumpKick},
    {"High Jump Kick", Move::HighJumpKick},
    {"Glare", Move::Glare},
    {"Dream Eater", Move::DreamEater},
    {"Poison Gas", Move::PoisonGas},
    {"Barrage", Move::Barrage},
    {"Leech Life", Move::LeechLife},
    {"Lovely Kiss", Move::LovelyKiss},
    {"Sky Attack", Move::SkyAttack},
    {"Transform", Move::Transform},
    {"Bubble", Move::Bubble},
    {"Dizzy Punch", Move::DizzyPunch},
    {"Spore", Move::Spore},
    {"Flash", Move::Flash},
    {"Psywave", Move::Psywave},
    {"Splash", Move::Splash},
    {"Acid Armor", Move::AcidArmor},
    {"Crabhammer", Move::Crabhammer},
    {"Explosion", Move::Explosion},
    {"Fury Swipes", Move::FurySwipes},
    {"Bonemerang", Move::Bonemerang},
    {"Rest", Move::Rest},
    {"Rock Slide", Move::RockSlide},
    {"Hyper Fang", Move::HyperFang},
    {"Sharpen", Move::Sharpen},
    {"Conversion", Move::Conversion},
    {"Tri Attack", Move::TriAttack},
    {"Super Fang", Move::SuperFang},
    {"Slash", Move::Slash},
    {"Substitute", Move::Substitute},
    {"Struggle", Move::Struggle},
    {"Sketch", Move::Sketch},
    {"Triple Kick", Move::TripleKick},
    {"Thief", Move::Thief},
    {"Spider Web", Move::SpiderWeb},
    {"Mind Reader", Move::MindReader},
    {"Nightmare", Move::Nightmare},
    {"Flame Wheel", Move::FlameWheel},
    {"Snore", Move::Snore},
    {"Curse", Move::Curse},
    {"Flail", Move::Flail},
    {"Conversion 2", Move::Conversion2},
    {"Aeroblast", Move::Aeroblast},
    {"Cotton Spore", Move::CottonSpore},
    {"Reversal", Move::Reversal},
    {"Spite", Move::Spite},
    {"Powder Snow", Move::PowderSnow},
    {"Protect", Move::Protect},
    {"Mach Punch", Move::MachPunch},
    {"Scary Face", Move::ScaryFace},
    {"Faint Attack", Move::FaintAttack},
    {"Feint Attack", Move::FaintAttack},
    {"Sweet Kiss", Move::SweetKiss},
    {"Belly Drum", Move::BellyDrum},
    {"Sludge Bomb", Move::SludgeBomb},
    {"Mud-slap", Move::MudSlap},
    {"Mud-Slap", Move::MudSlap},
    {"Octazooka", Move::Octazooka},
    {"Spikes", Move::Spikes},
    {"Zap Cannon", Move::ZapCannon},
    {"Foresight", Move::Foresight},
    {"Destiny Bond", Move::DestinyBond},
    {"Perish Song", Move::PerishSong},
    {"Icy Wind", Move::IcyWind},
    {"Detect", Move::Detect},
    {"Bone Rush", Move::BoneRush},
    {"Lock-on", Move::LockOn},
    {"Outrage", Move::Outrage},
    {"Sandstorm", Move::Sandstorm},
    {"Giga Drain", Move::GigaDrain},
    {"Endure", Move::Endure},
    {"Charm", Move::Charm},
    {"Rollout", Move::Rollout},
    {"False Swipe", Move::FalseSwipe},
    {"Swagger", Move::Swagger},
    {"Milk Drink", Move::MilkDrink},
    {"Spark", Move::Spark},
    {"Fury Cutter", Move::FuryCutter},
    {"Steel Wing", Move::SteelWing},
    {"Mean Look", Move::MeanLook},
    {"Attract", Move::Attract},
    {"Sleep Talk", Move::SleepTalk},
    {"Heal Bell", Move::HealBell},
    {"Return", Move::Return},
    {"Present", Move::Present},
    {"Frustration", Move::Frustration},
    {"Safeguard", Move::Safeguard},
    {"Pain Split", Move::PainSplit},
    {"Sacred Fire", Move::SacredFire},
    {"Magnitude", Move::Magnitude},
    {"Dynamicpunch", Move::Dynamicpunch},
    {"Dynamic Punch", Move::Dynamicpunch},
    {"Megahorn", Move::Megahorn},
    {"Dragonbreath", Move::DragonBreath},
    {"Baton Pass", Move::BatonPass},
    {"Encore", Move::Encore},
    {"Pursuit", Move::Pursuit},
    {"Rapid Spin", Move::RapidSpin},
    {"Sweet Scent", Move::SweetScent},
    {"Iron Tail", Move::IronTail},
    {"Metal Claw", Move::MetalClaw},
    {"Vital Throw", Move::VitalThrow},
    {"Morning Sun", Move::MorningSun},
    {"Synthesis", Move::Synthesis},
    {"Moonlight", Move::Moonlight},
    {"Hidden Power", Move::HiddenPower},
    {"Cross Chop", Move::CrossChop},
    {"Twister", Move::Twister},
    {"Rain Dance", Move::RainDance},
    {"Sunny Day", Move::SunnyDay},
    {"Crunch", Move::Crunch},
    {"Mirror Coat", Move::MirrorCoat},
    {"Psych Up", Move::PsychUp},
    {"Extremespeed", Move::ExtremeSpeed},
    {"Extreme Speed", Move::ExtremeSpeed},
    {"Ancientpower", Move::Ancientpower},
    {"Ancient Power", Move::Ancientpower},
    {"Shadow Ball", Move::ShadowBall},
    {"Future Sight", Move::FutureSight},
    {"Rock Smash", Move::RockSmash},
    {"Whirlpool", Move::Whirlpool},
    {"Beat Up", Move::BeatUp},
    {"Fake Out", Move::FakeOut},
    {"Uproar", Move::Uproar},
    {"Stockpile", Move::Stockpile},
    {"Spit Up", Move::SpitUp},
    {"Swallow", Move::Swallow},
    {"Heat Wave", Move::HeatWave},
    {"Hail", Move::Hail},
    {"Torment", Move::Torment},
    {"Flatter", Move::Flatter},
    {"Will-o-wisp", Move::WillOWisp},
    {"Will-O-Wisp", Move::WillOWisp},
    {"Memento", Move::Memento},
    {"Facade", Move::Facade},
    {"Focus Punch", Move::FocusPunch},
    {"Smellingsalt", Move::Smellingsalt},
    {"Follow Me", Move::FollowMe},
    {"Nature Power", Move::NaturePower},
    {"Charge", Move::Charge},
    {"Taunt", Move::Taunt},
    {"Helping Hand", Move::HelpingHand},
    {"Trick", Move::Trick},
    {"Role Play", Move::RolePlay},
    {"Wish", Move::Wish},
    {"Assist", Move::Assist},
    {"Ingrain", Move::Ingrain},
    {"Superpower", Move::Superpower},
    {"Magic Coat", Move::MagicCoat},
    {"Recycle", Move::Recycle},
    {"Revenge", Move::Revenge},
    {"Brick Break", Move::BrickBreak},
    {"Yawn", Move::Yawn},
    {"Knock Off", Move::KnockOff},
    {"Endeavor", Move::Endeavor},
    {"Eruption", Move::Eruption},
    {"Skill Swap", Move::SkillSwap},
    {"Imprison", Move::Imprison},
    {"Refresh", Move::Refresh},
    {"Grudge", Move::Grudge},
    {"Snatch", Move::Snatch},
    {"Secret Power", Move::SecretPower},
    {"Dive", Move::Dive},
    {"Arm Thrust", Move::ArmThrust},
    {"Camouflage", Move::Camouflage},
    {"Tail Glow", Move::TailGlow},
    {"Luster Purge", Move::LusterPurge},
    {"Mist Ball", Move::MistBall},
    {"Featherdance", Move::Featherdance},
    {"Feather Dance", Move::Featherdance},
    {"Teeter Dance", Move::TeeterDance},
    {"Blaze Kick", Move::BlazeKick},
    {"Mud Sport", Move::MudSport},
    {"Ice Ball", Move::IceBall},
    {"Needle Arm", Move::NeedleArm},
    {"Slack Off", Move::SlackOff},
    {"Hyper Voice", Move::HyperVoice},
    {"Poison Fang", Move::PoisonFang},
    {"Crush Claw", Move::CrushClaw},
    {"Blast Burn", Move::BlastBurn},
    {"Hydro Cannon", Move::HydroCannon},
    {"Meteor Mash", Move::MeteorMash},
    {"Astonish", Move::Astonish},
    {"Weather Ball", Move::WeatherBall},
    {"Aromatherapy", Move::Aromatherapy},
    {"Fake Tears", Move::FakeTears},
    {"Air Cutter", Move::AirCutter},
    {"Overheat", Move::Overheat},
    {"Odor Sleuth", Move::OdorSleuth},
    {"Rock Tomb", Move::RockTomb},
    {"Silver Wind", Move::SilverWind},
    {"Metal Sound", Move::MetalSound},
    {"Grasswhistle", Move::Grasswhistle},
    {"Tickle", Move::Tickle},
    {"Cosmic Power", Move::CosmicPower},
    {"Water Spout", Move::WaterSpout},
    {"Signal Beam", Move::SignalBeam},
    {"Shadow Punch", Move::ShadowPunch},
    {"Extrasensory", Move::Extrasensory},
    {"Sky Uppercut", Move::SkyUppercut},
    {"Sand Tomb", Move::SandTomb},
    {"Sheer Cold", Move::SheerCold},
    {"Muddy Water", Move::MuddyWater},
    {"Bullet Seed", Move::BulletSeed},
    {"Aerial Ace", Move::AerialAce},
    {"Icicle Spear", Move::IcicleSpear},
    {"Iron Defense", Move::IronDefense},
    {"Block", Move::Block},
    {"Howl", Move::Howl},
    {"Dragon Claw", Move::DragonClaw},
    {"Frenzy Plant", Move::FrenzyPlant},
    {"Bulk Up", Move::BulkUp},
    {"Bounce", Move::Bounce},
    {"Mud Shot", Move::MudShot},
    {"Poison Tail", Move::PoisonTail},
    {"Covet", Move::Covet},
    {"Volt Tackle", Move::VoltTackle},
    {"Magical Leaf", Move::MagicalLeaf},
    {"Water Sport", Move::WaterSport},
    {"Calm Mind", Move::CalmMind},
    {"Leaf Blade", Move::LeafBlade},
    {"Dragon Dance", Move::DragonDance},
    {"Rock Blast", Move::RockBlast},
    {"Shock Wave", Move::ShockWave},
    {"Water Pulse", Move::WaterPulse},
    {"Doom Desire", Move::DoomDesire},
    {"Psycho Boost", Move::PsychoBoost},
    {"Roost", Move::Roost},
    {"Gravity", Move::Gravity},
    {"Miracle Eye", Move::MiracleEye},
    {"Wake-up Slap", Move::WakeUpSlap},
    {"Hammer Arm", Move::HammerArm},
    {"Gyro Ball", Move::GyroBall},
    {"Healing Wish", Move::HealingWish},
    {"Brine", Move::Brine},
    {"Natural Gift", Move::NaturalGift},
    {"Feint", Move::Feint},
    {"Pluck", Move::Pluck},
    {"Tailwind", Move::Tailwind},
    {"Acupressure", Move::Acupressure},
    {"Metal Burst", Move::MetalBurst},
    {"U-turn", Move::UTurn},
    {"Close Combat", Move::CloseCombat},
    {"Payback", Move::Payback},
    {"Assurance", Move::Assurance},
    {"Embargo", Move::Embargo},
    {"Fling", Move::Fling},
    {"Psycho Shift", Move::PsychoShift},
    {"Trump Card", Move::TrumpCard},
    {"Heal Block", Move::HealBlock},
    {"Wring Out", Move::WringOut},
    {"Power Trick", Move::PowerTrick},
    {"Gastro Acid", Move::GastroAcid},
    {"Lucky Chant", Move::LuckyChant},
    {"Me First", Move::MeFirst},
    {"Copycat", Move::Copycat},
    {"Power Swap", Move::PowerSwap},
    {"Guard Swap", Move::GuardSwap},
    {"Punishment", Move::Punishment},
    {"Last Resort", Move::LastResort},
    {"Worry Seed", Move::WorrySeed},
    {"Sucker Punch", Move::SuckerPunch},
    {"Toxic Spikes", Move::ToxicSpikes},
    {"Heart Swap", Move::HeartSwap},
    {"Aqua Ring", Move::AquaRing},
    {"Magnet Rise", Move::MagnetRise},
    {"Flare Blitz", Move::FlareBlitz},
    {"Force Palm", Move::ForcePalm},
    {"Aura Sphere", Move::AuraSphere},
    {"Rock Polish", Move::RockPolish},
    {"Poison Jab", Move::PoisonJab},
    {"Dark Pulse", Move::DarkPulse},
    {"Night Slash", Move::NightSlash},
    {"Aqua Tail", Move::AquaTail},
    {"Seed Bomb", Move::SeedBomb},
    {"Air Slash", Move::AirSlash},
    {"X-scissor", Move::XScissor},
    {"X-Scissor", Move::XScissor},
    {"Bug Buzz", Move::BugBuzz},
    {"Dragon Pulse", Move::DragonPulse},
    {"Dragon Rush", Move::DragonRush},
    {"Power Gem", Move::PowerGem},
    {"Drain Punch", Move::DrainPunch},
    {"Vacuum Wave", Move::VacuumWave},
    {"Focus Blast", Move::FocusBlast},
    {"Energy Ball", Move::EnergyBall},
    {"Brave Bird", Move::BraveBird},
    {"Earth Power", Move::EarthPower},
    {"Switcheroo", Move::Switcheroo},
    {"Giga Impact", Move::GigaImpact},
    {"Nasty Plot", Move::NastyPlot},
    {"Bullet Punch", Move::BulletPunch},
    {"Avalanche", Move::Avalanche},
    {"Ice Shard", Move::IceShard},
    {"Shadow Claw", Move::ShadowClaw},
    {"Thunder Fang", Move::ThunderFang},
    {"Ice Fang", Move::IceFang},
    {"Fire Fang", Move::FireFang},
    {"Shadow Sneak", Move::ShadowSneak},
    {"Mud Bomb", Move::MudBomb},
    {"Psycho Cut", Move::PsychoCut},
    {"Zen Headbutt", Move::ZenHeadbutt},
    {"Mirror Shot", Move::MirrorShot},
    {"Flash Cannon", Move::FlashCannon},
    {"Rock Climb", Move::RockClimb},
    {"Defog", Move::Defog},
    {"Trick Room", Move::TrickRoom},
    {"Draco Meteor", Move::DracoMeteor},
    {"Discharge", Move::Discharge},
    {"Lava Plume", Move::LavaPlume},
    {"Leaf Storm", Move::LeafStorm},
    {"Power Whip", Move::PowerWhip},
    {"Rock Wrecker", Move::RockWrecker},
    {"Cross Poison", Move::CrossPoison},
    {"Gunk Shot", Move::GunkShot},
    {"Iron Head", Move::IronHead},
    {"Magnet Bomb", Move::MagnetBomb},
    {"Stone Edge", Move::StoneEdge},
    {"Captivate", Move::Captivate},
    {"Stealth Rock", Move::StealthRock},
    {"Grass Knot", Move::GrassKnot},
    {"Chatter", Move::Chatter},
    {"Judgment", Move::Judgment},
    {"Bug Bite", Move::BugBite},
    {"Charge Beam", Move::ChargeBeam},
    {"Wood Hammer", Move::WoodHammer},
    {"Aqua Jet", Move::AquaJet},
    {"Attack Order", Move::AttackOrder},
    {"Defend Order", Move::DefendOrder},
    {"Heal Order", Move::HealOrder},
    {"Head Smash", Move::HeadSmash},
    {"Double Hit", Move::DoubleHit},
    {"Roar Of Time", Move::RoarOfTime},
    {"Spacial Rend", Move::SpacialRend},
    {"Lunar Dance", Move::LunarDance},
    {"Crush Grip", Move::CrushGrip},
    {"Magma Storm", Move::MagmaStorm},
    {"Dark Void", Move::DarkVoid},
    {"Seed Flare", Move::SeedFlare},
    {"Ominous Wind", Move::OminousWind},
    {"Shadow Force", Move::ShadowForce}
};

inline std::unordered_map<Move, std::string> create_reverse_move_map() {
    std::unordered_map<Move, std::string> reverse_map;
    for (const auto& [str, move] : MOVE_MAP) {
        reverse_map[move] = str;
    }
    return reverse_map;
}

static const auto MOVE_TO_STRING =
    create_reverse_move_map();

inline void verify_map() {
    std::unordered_set<Move> moves;
    for (int i = 0; i < static_cast<int>(Move::Count); ++i) {
        moves.insert(static_cast<Move>(i));
    }
    for (Move value : MOVE_MAP | std::ranges::views::values) {
        moves.erase(value);
    }
    if (!moves.empty()) {
        throw std::logic_error{"Invalid move map"};
    }
}

#endif //ATTACKS_H
