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
    CAN_BE_REFLECTED_BY_MIRROR_COAT,
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
    CANT_BE_USED_DURING_GRAVITY,
    COUNT
};

using MoveFlagSet = std::bitset<static_cast<size_t>(MoveFlag::COUNT)>;
static constexpr std::array<MoveFlagSet, static_cast<int>(Move::Count)> MOVE_FLAGS = [] {
    std::array<MoveFlagSet, static_cast<int>(Move::Count)> flags{};
    flags[static_cast<int>(Move::Pound)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::DoubleSlap)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::CometPunch)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::MegaPunch)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::PayDay)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::FirePunch)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::IcePunch)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::ThunderPunch)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Scratch)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::ViceGrip)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::RazorWind)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Cut)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Gust)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::WingAttack)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Fly)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Bind)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Slam)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::VineWhip)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Stomp)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::DoubleKick)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::MegaKick)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::JumpKick)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::RollingKick)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Headbutt)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::HornAttack)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::FuryAttack)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Tackle)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::BodySlam)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Wrap)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::TakeDown)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Thrash)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::DoubleEdge)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::PoisonSting)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Twineedle)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::PinMissile)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Bite)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Acid)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Ember)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Flamethrower)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::WaterGun)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::HydroPump)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Surf)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::IceBeam)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Blizzard)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Psybeam)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Bubblebeam)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::AuroraBeam)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::HyperBeam)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Peck)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::DrillPeck)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Submission)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Strength)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Absorb)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::MegaDrain)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::RazorLeaf)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::SolarBeam)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::PetalDance)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::FireSpin)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Thundershock)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Thunderbolt)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::RockThrow)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Earthquake)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Dig)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Confusion)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Psychic)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::QuickAttack)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Selfdestruct)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::EggBomb)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Lick)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Smog)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Sludge)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::BoneClub)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::FireBlast)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Waterfall)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Clamp)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Swift)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::SkullBash)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::SpikeCannon)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Constrict)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::HighJumpKick)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Barrage)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::LeechLife)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::SkyAttack)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Bubble)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::DizzyPunch)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Crabhammer)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Explosion)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::FuryAttack)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Bonemerang)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::RockSlide)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::HyperFang)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::TriAttack)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Slash)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::FlameWheel)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Aeroblast)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::PowderSnow)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::MachPunch)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::FaintAttack)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::SludgeBomb)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::MudSlap)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Octazooka)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::ZapCannon)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::IcyWind)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::BoneRush)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Outrage)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::GigaDrain)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Spark)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::SteelWing)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Return)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Frustration)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::SacredFire)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Dynamicpunch)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Megahorn)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::DragonBreath)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::IronTail)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::MetalClaw)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::VitalThrow)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::CrossChop)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Twister)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Crunch)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::ExtremeSpeed)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Ancientpower)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::ShadowBall)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::RockSmash)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Whirlpool)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::HeatWave)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Superpower)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Dive)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::ArmThrust)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::LusterPurge)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::MistBall)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::BlazeKick)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::NeedleArm)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::HyperVoice)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::PoisonFang)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::CrushClaw)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::BlastBurn)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::HydroCannon)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::MeteorMash)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Astonish)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::AirCutter)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Overheat)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::RockTomb)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::SilverWind)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::SignalBeam)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::ShadowPunch)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Extrasensory)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::SkyUppercut)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::SandTomb)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::MuddyWater)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::BulletSeed)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::AerialAce)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::IcicleSpear)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::DragonClaw)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::FrenzyPlant)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Bounce)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::MudShot)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::PoisonTail)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::VoltTackle)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::MagicalLeaf)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::LeafBlade)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::RockBlast)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::ShockWave)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::WaterPulse)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::PsychoBoost)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::HammerArm)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::CloseCombat)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::FlareBlitz)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::ForcePalm)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::AuraSphere)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::PoisonJab)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::DarkPulse)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::NightSlash)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::AquaTail)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::SeedBomb)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::AirSlash)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::XScissor)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::BugBuzz)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::DragonPulse)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::DragonRush)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::PowerGem)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::DrainPunch)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::VacuumWave)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::FocusBlast)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::EnergyBall)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::BraveBird)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::EarthPower)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::GigaImpact)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::BulletPunch)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::IceShard)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::ShadowClaw)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::ThunderFang)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::IceFang)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::FireFang)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::ShadowSneak)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::MudBomb)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::PsychoCut)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::ZenHeadbutt)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::MirrorShot)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::FlashCannon)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::RockClimb)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::DracoMeteor)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::Discharge)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::LavaPlume)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::LeafStorm)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::PowerWhip)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::RockWrecker)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::CrossPoison)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::GunkShot)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::IronHead)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::MagnetBomb)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::StoneEdge)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::ChargeBeam)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::WoodHammer)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::AquaJet)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::AttackOrder)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::HeadSmash)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::DoubleHit)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::RoarOfTime)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::SpacialRend)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::SeedFlare)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::OminousWind)].set(static_cast<int>(MoveFlag::HAS_POWER));
    flags[static_cast<int>(Move::ShadowForce)].set(static_cast<int>(MoveFlag::HAS_POWER));

    flags[static_cast<int>(Move::Absorb)].set(static_cast<int>(MoveFlag::HEALS_ATTACKER));
    flags[static_cast<int>(Move::MegaDrain)].set(static_cast<int>(MoveFlag::HEALS_ATTACKER));
    flags[static_cast<int>(Move::Recover)].set(static_cast<int>(MoveFlag::HEALS_ATTACKER));
    flags[static_cast<int>(Move::Softboiled)].set(static_cast<int>(MoveFlag::HEALS_ATTACKER));
    flags[static_cast<int>(Move::LeechLife)].set(static_cast<int>(MoveFlag::HEALS_ATTACKER));
    flags[static_cast<int>(Move::GigaDrain)].set(static_cast<int>(MoveFlag::HEALS_ATTACKER));
    flags[static_cast<int>(Move::MilkDrink)].set(static_cast<int>(MoveFlag::HEALS_ATTACKER));
    flags[static_cast<int>(Move::MorningSun)].set(static_cast<int>(MoveFlag::HEALS_ATTACKER));
    flags[static_cast<int>(Move::Synthesis)].set(static_cast<int>(MoveFlag::HEALS_ATTACKER));
    flags[static_cast<int>(Move::Moonlight)].set(static_cast<int>(MoveFlag::HEALS_ATTACKER));
    flags[static_cast<int>(Move::SlackOff)].set(static_cast<int>(MoveFlag::HEALS_ATTACKER));
    flags[static_cast<int>(Move::DrainPunch)].set(static_cast<int>(MoveFlag::HEALS_ATTACKER));
    flags[static_cast<int>(Move::HealOrder)].set(static_cast<int>(MoveFlag::HEALS_ATTACKER));

    flags[static_cast<int>(Move::SwordsDance)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
    flags[static_cast<int>(Move::Growth)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
    flags[static_cast<int>(Move::Meditate)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
    flags[static_cast<int>(Move::Agility)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
    flags[static_cast<int>(Move::DoubleTeam)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
    flags[static_cast<int>(Move::Harden)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
    flags[static_cast<int>(Move::Minimize)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
    flags[static_cast<int>(Move::Withdraw)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
    flags[static_cast<int>(Move::Amnesia)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
    flags[static_cast<int>(Move::AcidArmor)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
    flags[static_cast<int>(Move::Sharpen)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
    flags[static_cast<int>(Move::TailGlow)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
    flags[static_cast<int>(Move::CosmicPower)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
    flags[static_cast<int>(Move::IronDefense)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
    flags[static_cast<int>(Move::Howl)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
    flags[static_cast<int>(Move::BulkUp)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
    flags[static_cast<int>(Move::CalmMind)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
    flags[static_cast<int>(Move::DragonDance)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
    flags[static_cast<int>(Move::RockPolish)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
    flags[static_cast<int>(Move::NastyPlot)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));
    flags[static_cast<int>(Move::DefendOrder)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKER_STAT));

    flags[static_cast<int>(Move::SwordsDance)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_ATTACK));
    flags[static_cast<int>(Move::Meditate)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_ATTACK));
    flags[static_cast<int>(Move::Sharpen)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_ATTACK));
    flags[static_cast<int>(Move::BellyDrum)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_ATTACK));
    flags[static_cast<int>(Move::MetalClaw)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_ATTACK));
    flags[static_cast<int>(Move::MeteorMash)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_ATTACK));
    flags[static_cast<int>(Move::Howl)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_ATTACK));
    flags[static_cast<int>(Move::BulkUp)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_ATTACK));
    flags[static_cast<int>(Move::DragonDance)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_ATTACK));

    flags[static_cast<int>(Move::Superpower)].set(static_cast<int>(MoveFlag::LOWERS_ATTACKERS_ATTACK));

    flags[static_cast<int>(Move::Harden)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_DEFENSE));
    flags[static_cast<int>(Move::Withdraw)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_DEFENSE));
    flags[static_cast<int>(Move::SkullBash)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_DEFENSE));
    flags[static_cast<int>(Move::AcidArmor)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_DEFENSE));
    flags[static_cast<int>(Move::SteelWing)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_DEFENSE));
    flags[static_cast<int>(Move::CosmicPower)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_DEFENSE));
    flags[static_cast<int>(Move::IronDefense)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_DEFENSE));
    flags[static_cast<int>(Move::BulkUp)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_DEFENSE));
    flags[static_cast<int>(Move::DefendOrder)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_DEFENSE));

    flags[static_cast<int>(Move::Superpower)].set(static_cast<int>(MoveFlag::LOWERS_ATTACKERS_DEFENSE));
    flags[static_cast<int>(Move::CloseCombat)].set(static_cast<int>(MoveFlag::LOWERS_ATTACKERS_DEFENSE));

    flags[static_cast<int>(Move::Overheat)].set(static_cast<int>(MoveFlag::LOWERS_ATTACKERS_SPECIAL_ATTACK));
    flags[static_cast<int>(Move::PsychoBoost)].set(static_cast<int>(MoveFlag::LOWERS_ATTACKERS_SPECIAL_ATTACK));
    flags[static_cast<int>(Move::DracoMeteor)].set(static_cast<int>(MoveFlag::LOWERS_ATTACKERS_SPECIAL_ATTACK));
    flags[static_cast<int>(Move::LeafStorm)].set(static_cast<int>(MoveFlag::LOWERS_ATTACKERS_SPECIAL_ATTACK));

    flags[static_cast<int>(Move::Growth)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPECIAL_ATTACK));
    flags[static_cast<int>(Move::TailGlow)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPECIAL_ATTACK));
    flags[static_cast<int>(Move::CalmMind)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPECIAL_ATTACK));
    flags[static_cast<int>(Move::NastyPlot)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPECIAL_ATTACK));
    flags[static_cast<int>(Move::ChargeBeam)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPECIAL_ATTACK));

    flags[static_cast<int>(Move::Amnesia)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPECIAL_DEFENSE));
    flags[static_cast<int>(Move::CosmicPower)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPECIAL_DEFENSE));
    flags[static_cast<int>(Move::CalmMind)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPECIAL_DEFENSE));
    flags[static_cast<int>(Move::DefendOrder)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPECIAL_DEFENSE));

    flags[static_cast<int>(Move::CloseCombat)].set(static_cast<int>(MoveFlag::LOWERS_ATTACKERS_SPECIAL_DEFENSE));

    flags[static_cast<int>(Move::Agility)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPEED));
    flags[static_cast<int>(Move::DragonDance)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPEED));
    flags[static_cast<int>(Move::RockPolish)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_SPEED));

    flags[static_cast<int>(Move::HammerArm)].set(static_cast<int>(MoveFlag::LOWERS_ATTACKERS_SPEED));

    flags[static_cast<int>(Move::DoubleTeam)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_EVASION));
    flags[static_cast<int>(Move::Minimize)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_EVASION));

    flags[static_cast<int>(Move::FocusEnergy)].set(static_cast<int>(MoveFlag::BOOSTS_ATTACKERS_CRIT_RATE));

    flags[static_cast<int>(Move::Ancientpower)].set(static_cast<int>(MoveFlag::OMNI_BOOSTS_ATTACKER));
    flags[static_cast<int>(Move::SilverWind)].set(static_cast<int>(MoveFlag::OMNI_BOOSTS_ATTACKER));
    flags[static_cast<int>(Move::OminousWind)].set(static_cast<int>(MoveFlag::OMNI_BOOSTS_ATTACKER));

    flags[static_cast<int>(Move::Swagger)].set(static_cast<int>(MoveFlag::RAISES_DEFENDER_ATTACK));

    flags[static_cast<int>(Move::Growl)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_ATTACK));
    flags[static_cast<int>(Move::AuroraBeam)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_ATTACK));
    flags[static_cast<int>(Move::Charm)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_ATTACK));
    flags[static_cast<int>(Move::Memento)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_ATTACK));
    flags[static_cast<int>(Move::Featherdance)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_ATTACK));
    flags[static_cast<int>(Move::Tickle)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_ATTACK));

    flags[static_cast<int>(Move::Flatter)].set(static_cast<int>(MoveFlag::RAISES_DEFENDER_SPECIAL_ATTACK));

    flags[static_cast<int>(Move::Memento)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_ATTACK));
    flags[static_cast<int>(Move::MistBall)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_ATTACK));

    flags[static_cast<int>(Move::TailWhip)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_DEFENSE));
    flags[static_cast<int>(Move::Leer)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_DEFENSE));
    flags[static_cast<int>(Move::Screech)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_DEFENSE));
    flags[static_cast<int>(Move::IronTail)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_DEFENSE));
    flags[static_cast<int>(Move::Crunch)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_DEFENSE));
    flags[static_cast<int>(Move::RockSmash)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_DEFENSE));
    flags[static_cast<int>(Move::CrushClaw)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_DEFENSE));
    flags[static_cast<int>(Move::Tickle)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_DEFENSE));

    flags[static_cast<int>(Move::Acid)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE));
    flags[static_cast<int>(Move::Psychic)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE));
    flags[static_cast<int>(Move::ShadowBall)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE));
    flags[static_cast<int>(Move::LusterPurge)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE));
    flags[static_cast<int>(Move::FakeTears)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE));
    flags[static_cast<int>(Move::MetalSound)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE));
    flags[static_cast<int>(Move::BugBuzz)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE));
    flags[static_cast<int>(Move::FocusBlast)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE));
    flags[static_cast<int>(Move::EnergyBall)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE));
    flags[static_cast<int>(Move::EarthPower)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE));
    flags[static_cast<int>(Move::FlashCannon)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE));
    flags[static_cast<int>(Move::SeedFlare)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPECIAL_DEFENSE));

    flags[static_cast<int>(Move::Bubblebeam)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPEED));
    flags[static_cast<int>(Move::StringShot)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPEED));
    flags[static_cast<int>(Move::Constrict)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPEED));
    flags[static_cast<int>(Move::Bubble)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPEED));
    flags[static_cast<int>(Move::CottonSpore)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPEED));
    flags[static_cast<int>(Move::ScaryFace)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPEED));
    flags[static_cast<int>(Move::IcyWind)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPEED));
    flags[static_cast<int>(Move::RockTomb)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPEED));
    flags[static_cast<int>(Move::MudShot)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_SPEED));

    flags[static_cast<int>(Move::SandAttack)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_ACCURACY));
    flags[static_cast<int>(Move::Smokescreen)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_ACCURACY));
    flags[static_cast<int>(Move::Kinesis)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_ACCURACY));
    flags[static_cast<int>(Move::Flash)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_ACCURACY));
    flags[static_cast<int>(Move::MudSlap)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_ACCURACY));
    flags[static_cast<int>(Move::Octazooka)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_ACCURACY));
    flags[static_cast<int>(Move::MuddyWater)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_ACCURACY));
    flags[static_cast<int>(Move::MudBomb)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_ACCURACY));
    flags[static_cast<int>(Move::MirrorShot)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_ACCURACY));

    flags[static_cast<int>(Move::SweetScent)].set(static_cast<int>(MoveFlag::LOWERS_DEFENDER_EVASION));

    flags[static_cast<int>(Move::Blizzard)].set(static_cast<int>(MoveFlag::BYPASSES_ACCURACY));
    flags[static_cast<int>(Move::Thunder)].set(static_cast<int>(MoveFlag::BYPASSES_ACCURACY));
    flags[static_cast<int>(Move::Swift)].set(static_cast<int>(MoveFlag::BYPASSES_ACCURACY));
    flags[static_cast<int>(Move::FaintAttack)].set(static_cast<int>(MoveFlag::BYPASSES_ACCURACY));
    flags[static_cast<int>(Move::VitalThrow)].set(static_cast<int>(MoveFlag::BYPASSES_ACCURACY));
    flags[static_cast<int>(Move::ShadowPunch)].set(static_cast<int>(MoveFlag::BYPASSES_ACCURACY));
    flags[static_cast<int>(Move::AerialAce)].set(static_cast<int>(MoveFlag::BYPASSES_ACCURACY));
    flags[static_cast<int>(Move::MagicalLeaf)].set(static_cast<int>(MoveFlag::BYPASSES_ACCURACY));
    flags[static_cast<int>(Move::ShockWave)].set(static_cast<int>(MoveFlag::BYPASSES_ACCURACY));
    flags[static_cast<int>(Move::AuraSphere)].set(static_cast<int>(MoveFlag::BYPASSES_ACCURACY));
    flags[static_cast<int>(Move::MagnetBomb)].set(static_cast<int>(MoveFlag::BYPASSES_ACCURACY));

    flags[static_cast<int>(Move::Bounce)].set(static_cast<int>(MoveFlag::CANT_BE_USED_DURING_GRAVITY));
    flags[static_cast<int>(Move::Fly)].set(static_cast<int>(MoveFlag::CANT_BE_USED_DURING_GRAVITY));
    flags[static_cast<int>(Move::HighJumpKick)].set(static_cast<int>(MoveFlag::CANT_BE_USED_DURING_GRAVITY));
    flags[static_cast<int>(Move::JumpKick)].set(static_cast<int>(MoveFlag::CANT_BE_USED_DURING_GRAVITY));
    flags[static_cast<int>(Move::MagnetRise)].set(static_cast<int>(MoveFlag::CANT_BE_USED_DURING_GRAVITY));
    flags[static_cast<int>(Move::Splash)].set(static_cast<int>(MoveFlag::CANT_BE_USED_DURING_GRAVITY));

    flags[static_cast<int>(Move::Detect)].set(static_cast<int>(MoveFlag::PROTECTS_USER));
    flags[static_cast<int>(Move::Protect)].set(static_cast<int>(MoveFlag::PROTECTS_USER));

    flags[static_cast<int>(Move::Disable)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Mist)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::LowKick)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Counter)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::LeechSeed)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Rage)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Teleport)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Mimic)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::DefenseCurl)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Barrier)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::LightScreen)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Haze)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Reflect)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Bide)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Metronome)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::MirrorMove)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::DreamEater)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Transform)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Psywave)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Splash)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Rest)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Conversion)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::TriAttack)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::SuperFang)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Substitute)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Struggle)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Sketch)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::TripleKick)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Thief)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::SpiderWeb)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::MindReader)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Nightmare)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Snore)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Curse)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Flail)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Conversion2)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Reversal)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Spite)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Spikes)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Foresight)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::DestinyBond)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::PerishSong)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::LockOn)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Rollout)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::FalseSwipe)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::FuryCutter)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::MeanLook)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Attract)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::SleepTalk)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::HealBell)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Present)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Safeguard)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::PainSplit)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Magnitude)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::BatonPass)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Encore)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Pursuit)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::RapidSpin)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::HiddenPower)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::MirrorCoat)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::PsychUp)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::FutureSight)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::BeatUp)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::FakeOut)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Uproar)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Stockpile)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::SpitUp)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Swallow)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Torment)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Facade)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::FocusPunch)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Smellingsalt)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::FollowMe)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::NaturePower)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Charge)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Taunt)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::HelpingHand)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Trick)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::RolePlay)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Wish)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Assist)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Ingrain)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::MagicCoat)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Recycle)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Revenge)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::BrickBreak)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Yawn)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::KnockOff)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Endeavor)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Eruption)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::SkillSwap)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Imprison)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Refresh)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Grudge)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Snatch)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::SecretPower)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Camouflage)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::MudSport)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::IceBall)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::WeatherBall)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Aromatherapy)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::OdorSleuth)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::WaterSpout)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Block)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Covet)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::WaterSport)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::DoomDesire)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Roost)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Gravity)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::MiracleEye)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::WakeUpSlap)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::GyroBall)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::HealingWish)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Brine)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::NaturalGift)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Feint)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Pluck)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Tailwind)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Acupressure)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::MetalBurst)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::UTurn)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Payback)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Assurance)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Embargo)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Fling)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::PsychoShift)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::TrumpCard)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::HealBlock)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::WringOut)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::PowerTrick)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::GastroAcid)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::LuckyChant)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::MeFirst)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Copycat)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::PowerSwap)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::GuardSwap)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Punishment)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::LastResort)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::WorrySeed)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::SuckerPunch)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::ToxicSpikes)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::HeartSwap)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::AquaRing)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::MagnetRise)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Switcheroo)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Avalanche)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Defog)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::TrickRoom)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Captivate)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::StealthRock)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::GrassKnot)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Chatter)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::Judgment)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::BugBite)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::LunarDance)].set(static_cast<int>(MoveFlag::IS_OTHER));
    flags[static_cast<int>(Move::CrushGrip)].set(static_cast<int>(MoveFlag::IS_OTHER));

    flags[static_cast<int>(Move::Sandstorm)].set(static_cast<int>(MoveFlag::CHANGES_WEATHER));
    flags[static_cast<int>(Move::RainDance)].set(static_cast<int>(MoveFlag::CHANGES_WEATHER));
    flags[static_cast<int>(Move::SunnyDay)].set(static_cast<int>(MoveFlag::CHANGES_WEATHER));
    flags[static_cast<int>(Move::Hail)].set(static_cast<int>(MoveFlag::CHANGES_WEATHER));

    flags[static_cast<int>(Move::Whirlwind)].set(static_cast<int>(MoveFlag::FORCES_USER_OUT));
    flags[static_cast<int>(Move::Roar)].set(static_cast<int>(MoveFlag::FORCES_USER_OUT));

    flags[static_cast<int>(Move::Guillotine)].set(static_cast<int>(MoveFlag::KOS_IN_ONE_HIT));
    flags[static_cast<int>(Move::HornDrill)].set(static_cast<int>(MoveFlag::KOS_IN_ONE_HIT));
    flags[static_cast<int>(Move::Fissure)].set(static_cast<int>(MoveFlag::KOS_IN_ONE_HIT));
    flags[static_cast<int>(Move::SheerCold)].set(static_cast<int>(MoveFlag::KOS_IN_ONE_HIT));

    flags[static_cast<int>(Move::Selfdestruct)].set(static_cast<int>(MoveFlag::MAKES_ATTACKER_FAINT));
    flags[static_cast<int>(Move::Explosion)].set(static_cast<int>(MoveFlag::MAKES_ATTACKER_FAINT));
    flags[static_cast<int>(Move::Memento)].set(static_cast<int>(MoveFlag::MAKES_ATTACKER_FAINT));

    flags[static_cast<int>(Move::KarateChop)].set(static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE));
    flags[static_cast<int>(Move::RazorWind)].set(static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE));
    flags[static_cast<int>(Move::RazorLeaf)].set(static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE));
    flags[static_cast<int>(Move::SkyAttack)].set(static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE));
    flags[static_cast<int>(Move::Crabhammer)].set(static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE));
    flags[static_cast<int>(Move::Slash)].set(static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE));
    flags[static_cast<int>(Move::Aeroblast)].set(static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE));
    flags[static_cast<int>(Move::CrossChop)].set(static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE));
    flags[static_cast<int>(Move::BlazeKick)].set(static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE));
    flags[static_cast<int>(Move::AirCutter)].set(static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE));
    flags[static_cast<int>(Move::PoisonTail)].set(static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE));
    flags[static_cast<int>(Move::LeafBlade)].set(static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE));
    flags[static_cast<int>(Move::NightSlash)].set(static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE));
    flags[static_cast<int>(Move::ShadowClaw)].set(static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE));
    flags[static_cast<int>(Move::PsychoCut)].set(static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE));
    flags[static_cast<int>(Move::CrossPoison)].set(static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE));
    flags[static_cast<int>(Move::StoneEdge)].set(static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE));
    flags[static_cast<int>(Move::AttackOrder)].set(static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE));
    flags[static_cast<int>(Move::SpacialRend)].set(static_cast<int>(MoveFlag::HAS_INCREASED_CRIT_CHANCE));

    flags[static_cast<int>(Move::RazorWind)].set(static_cast<int>(MoveFlag::REQUIRES_CHARGING_TURN));
    flags[static_cast<int>(Move::SolarBeam)].set(static_cast<int>(MoveFlag::REQUIRES_CHARGING_TURN));
    flags[static_cast<int>(Move::SkullBash)].set(static_cast<int>(MoveFlag::REQUIRES_CHARGING_TURN));
    flags[static_cast<int>(Move::SkyAttack)].set(static_cast<int>(MoveFlag::REQUIRES_CHARGING_TURN));

    flags[static_cast<int>(Move::HyperBeam)].set(static_cast<int>(MoveFlag::REQUIRES_RECHARGE_TURN));
    flags[static_cast<int>(Move::BlastBurn)].set(static_cast<int>(MoveFlag::REQUIRES_RECHARGE_TURN));
    flags[static_cast<int>(Move::HydroCannon)].set(static_cast<int>(MoveFlag::REQUIRES_RECHARGE_TURN));
    flags[static_cast<int>(Move::FrenzyPlant)].set(static_cast<int>(MoveFlag::REQUIRES_RECHARGE_TURN));
    flags[static_cast<int>(Move::GigaImpact)].set(static_cast<int>(MoveFlag::REQUIRES_RECHARGE_TURN));
    flags[static_cast<int>(Move::RockWrecker)].set(static_cast<int>(MoveFlag::REQUIRES_RECHARGE_TURN));
    flags[static_cast<int>(Move::RoarOfTime)].set(static_cast<int>(MoveFlag::REQUIRES_RECHARGE_TURN));

    flags[static_cast<int>(Move::Stomp)].set(static_cast<int>(MoveFlag::DEALS_DOUBLE_AFTER_MINIMIZE));

    flags[static_cast<int>(Move::Bind)].set(static_cast<int>(MoveFlag::CONTINUES));
    flags[static_cast<int>(Move::Wrap)].set(static_cast<int>(MoveFlag::CONTINUES));
    flags[static_cast<int>(Move::FireSpin)].set(static_cast<int>(MoveFlag::CONTINUES));
    flags[static_cast<int>(Move::Clamp)].set(static_cast<int>(MoveFlag::CONTINUES));
    flags[static_cast<int>(Move::Whirlpool)].set(static_cast<int>(MoveFlag::CONTINUES));
    flags[static_cast<int>(Move::SandTomb)].set(static_cast<int>(MoveFlag::CONTINUES));
    flags[static_cast<int>(Move::MagmaStorm)].set(static_cast<int>(MoveFlag::CONTINUES));

    flags[static_cast<int>(Move::FirePunch)].set(static_cast<int>(MoveFlag::BURNS_DEFENDER));
    flags[static_cast<int>(Move::Ember)].set(static_cast<int>(MoveFlag::BURNS_DEFENDER));
    flags[static_cast<int>(Move::Flamethrower)].set(static_cast<int>(MoveFlag::BURNS_DEFENDER));
    flags[static_cast<int>(Move::FireBlast)].set(static_cast<int>(MoveFlag::BURNS_DEFENDER));
    flags[static_cast<int>(Move::FlameWheel)].set(static_cast<int>(MoveFlag::BURNS_DEFENDER));
    flags[static_cast<int>(Move::SacredFire)].set(static_cast<int>(MoveFlag::BURNS_DEFENDER));
    flags[static_cast<int>(Move::HeatWave)].set(static_cast<int>(MoveFlag::BURNS_DEFENDER));
    flags[static_cast<int>(Move::WillOWisp)].set(static_cast<int>(MoveFlag::BURNS_DEFENDER));
    flags[static_cast<int>(Move::BlazeKick)].set(static_cast<int>(MoveFlag::BURNS_DEFENDER));
    flags[static_cast<int>(Move::FlareBlitz)].set(static_cast<int>(MoveFlag::BURNS_DEFENDER));
    flags[static_cast<int>(Move::FireFang)].set(static_cast<int>(MoveFlag::BURNS_DEFENDER));
    flags[static_cast<int>(Move::LavaPlume)].set(static_cast<int>(MoveFlag::BURNS_DEFENDER));

    flags[static_cast<int>(Move::IcePunch)].set(static_cast<int>(MoveFlag::FREEZES_DEFENDER));
    flags[static_cast<int>(Move::IceBeam)].set(static_cast<int>(MoveFlag::FREEZES_DEFENDER));
    flags[static_cast<int>(Move::Blizzard)].set(static_cast<int>(MoveFlag::FREEZES_DEFENDER));
    flags[static_cast<int>(Move::PowderSnow)].set(static_cast<int>(MoveFlag::FREEZES_DEFENDER));
    flags[static_cast<int>(Move::IceFang)].set(static_cast<int>(MoveFlag::FREEZES_DEFENDER));

    flags[static_cast<int>(Move::ThunderPunch)].set(static_cast<int>(MoveFlag::PARALYZES_DEFENDER));
    flags[static_cast<int>(Move::BodySlam)].set(static_cast<int>(MoveFlag::PARALYZES_DEFENDER));
    flags[static_cast<int>(Move::StunSpore)].set(static_cast<int>(MoveFlag::PARALYZES_DEFENDER));
    flags[static_cast<int>(Move::Thundershock)].set(static_cast<int>(MoveFlag::PARALYZES_DEFENDER));
    flags[static_cast<int>(Move::Thunderbolt)].set(static_cast<int>(MoveFlag::PARALYZES_DEFENDER));
    flags[static_cast<int>(Move::ThunderWave)].set(static_cast<int>(MoveFlag::PARALYZES_DEFENDER));
    flags[static_cast<int>(Move::Thunder)].set(static_cast<int>(MoveFlag::PARALYZES_DEFENDER));
    flags[static_cast<int>(Move::Lick)].set(static_cast<int>(MoveFlag::PARALYZES_DEFENDER));
    flags[static_cast<int>(Move::Glare)].set(static_cast<int>(MoveFlag::PARALYZES_DEFENDER));
    flags[static_cast<int>(Move::ZapCannon)].set(static_cast<int>(MoveFlag::PARALYZES_DEFENDER));
    flags[static_cast<int>(Move::Spark)].set(static_cast<int>(MoveFlag::PARALYZES_DEFENDER));
    flags[static_cast<int>(Move::DragonBreath)].set(static_cast<int>(MoveFlag::PARALYZES_DEFENDER));
    flags[static_cast<int>(Move::Bounce)].set(static_cast<int>(MoveFlag::PARALYZES_DEFENDER));
    flags[static_cast<int>(Move::VoltTackle)].set(static_cast<int>(MoveFlag::PARALYZES_DEFENDER));
    flags[static_cast<int>(Move::ForcePalm)].set(static_cast<int>(MoveFlag::PARALYZES_DEFENDER));
    flags[static_cast<int>(Move::ThunderFang)].set(static_cast<int>(MoveFlag::PARALYZES_DEFENDER));
    flags[static_cast<int>(Move::Discharge)].set(static_cast<int>(MoveFlag::PARALYZES_DEFENDER));

    flags[static_cast<int>(Move::PoisonSting)].set(static_cast<int>(MoveFlag::POISONS_DEFENDER));
    flags[static_cast<int>(Move::Twineedle)].set(static_cast<int>(MoveFlag::POISONS_DEFENDER));
    flags[static_cast<int>(Move::PoisonPowder)].set(static_cast<int>(MoveFlag::POISONS_DEFENDER));
    flags[static_cast<int>(Move::Smog)].set(static_cast<int>(MoveFlag::POISONS_DEFENDER));
    flags[static_cast<int>(Move::Sludge)].set(static_cast<int>(MoveFlag::POISONS_DEFENDER));
    flags[static_cast<int>(Move::PoisonGas)].set(static_cast<int>(MoveFlag::POISONS_DEFENDER));
    flags[static_cast<int>(Move::SludgeBomb)].set(static_cast<int>(MoveFlag::POISONS_DEFENDER));
    flags[static_cast<int>(Move::PoisonTail)].set(static_cast<int>(MoveFlag::POISONS_DEFENDER));
    flags[static_cast<int>(Move::PoisonJab)].set(static_cast<int>(MoveFlag::POISONS_DEFENDER));
    flags[static_cast<int>(Move::CrossPoison)].set(static_cast<int>(MoveFlag::POISONS_DEFENDER));
    flags[static_cast<int>(Move::GunkShot)].set(static_cast<int>(MoveFlag::POISONS_DEFENDER));

    flags[static_cast<int>(Move::Toxic)].set(static_cast<int>(MoveFlag::BADLY_POISONS));
    flags[static_cast<int>(Move::PoisonFang)].set(static_cast<int>(MoveFlag::BADLY_POISONS));

    flags[static_cast<int>(Move::Sing)].set(static_cast<int>(MoveFlag::MAKES_DEFENDER_SLEEP));
    flags[static_cast<int>(Move::SleepPowder)].set(static_cast<int>(MoveFlag::MAKES_DEFENDER_SLEEP));
    flags[static_cast<int>(Move::Hypnosis)].set(static_cast<int>(MoveFlag::MAKES_DEFENDER_SLEEP));
    flags[static_cast<int>(Move::LovelyKiss)].set(static_cast<int>(MoveFlag::MAKES_DEFENDER_SLEEP));
    flags[static_cast<int>(Move::Spore)].set(static_cast<int>(MoveFlag::MAKES_DEFENDER_SLEEP));
    flags[static_cast<int>(Move::Grasswhistle)].set(static_cast<int>(MoveFlag::MAKES_DEFENDER_SLEEP));
    flags[static_cast<int>(Move::DarkVoid)].set(static_cast<int>(MoveFlag::MAKES_DEFENDER_SLEEP));

    flags[static_cast<int>(Move::Supersonic)].set(static_cast<int>(MoveFlag::CONFUSES_DEFENDER));
    flags[static_cast<int>(Move::Psybeam)].set(static_cast<int>(MoveFlag::CONFUSES_DEFENDER));
    flags[static_cast<int>(Move::Confusion)].set(static_cast<int>(MoveFlag::CONFUSES_DEFENDER));
    flags[static_cast<int>(Move::ConfuseRay)].set(static_cast<int>(MoveFlag::CONFUSES_DEFENDER));
    flags[static_cast<int>(Move::DizzyPunch)].set(static_cast<int>(MoveFlag::CONFUSES_DEFENDER));
    flags[static_cast<int>(Move::SweetKiss)].set(static_cast<int>(MoveFlag::CONFUSES_DEFENDER));
    flags[static_cast<int>(Move::Swagger)].set(static_cast<int>(MoveFlag::CONFUSES_DEFENDER));
    flags[static_cast<int>(Move::Dynamicpunch)].set(static_cast<int>(MoveFlag::CONFUSES_DEFENDER));
    flags[static_cast<int>(Move::Flatter)].set(static_cast<int>(MoveFlag::CONFUSES_DEFENDER));
    flags[static_cast<int>(Move::TeeterDance)].set(static_cast<int>(MoveFlag::CONFUSES_DEFENDER));
    flags[static_cast<int>(Move::SignalBeam)].set(static_cast<int>(MoveFlag::CONFUSES_DEFENDER));
    flags[static_cast<int>(Move::WaterPulse)].set(static_cast<int>(MoveFlag::CONFUSES_DEFENDER));
    flags[static_cast<int>(Move::RockClimb)].set(static_cast<int>(MoveFlag::CONFUSES_DEFENDER));

    flags[static_cast<int>(Move::Stomp)].set(static_cast<int>(MoveFlag::FLINCHES_DEFENDER));
    flags[static_cast<int>(Move::RollingKick)].set(static_cast<int>(MoveFlag::FLINCHES_DEFENDER));
    flags[static_cast<int>(Move::Headbutt)].set(static_cast<int>(MoveFlag::FLINCHES_DEFENDER));
    flags[static_cast<int>(Move::Bite)].set(static_cast<int>(MoveFlag::FLINCHES_DEFENDER));
    flags[static_cast<int>(Move::BoneClub)].set(static_cast<int>(MoveFlag::FLINCHES_DEFENDER));
    flags[static_cast<int>(Move::Waterfall)].set(static_cast<int>(MoveFlag::FLINCHES_DEFENDER));
    flags[static_cast<int>(Move::SkyAttack)].set(static_cast<int>(MoveFlag::FLINCHES_DEFENDER));
    flags[static_cast<int>(Move::HyperFang)].set(static_cast<int>(MoveFlag::FLINCHES_DEFENDER));
    flags[static_cast<int>(Move::Twister)].set(static_cast<int>(MoveFlag::FLINCHES_DEFENDER));
    flags[static_cast<int>(Move::NeedleArm)].set(static_cast<int>(MoveFlag::FLINCHES_DEFENDER));
    flags[static_cast<int>(Move::Astonish)].set(static_cast<int>(MoveFlag::FLINCHES_DEFENDER));
    flags[static_cast<int>(Move::Extrasensory)].set(static_cast<int>(MoveFlag::FLINCHES_DEFENDER));
    flags[static_cast<int>(Move::DarkPulse)].set(static_cast<int>(MoveFlag::FLINCHES_DEFENDER));
    flags[static_cast<int>(Move::AirSlash)].set(static_cast<int>(MoveFlag::FLINCHES_DEFENDER));
    flags[static_cast<int>(Move::DragonRush)].set(static_cast<int>(MoveFlag::FLINCHES_DEFENDER));
    flags[static_cast<int>(Move::ThunderFang)].set(static_cast<int>(MoveFlag::FLINCHES_DEFENDER));
    flags[static_cast<int>(Move::IceFang)].set(static_cast<int>(MoveFlag::FLINCHES_DEFENDER));
    flags[static_cast<int>(Move::FireFang)].set(static_cast<int>(MoveFlag::FLINCHES_DEFENDER));
    flags[static_cast<int>(Move::ZenHeadbutt)].set(static_cast<int>(MoveFlag::FLINCHES_DEFENDER));
    flags[static_cast<int>(Move::IronHead)].set(static_cast<int>(MoveFlag::FLINCHES_DEFENDER));

    flags[static_cast<int>(Move::JumpKick)].set(static_cast<int>(MoveFlag::HAS_RECOIL));
    flags[static_cast<int>(Move::TakeDown)].set(static_cast<int>(MoveFlag::HAS_RECOIL));
    flags[static_cast<int>(Move::DoubleEdge)].set(static_cast<int>(MoveFlag::HAS_RECOIL));
    flags[static_cast<int>(Move::Submission)].set(static_cast<int>(MoveFlag::HAS_RECOIL));
    flags[static_cast<int>(Move::HighJumpKick)].set(static_cast<int>(MoveFlag::HAS_RECOIL));
    flags[static_cast<int>(Move::BellyDrum)].set(static_cast<int>(MoveFlag::HAS_RECOIL));
    flags[static_cast<int>(Move::VoltTackle)].set(static_cast<int>(MoveFlag::HAS_RECOIL));
    flags[static_cast<int>(Move::FlareBlitz)].set(static_cast<int>(MoveFlag::HAS_RECOIL));
    flags[static_cast<int>(Move::BraveBird)].set(static_cast<int>(MoveFlag::HAS_RECOIL));
    flags[static_cast<int>(Move::WoodHammer)].set(static_cast<int>(MoveFlag::HAS_RECOIL));
    flags[static_cast<int>(Move::HeadSmash)].set(static_cast<int>(MoveFlag::HAS_RECOIL));

    flags[static_cast<int>(Move::DoubleSlap)].set(static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES));
    flags[static_cast<int>(Move::CometPunch)].set(static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES));
    flags[static_cast<int>(Move::DoubleKick)].set(static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES));
    flags[static_cast<int>(Move::FuryAttack)].set(static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES));
    flags[static_cast<int>(Move::Twineedle)].set(static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES));
    flags[static_cast<int>(Move::PinMissile)].set(static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES));
    flags[static_cast<int>(Move::SpikeCannon)].set(static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES));
    flags[static_cast<int>(Move::Barrage)].set(static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES));
    flags[static_cast<int>(Move::FuryAttack)].set(static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES));
    flags[static_cast<int>(Move::Bonemerang)].set(static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES));
    flags[static_cast<int>(Move::BoneRush)].set(static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES));
    flags[static_cast<int>(Move::ArmThrust)].set(static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES));
    flags[static_cast<int>(Move::BulletSeed)].set(static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES));
    flags[static_cast<int>(Move::IcicleSpear)].set(static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES));
    flags[static_cast<int>(Move::RockBlast)].set(static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES));
    flags[static_cast<int>(Move::DoubleHit)].set(static_cast<int>(MoveFlag::HITS_MULTIPLE_TIMES));

    flags[static_cast<int>(Move::Thrash)].set(static_cast<int>(MoveFlag::LASTS_MULTIPLE_TURNS));
    flags[static_cast<int>(Move::PetalDance)].set(static_cast<int>(MoveFlag::LASTS_MULTIPLE_TURNS));
    flags[static_cast<int>(Move::Outrage)].set(static_cast<int>(MoveFlag::LASTS_MULTIPLE_TURNS));

    flags[static_cast<int>(Move::SonicBoom)].set(static_cast<int>(MoveFlag::HAS_FIXED_DAMAGE));
    flags[static_cast<int>(Move::SeismicToss)].set(static_cast<int>(MoveFlag::HAS_FIXED_DAMAGE));
    flags[static_cast<int>(Move::DragonRage)].set(static_cast<int>(MoveFlag::HAS_FIXED_DAMAGE));
    flags[static_cast<int>(Move::NightShade)].set(static_cast<int>(MoveFlag::HAS_FIXED_DAMAGE));

    flags[static_cast<int>(Move::Bounce)].set(static_cast<int>(MoveFlag::GOES_INTO_AIR));
    flags[static_cast<int>(Move::Fly)].set(static_cast<int>(MoveFlag::GOES_INTO_AIR));

    flags[static_cast<int>(Move::Gust)].set(static_cast<int>(MoveFlag::HITS_DEFENDER_IN_AIR));
    flags[static_cast<int>(Move::SkyUppercut)].set(static_cast<int>(MoveFlag::HITS_DEFENDER_IN_AIR));
    flags[static_cast<int>(Move::Thunder)].set(static_cast<int>(MoveFlag::HITS_DEFENDER_IN_AIR));
    flags[static_cast<int>(Move::Twister)].set(static_cast<int>(MoveFlag::HITS_DEFENDER_IN_AIR));

    flags[static_cast<int>(Move::Dig)].set(static_cast<int>(MoveFlag::GOES_UNDER_GROUND));

    flags[static_cast<int>(Move::Earthquake)].set(static_cast<int>(MoveFlag::HITS_DEFENDER_UNDER_GROUND));
    flags[static_cast<int>(Move::Magnitude)].set(static_cast<int>(MoveFlag::HITS_DEFENDER_UNDER_GROUND));
    flags[static_cast<int>(Move::Fissure)].set(static_cast<int>(MoveFlag::HITS_DEFENDER_UNDER_GROUND));

    flags[static_cast<int>(Move::Dive)].set(static_cast<int>(MoveFlag::GOES_UNDER_WATER));

    flags[static_cast<int>(Move::Surf)].set(static_cast<int>(MoveFlag::HITS_DEFENDER_UNDER_WATER));
    flags[static_cast<int>(Move::Whirlpool)].set(static_cast<int>(MoveFlag::HITS_DEFENDER_UNDER_WATER));

    flags[static_cast<int>(Move::ShadowForce)].set(static_cast<int>(MoveFlag::GOES_INTO_VOID));

    flags[static_cast<int>(Move::Pound)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::KarateChop)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::DoubleSlap)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::CometPunch)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::MegaPunch)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::FirePunch)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::IcePunch)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::ThunderPunch)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Scratch)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::ViceGrip)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Guillotine)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Cut)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::WingAttack)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Fly)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Bind)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Slam)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::VineWhip)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Stomp)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::DoubleKick)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::MegaKick)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::JumpKick)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::RollingKick)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Headbutt)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::HornAttack)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::FuryAttack)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::HornDrill)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Tackle)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::BodySlam)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Wrap)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::TakeDown)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Thrash)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::DoubleEdge)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Bite)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Peck)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::DrillPeck)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Submission)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::LowKick)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Counter)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::SeismicToss)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Strength)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::PetalDance)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Dig)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::QuickAttack)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Rage)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Bide)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Lick)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Waterfall)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Clamp)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::SkullBash)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Constrict)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::HighJumpKick)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::LeechLife)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::DizzyPunch)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Crabhammer)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::FurySwipes)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::HyperFang)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::SuperFang)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Slash)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Struggle)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::TripleKick)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Thief)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::FlameWheel)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Flail)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Reversal)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::MachPunch)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::FaintAttack)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Outrage)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Rollout)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::FalseSwipe)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Spark)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::FuryCutter)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::SteelWing)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Return)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Frustration)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Dynamicpunch)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Megahorn)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Pursuit)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::RapidSpin)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::IronTail)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::MetalClaw)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::VitalThrow)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::CrossChop)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Crunch)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::ExtremeSpeed)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::RockSmash)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::FakeOut)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Facade)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::FocusPunch)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Smellingsalt)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Superpower)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Revenge)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::BrickBreak)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::KnockOff)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Endeavor)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Dive)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::ArmThrust)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::BlazeKick)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::IceBall)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::NeedleArm)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::PoisonFang)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::CrushClaw)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::MeteorMash)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Astonish)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::ShadowPunch)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::SkyUppercut)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::AerialAce)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::DragonClaw)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Bounce)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::PoisonTail)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Covet)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::VoltTackle)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::LeafBlade)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::WakeUpSlap)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::HammerArm)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::GyroBall)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Pluck)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::UTurn)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::CloseCombat)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Payback)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Assurance)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Punishment)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::LastResort)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::SuckerPunch)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::FlareBlitz)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::ForcePalm)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::PoisonJab)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::NightSlash)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::AquaTail)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::XScissor)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::DragonRush)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::DrainPunch)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::BraveBird)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::GigaImpact)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::BulletPunch)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::Avalanche)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::ShadowClaw)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::ThunderFang)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::IceFang)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::FireFang)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::ShadowSneak)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::ZenHeadbutt)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::RockClimb)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::PowerWhip)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::CrossPoison)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::IronHead)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::BugBite)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::WoodHammer)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::AquaJet)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::HeadSmash)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::DoubleHit)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::CrushGrip)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));
    flags[static_cast<int>(Move::ShadowForce)].set(static_cast<int>(MoveFlag::MAKES_CONTACT));

    flags[static_cast<int>(Move::Acupressure)].set(static_cast<int>(MoveFlag::BYPASSES_PROTECT));
    flags[static_cast<int>(Move::Curse)].set(static_cast<int>(MoveFlag::BYPASSES_PROTECT));
    flags[static_cast<int>(Move::DoomDesire)].set(static_cast<int>(MoveFlag::BYPASSES_PROTECT));
    flags[static_cast<int>(Move::Feint)].set(static_cast<int>(MoveFlag::BYPASSES_PROTECT));
    flags[static_cast<int>(Move::FutureSight)].set(static_cast<int>(MoveFlag::BYPASSES_PROTECT));
    flags[static_cast<int>(Move::PerishSong)].set(static_cast<int>(MoveFlag::BYPASSES_PROTECT));
    flags[static_cast<int>(Move::PsychUp)].set(static_cast<int>(MoveFlag::BYPASSES_PROTECT));
    flags[static_cast<int>(Move::RolePlay)].set(static_cast<int>(MoveFlag::BYPASSES_PROTECT));
    flags[static_cast<int>(Move::ShadowForce)].set(static_cast<int>(MoveFlag::BYPASSES_PROTECT));
    flags[static_cast<int>(Move::Sketch)].set(static_cast<int>(MoveFlag::BYPASSES_PROTECT));
    flags[static_cast<int>(Move::Transform)].set(static_cast<int>(MoveFlag::BYPASSES_PROTECT));

    flags[static_cast<int>(Move::Feint)].set(static_cast<int>(MoveFlag::BREAKS_PROTECT));
    flags[static_cast<int>(Move::ShadowForce)].set(static_cast<int>(MoveFlag::BREAKS_PROTECT));

    flags[static_cast<int>(Move::Attract)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::Block)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::Captivate)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::Charm)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::ConfuseRay)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::CottonSpore)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::DarkVoid)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::FakeTears)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::Featherdance)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::Flash)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::Flatter)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::GastroAcid)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::Glare)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::Grasswhistle)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::Growl)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::Hypnosis)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::Kinesis)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::LeechSeed)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::Leer)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::LovelyKiss)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::MeanLook)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::MetalSound)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::PoisonGas)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::PoisonPowder)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::SandAttack)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::ScaryFace)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::Screech)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::Sing)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::SleepPowder)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::Smokescreen)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::SpiderWeb)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::Spore)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::StringShot)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::StunSpore)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::Supersonic)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::Swagger)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::SweetKiss)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::SweetScent)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::TailWhip)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::ThunderWave)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::Tickle)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::Toxic)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::WillOWisp)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::WorrySeed)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));
    flags[static_cast<int>(Move::Yawn)].set(static_cast<int>(MoveFlag::CAN_BE_REFLECTED_BY_MIRROR_COAT));

    flags[static_cast<int>(Move::AcidArmor)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Acupressure)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Agility)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Amnesia)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Aromatherapy)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Barrier)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::BellyDrum)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::BulkUp)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::CalmMind)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Camouflage)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Charge)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::CosmicPower)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::DefendOrder)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::DefenseCurl)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::DoubleTeam)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::DragonDance)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::FocusEnergy)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Growth)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Harden)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::HealBell)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::HealOrder)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Howl)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Ingrain)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::IronDefense)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::LightScreen)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Meditate)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::MilkDrink)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Minimize)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Mist)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Moonlight)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::MorningSun)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::NastyPlot)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::PsychUp)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Recover)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Reflect)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Refresh)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Rest)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::RockPolish)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Roost)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Safeguard)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Sharpen)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::SlackOff)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Softboiled)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Stockpile)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Substitute)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Swallow)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::SwordsDance)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Synthesis)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::TailGlow)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Tailwind)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));
    flags[static_cast<int>(Move::Withdraw)].set(static_cast<int>(MoveFlag::CAN_BE_SNATCHED));

    flags[static_cast<int>(Move::AerialAce)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Aeroblast)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::AirCutter)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::AirSlash)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::AquaJet)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::AquaTail)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::ArmThrust)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Assurance)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::AttackOrder)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::AuraSphere)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Avalanche)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Barrage)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::BeatUp)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Bide)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Bind)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::BlastBurn)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::BoneRush)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Bonemerang)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Bounce)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::BraveBird)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::BrickBreak)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Brine)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::BugBite)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::BulletPunch)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::BulletSeed)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::ChargeBeam)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Clamp)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::CloseCombat)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::CometPunch)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Crabhammer)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::CrossChop)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::CrossPoison)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::CrushGrip)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Cut)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::DarkPulse)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Dig)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Discharge)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Dive)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::DoubleHit)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::DoubleKick)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::DoubleSlap)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::DoubleEdge)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::DracoMeteor)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::DragonBreath)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::DragonClaw)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::DragonPulse)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::DragonRush)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::DrainPunch)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::DrillPeck)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::EarthPower)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Earthquake)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::EggBomb)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Endeavor)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Eruption)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Explosion)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::ExtremeSpeed)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::FalseSwipe)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::FaintAttack)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::FireFang)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::FireSpin)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Flail)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::FlashCannon)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Fly)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::ForcePalm)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::FrenzyPlant)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Frustration)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::FuryAttack)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::FuryCutter)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::FurySwipes)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::GigaImpact)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::GrassKnot)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::GunkShot)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::GyroBall)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::HammerArm)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::HeadSmash)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::HiddenPower)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::HighJumpKick)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::HornAttack)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::HydroCannon)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::HydroPump)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::HyperBeam)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::IceBall)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::IceFang)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::IceShard)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::IcicleSpear)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::IronHead)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Judgment)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::JumpKick)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::KarateChop)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::LastResort)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::LavaPlume)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::LeafBlade)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::LeafStorm)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::LowKick)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::MachPunch)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::MagicalLeaf)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::MagmaStorm)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::MagnetBomb)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Magnitude)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::MegaKick)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::MegaPunch)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Megahorn)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::MeteorMash)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::MirrorCoat)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::MirrorShot)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::MudBomb)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::MudShot)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::MuddyWater)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::NightShade)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::NightSlash)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::OminousWind)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Outrage)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Overheat)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::PayDay)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Payback)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Peck)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::PetalDance)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::PinMissile)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Pluck)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::PoisonJab)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::PoisonTail)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::PowerGem)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::PowerWhip)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::PsychoBoost)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::PsychoCut)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Psywave)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Punishment)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::QuickAttack)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Rage)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::RapidSpin)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::RazorLeaf)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::RazorWind)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Return)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Revenge)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Reversal)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::RoarOfTime)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::RockBlast)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::RockClimb)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::RockThrow)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::RockWrecker)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::RollingKick)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Rollout)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::SandTomb)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Scratch)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::SeedBomb)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::SeedFlare)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::SeismicToss)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::ShadowClaw)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::ShadowForce)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::ShadowPunch)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::ShadowSneak)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::ShockWave)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::SignalBeam)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::SilverWind)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::SkullBash)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::SkyAttack)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::SkyUppercut)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Slam)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Slash)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Snore)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::SolarBeam)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::SonicBoom)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::SpacialRend)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::SpikeCannon)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::SpitUp)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::SteelWing)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::StoneEdge)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Strength)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Struggle)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Submission)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::SuckerPunch)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Surf)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Swift)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Tackle)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::TakeDown)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Thrash)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::ThunderFang)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::TripleKick)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::TrumpCard)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Twister)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::UTurn)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Uproar)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::VacuumWave)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::ViceGrip)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::VineWhip)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::VitalThrow)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::VoltTackle)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::WakeUpSlap)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::WaterGun)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::WaterPulse)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Waterfall)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::WeatherBall)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Whirlpool)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::WingAttack)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::WoodHammer)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::Wrap)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::WringOut)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    flags[static_cast<int>(Move::XScissor)].set(static_cast<int>(MoveFlag::AFFECTED_BY_KINGS_ROCK));
    return flags;
}();

static constexpr std::array<int, static_cast<int>(Move::Count)> MOVE_PRIORITIES = [] {
    std::array<int, static_cast<int>(Move::Count)> array{};
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

static void verify_map() {
    std::unordered_set<Move> moves;
    for (int i = 0; i < static_cast<int>(Move::Count); ++i) {
        moves.insert(static_cast<Move>(i));
    }
    for (Move value : MOVE_MAP | std::ranges::views::values) {
        moves.erase(value);
    }
    if (!moves.empty()) {
        throw std::logic_error("Invalid move map");
    }
}

#endif //ATTACKS_H
