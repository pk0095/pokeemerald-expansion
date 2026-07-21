#include "global.h"
#include "level_scaling.h"
#include "pokemon.h"
#include "battle.h"
#include "trainer_hill.h"
#include "random.h"
#include "data/level_scaling_evolution_overrides.h"

static u8 sCachedPartyBaseLevel = 0;
static bool8 sPartyLevelCacheValid = FALSE;

void InvalidatePartyLevelCache(void)
{
    sPartyLevelCacheValid = FALSE;
}

u8 CalculatePlayerPartyBaseLevel(u8 mode, bool8 excludeFainted)
{
    u8 i;
    u16 sum = 0;
    u8 count = 0;
    u8 highest = 0;
    u8 lowest = MAX_LEVEL;
    struct Pokemon *party = *gPlayerPartyPtr;

    if (mode == LEVEL_SCALING_PARTY_AVG && sPartyLevelCacheValid)
        return sCachedPartyBaseLevel;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        u16 species = GetMonData(&party[i], MON_DATA_SPECIES, NULL);
        u8 level;

        if (species == SPECIES_NONE)
            continue;
        if (excludeFainted && GetMonData(&party[i], MON_DATA_HP, NULL) == 0)
            continue;

        level = GetMonData(&party[i], MON_DATA_LEVEL, NULL);
        sum += level;
        count++;

        if (level > highest)
            highest = level;
        if (level < lowest)
            lowest = level;
    }

    if (count == 0)
        return 5; // fallback, empty/all-fainted party

    switch (mode)
    {
    case LEVEL_SCALING_PARTY_HIGHEST:
        return highest;
    case LEVEL_SCALING_PARTY_LOWEST:
        return lowest;
    case LEVEL_SCALING_PARTY_OR_PC_HIGHEST:
        // PC box scan not yet implemented; fall through to party highest
        return highest;
    case LEVEL_SCALING_PARTY_AVG:
    default:
        sCachedPartyBaseLevel = (u8)(sum / count);
        sPartyLevelCacheValid = TRUE;
        return sCachedPartyBaseLevel;
    }
}

static s32 ApplyLevelVariation(s32 level, u8 variationPct)
{
    s32 range;
    s32 roll;

    if (variationPct == 0)
        return level;

    range = (level * variationPct) / 100;
    if (range < 1)
        range = 1;

    roll = (Random() % (range * 2 + 1)) - range;
    level += roll;

    if (level < 1)
        level = 1;
    if (level > MAX_LEVEL)
        level = MAX_LEVEL;

    return level;
}

u8 CalculateScaledLevel(const struct LevelScalingConfig *config, u8 baseLevel)
{
    s32 level;

    if (config == NULL || config->mode == LEVEL_SCALING_NONE)
        return baseLevel;

    level = baseLevel + config->levelAugmentAdd;
    level = ApplyLevelVariation(level, config->levelVariationPct);

    if (config->minLevel != 0 && level < config->minLevel)
        level = config->minLevel;
    if (config->maxLevel != 0 && level > config->maxLevel)
        level = config->maxLevel;

    if (level < 1)
        level = 1;
    if (level > MAX_LEVEL)
        level = MAX_LEVEL;

    return (u8)level;
}

// Per-trainer config table is defined per-trainer in trainer data (future hookup).
// For now this resolves to the global default config from level_scaling.h.
static const struct LevelScalingConfig sDefaultTrainerConfig = {
    .mode = B_TRAINER_SCALING_DEFAULT_MODE,
    .levelAugmentAdd = B_TRAINER_SCALING_LEVEL_AUGMENT,
    .levelVariationPct = B_TRAINER_SCALING_LEVEL_VARIATION_PCT,
    .minLevel = B_TRAINER_SCALING_MIN_LEVEL,
    .maxLevel = B_TRAINER_SCALING_MAX_LEVEL,
    .manageEvolutions = B_TRAINER_SCALING_MANAGE_EVOLUTIONS,
    .excludeFainted = B_TRAINER_SCALING_EXCLUDE_FAINTED,
    .scaleEVs = B_TRAINER_SCALING_SCALE_EVS,
    .scaleMoves = B_TRAINER_SCALING_SCALE_MOVES,
    .scaleItems = B_TRAINER_SCALING_SCALE_ITEMS,
};

static const u16 sScalingExcludedTrainerIds[] =
{
    TRAINER_BRENDAN_ROUTE_103_MUDKIP,
    TRAINER_BRENDAN_ROUTE_103_TREECKO,
    TRAINER_BRENDAN_ROUTE_103_TORCHIC,
    TRAINER_MAY_ROUTE_103_MUDKIP,
    TRAINER_MAY_ROUTE_103_TREECKO,
    TRAINER_MAY_ROUTE_103_TORCHIC,
};

const struct LevelScalingConfig *GetTrainerLevelScalingConfig(u16 trainerId)
{
    u32 i;

    if (!B_TRAINER_SCALING_ENABLED)
        return NULL;

    for (i = 0; i < ARRAY_COUNT(sScalingExcludedTrainerIds); i++)
    {
        if (sScalingExcludedTrainerIds[i] == trainerId)
            return NULL; // scaling skipped entirely, mon uses its defined level/species as-is
    }

    // TODO: hook per-trainer override table here once trainer data supports it.
    return &sDefaultTrainerConfig;
}

const struct EvolutionOverride *GetEvolutionOverride(u16 species)
{
    u32 i;

    if (!B_SCALING_USE_OVERRIDES)
        return NULL;

    for (i = 0; i < ARRAY_COUNT(sEvolutionOverrides); i++)
    {
        if (sEvolutionOverrides[i].species == species)
            return &sEvolutionOverrides[i];
    }
    return NULL;
}

u8 GetEvolutionLevelForSpecies(u16 species)
{
    const struct EvolutionOverride *override = GetEvolutionOverride(species);
    u32 i;

    if (override != NULL)
        return override->minimumLevel;

    // Fall back to standard EVO_LEVEL entries pointing at this species
    // by scanning all species for one whose evolution target is `species`.
    for (i = 0; i < NUM_SPECIES; i++)
    {
        const struct Evolution *evolutions;
        u32 j;
        if (!IsSpeciesEnabled(i))
            continue;
        evolutions = GetSpeciesEvolutions(i);
        if (evolutions == NULL)
            continue;
        for (j = 0; evolutions[j].method != EVOLUTIONS_END; j++)
        {
            if (evolutions[j].method == EVO_LEVEL
             && evolutions[j].targetSpecies == species)
            {
                return evolutions[j].param;
            }
        }
    }

    return 0; // no level requirement found (base-stage species)
}

u16 GetPreEvolution(u16 species)
{
    u32 i, j;

    for (i = 0; i < NUM_SPECIES; i++)
    {
        const struct Evolution *evolutions;
        if (!IsSpeciesEnabled(i))
            continue;
        evolutions = GetSpeciesEvolutions(i);
        if (evolutions == NULL)
            continue;
        for (j = 0; evolutions[j].method != EVOLUTIONS_END; j++)
        {
            if (evolutions[j].targetSpecies == species)
                return i;
        }
    }
    return SPECIES_NONE;
}

bool8 IsSpeciesLegalAtLevel(u16 species, u8 level)
{
    u8 requiredLevel = GetEvolutionLevelForSpecies(species);

    if (requiredLevel == 0)
        return TRUE; // base-stage or no level gate

    return (level >= requiredLevel);
}

u16 ValidateSpeciesForLevel(u16 species, u8 level, bool8 manageEvolutions)
{
    u16 current = species;

    if (!manageEvolutions)
        return species;

    // Step 1: walk backwards through pre-evolutions until the species is legal at this level.
    while (!IsSpeciesLegalAtLevel(current, level))
    {
        u16 pre = GetPreEvolution(current);
        if (pre == SPECIES_NONE || pre == current)
            break; // can't devolve further, accept as-is
        current = pre;
    }

    // Step 2: walk forward, evolving as far as the level legally allows.
    for (;;)
    {
        const struct Evolution *evolutions;
        u16 nextSpecies = SPECIES_NONE;
        u32 j;

        if (!IsSpeciesEnabled(current))
            break;

        evolutions = GetSpeciesEvolutions(current);
        if (evolutions == NULL)
            break;

        for (j = 0; evolutions[j].method != EVOLUTIONS_END; j++)
        {
            u16 target = evolutions[j].targetSpecies;
            if (SanitizeSpeciesId(target) == SPECIES_NONE)
                continue;
            if (IsSpeciesLegalAtLevel(target, level))
            {
                nextSpecies = target;
                break; // take first eligible evolution branch
            }
        }

        if (nextSpecies == SPECIES_NONE || nextSpecies == current)
            break;

        current = nextSpecies;
    }

    return current;
}

u8 CalculateWildScaledLevel(u16 species, u8 level)
{
    const struct LevelScalingConfig config = LEVEL_SCALING_CONFIG_PARTY_AVG;
    u8 baseLevel;
    u8 scaledLevel;

    if (!B_WILD_SCALING_ENABLED)
        return level;

    baseLevel = CalculatePlayerPartyBaseLevel(B_WILD_SCALING_DEFAULT_MODE, B_WILD_SCALING_EXCLUDE_FAINTED);

    // Wild mons scale off the player's party, not their own listed level,
    // but never scale *below* their own defined minimum (encounter table floor).
    scaledLevel = CalculateScaledLevel(&config, baseLevel);
    if (scaledLevel < level)
        scaledLevel = level;

    return scaledLevel;
}

u16 CalculateWildScaledSpecies(u16 species, u8 level)
{
    if (!B_WILD_SCALING_MANAGE_EVOLUTIONS)
        return species;

    return ValidateSpeciesForLevel(species, level, TRUE);
}

bool8 TryApplyScaledTrainerEVs(struct Pokemon *mon, const struct LevelScalingConfig *config, u8 scaledLevel)
{
    u8 i;
    u16 evTotal;
    u16 evPerStat;

    if (config == NULL || !config->scaleEVs)
        return FALSE;

    // Simple even spread scaled to level ratio; replace with per-species EV yield logic if desired.
    evTotal = (MAX_TOTAL_EVS * scaledLevel) / MAX_LEVEL;
    evPerStat = evTotal / NUM_STATS;

    for (i = 0; i < NUM_STATS; i++)
        SetMonData(mon, MON_DATA_HP_EV + i, &evPerStat);

    return TRUE;
}

bool8 IsMovePermittedAtLevel(u16 move, u8 level)
{
    // Placeholder: always permitted. Hook into a move-level requirement
    // table here if you want scaled trainers to be denied late-game moves
    // they wouldn't "legally" have learned yet at a lower scaled level.
    return TRUE;
}

void MaybeFilterTrainerMoves(struct Pokemon *mon, u16 species, u8 scaledLevel, const struct LevelScalingConfig *config)
{
    u8 i;

    if (config == NULL || !config->scaleMoves)
        return;

    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        u16 move = GetMonData(mon, MON_DATA_MOVE1 + i, NULL);
        if (move != MOVE_NONE && !IsMovePermittedAtLevel(move, scaledLevel))
            SetMonData(mon, MON_DATA_MOVE1 + i, &(u16){MOVE_NONE});
    }
}