#ifndef GUARD_LEVEL_SCALING_H
#define GUARD_LEVEL_SCALING_H

#include "config/level_scaling.h"

enum LevelScalingMode {
    LEVEL_SCALING_NONE,
    LEVEL_SCALING_TO_LEVEL_CAP,
    LEVEL_SCALING_PARTY_AVG,
    LEVEL_SCALING_PARTY_HIGHEST,
    LEVEL_SCALING_PARTY_LOWEST,
    LEVEL_SCALING_PARTY_OR_PC_HIGHEST,
};

enum PartySizeScalingMode {
    PARTY_SIZE_SCALING_INHERIT,
    PARTY_SIZE_SCALING_NONE,
    PARTY_SIZE_SCALING_BY_PLAYER_SIZE,
    PARTY_SIZE_SCALING_BY_PLAYER_LEVEL,
};

enum PartySizeSortMode {
    PARTY_SIZE_SORT_INHERIT,
    PARTY_SIZE_SORT_NONE,
    PARTY_SIZE_SORT_KEEP_HIGHEST_BST,
    PARTY_SIZE_SORT_KEEP_LOWEST_BST,
    PARTY_SIZE_SORT_RANDOM,
};

enum MoveProgressionTier {
    MOVE_TIER_DEFAULT,
    MOVE_TIER_MID,
    MOVE_TIER_LATE,
    MOVE_TIER_ENDGAME,
};

enum ItemProgressionTier {
    ITEM_TIER_DEFAULT,
    ITEM_TIER_MID,
    ITEM_TIER_LATE,
    ITEM_TIER_ENDGAME,
};

struct LevelScalingConfig {
    u8 mode;
    s8 levelAugmentAdd;
    u8 levelVariationPct;
    u8 minLevel;
    u8 maxLevel;
    bool8 manageEvolutions;
    bool8 excludeFainted;
    bool8 scaleEVs;
    bool8 scaleMoves;
    bool8 scaleItems;
    u8 partySizeMode;
    u8 partySizeSort;
};

struct EvolutionOverride {
    u16 species;
    u8 minimumLevel;
    u8 padding;
};

struct PartySizeTier {
    u8 maxAvgLevel;
    u8 maxPartySize;
};

#define LEVEL_SCALING_CONFIG_NONE \
    { .mode = LEVEL_SCALING_NONE, .padding = 0xFF }

#define LEVEL_SCALING_CONFIG_LEVEL_CAP \
    { .mode = LEVEL_SCALING_TO_LEVEL_CAP, .manageEvolutions = TRUE }

#define LEVEL_SCALING_CONFIG_PARTY_AVG \
    { .mode = LEVEL_SCALING_PARTY_AVG, .manageEvolutions = TRUE }

#define LEVEL_SCALING_CONFIG_PARTY_HIGHEST \
    { .mode = LEVEL_SCALING_PARTY_HIGHEST, .manageEvolutions = TRUE }

u8 CalculatePlayerPartyBaseLevel(u8 mode, bool8 excludeFainted);
u8 CalculateScaledLevel(const struct LevelScalingConfig *config, u8 baseLevel);
const struct LevelScalingConfig *GetTrainerLevelScalingConfig(u16 trainerId);
u16 ValidateSpeciesForLevel(u16 species, u8 level, bool8 manageEvolutions);
u8 GetEvolutionLevelForSpecies(u16 species);
bool8 IsSpeciesLegalAtLevel(u16 species, u8 level);
u16 GetPreEvolution(u16 species);
const struct EvolutionOverride *GetEvolutionOverride(u16 species);
void InvalidatePartyLevelCache(void);
u8 CalculateWildScaledLevel(u16 species, u8 level);
u16 CalculateWildScaledSpecies(u16 species, u8 level);
bool8 TryApplyScaledTrainerEVs(struct Pokemon *mon, const struct LevelScalingConfig *config, u8 scaledLevel);
bool8 IsMovePermittedAtLevel(u16 move, u8 level);
void MaybeFilterTrainerMoves(struct Pokemon *mon, u16 species, u8 scaledLevel, const struct LevelScalingConfig *config);

#endif // GUARD_LEVEL_SCALING_H