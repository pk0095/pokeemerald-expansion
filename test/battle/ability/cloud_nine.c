#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("ABILITY: Cloud Nine/Air Lock prevent basic weather effects, but without them disappearing - Sandstorm")
{
    u32 species = 0, ability = 0;
    PARAMETRIZE { species = SPECIES_PSYDUCK;  ability = ABILITY_CLOUD_NINE; }
    PARAMETRIZE { species = SPECIES_RAYQUAZA; ability = ABILITY_AIR_LOCK; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SANDSTORM) == EFFECT_SANDSTORM);
        PLAYER(species) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SANDSTORM); }
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, ability);
        MESSAGE("The effects of the weather disappeared.");
        MESSAGE("The opposing Wobbuffet used Sandstorm!");
        MESSAGE("The sandstorm is raging.");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SANDSTORM_CONTINUES);
        NONE_OF {
            HP_BAR(player);
            HP_BAR(opponent);
            MESSAGE("The opposing Wobbuffet is buffeted by the sandstorm!");
        }
        MESSAGE("The sandstorm is raging.");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SANDSTORM_CONTINUES);
    }
}

TO_DO_BATTLE_TEST("ABILITY: Cloud Nine/Air Lock prevent basic weather effects, but without them disappearing - Sun");
TO_DO_BATTLE_TEST("ABILITY: Cloud Nine/Air Lock prevent basic weather effects, but without them disappearing - Rain");
TO_DO_BATTLE_TEST("ABILITY: Cloud Nine/Air Lock prevent basic weather effects, but without them disappearing - Hail");
TO_DO_BATTLE_TEST("ABILITY: Cloud Nine/Air Lock prevent basic weather effects, but without them disappearing - Snow");
TO_DO_BATTLE_TEST("ABILITY: Cloud Nine/Air Lock prevent basic weather effects, but without them disappearing - Fog");
TO_DO_BATTLE_TEST("ABILITY: Cloud Nine/Air Lock prevent basic weather effects, but without them disappearing - Primal Sun");
TO_DO_BATTLE_TEST("ABILITY: Cloud Nine/Air Lock prevent basic weather effects, but without them disappearing - Primal Rain");
TO_DO_BATTLE_TEST("ABILITY: Cloud Nine/Air Lock prevent basic weather effects, but without them disappearing - Strong Winds");

// Moves and abilities that are affected by weather should have new tests that check for Clould Nine/Air Lock, like Mold-Breaker Abilities

SINGLE_BATTLE_TEST("INNATE: Cloud Nine/Air Lock prevent basic weather effects, but without them disappearing - Sandstorm")
{
    u32 species = 0, ability = 0, innate = 0;
    PARAMETRIZE { species = SPECIES_PSYDUCK;  ability = ABILITY_DAMP; innate = ABILITY_CLOUD_NINE; }
    PARAMETRIZE { species = SPECIES_RAYQUAZA; ability = ABILITY_LIGHT_METAL; innate = ABILITY_AIR_LOCK; }
    GIVEN {
        ASSUME(GetMoveEffect(MOVE_SANDSTORM) == EFFECT_SANDSTORM);
        PLAYER(species) { Ability(ability); Innates(innate); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_SANDSTORM); }
        TURN {}
    } SCENE {
        ABILITY_POPUP(player, innate);
        MESSAGE("The effects of the weather disappeared.");
        MESSAGE("The opposing Wobbuffet used Sandstorm!");
        MESSAGE("The sandstorm is raging.");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SANDSTORM_CONTINUES);
        NONE_OF {
            HP_BAR(player);
            HP_BAR(opponent);
            MESSAGE("The opposing Wobbuffet is buffeted by the sandstorm!");
        }
        MESSAGE("The sandstorm is raging.");
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_SANDSTORM_CONTINUES);
    }
}

TO_DO_BATTLE_TEST("INNATE: Cloud Nine/Air Lock prevent basic weather effects, but without them disappearing - Sun");
TO_DO_BATTLE_TEST("INNATE: Cloud Nine/Air Lock prevent basic weather effects, but without them disappearing - Rain");
TO_DO_BATTLE_TEST("INNATE: Cloud Nine/Air Lock prevent basic weather effects, but without them disappearing - Hail");
TO_DO_BATTLE_TEST("INNATE: Cloud Nine/Air Lock prevent basic weather effects, but without them disappearing - Snow");
TO_DO_BATTLE_TEST("INNATE: Cloud Nine/Air Lock prevent basic weather effects, but without them disappearing - Fog");
TO_DO_BATTLE_TEST("INNATE: Cloud Nine/Air Lock prevent basic weather effects, but without them disappearing - Primal Sun");
TO_DO_BATTLE_TEST("INNATE: Cloud Nine/Air Lock prevent basic weather effects, but without them disappearing - Primal Rain");
TO_DO_BATTLE_TEST("INNATE: Cloud Nine/Air Lock prevent basic weather effects, but without them disappearing - Strong Winds");

// Moves and abilities that are affected by weather should have new tests that check for Clould Nine/Air Lock, like Mold-Breaker Abilities
