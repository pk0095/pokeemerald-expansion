#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(GetMoveEffect(MOVE_HAIL) == EFFECT_HAIL);
    ASSUME(GetMoveEffect(MOVE_SNOWSCAPE) == EFFECT_SNOWSCAPE);
}

SINGLE_BATTLE_TEST("ABILITY: Ice Body prevents damage from hail")
{
    u32 move;
    PARAMETRIZE { move = MOVE_HAIL; }
    PARAMETRIZE { move = MOVE_SNOWSCAPE; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_GLALIE) { Ability(ABILITY_ICE_BODY); }
    } WHEN {
        TURN { MOVE(player, move); MOVE(opponent, MOVE_SKILL_SWAP); }
    } SCENE {
        NONE_OF { HP_BAR(player); }
    }
}

SINGLE_BATTLE_TEST("ABILITY: Ice Body recovers 1/16th of Max HP in hail.")
{
    u32 move;
    PARAMETRIZE { move = MOVE_HAIL; }
    PARAMETRIZE { move = MOVE_SNOWSCAPE; }
    GIVEN {
        PLAYER(SPECIES_GLALIE) { Ability(ABILITY_ICE_BODY); HP(1); MaxHP(100); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, move); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_ICE_BODY);
        HP_BAR(player, damage: -(100 / 16));
        MESSAGE("Glalie's Ice Body healed it a little bit!");
    }
}

SINGLE_BATTLE_TEST("ABILITY: Ice Body doesn't recover HP if Cloud Nine/Air Lock is on the field")
{
    u32 move;
    PARAMETRIZE { move = MOVE_HAIL; }
    PARAMETRIZE { move = MOVE_SNOWSCAPE; }
    GIVEN {
        PLAYER(SPECIES_GLALIE) { Ability(ABILITY_ICE_BODY); HP(1); MaxHP(100); }
        OPPONENT(SPECIES_GOLDUCK) { Ability(ABILITY_CLOUD_NINE); }
    } WHEN {
        TURN { MOVE(opponent, move); }
    } SCENE {
        NOT ABILITY_POPUP(player, ABILITY_ICE_BODY);
    }
}

SINGLE_BATTLE_TEST("INNATE: Ice Body prevents damage from hail")
{
    u32 move;
    PARAMETRIZE { move = MOVE_HAIL; }
    PARAMETRIZE { move = MOVE_SNOWSCAPE; }
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET) { Innates(ABILITY_ICE_BODY); }
        OPPONENT(SPECIES_GLALIE) { Ability(ABILITY_INNER_FOCUS); Innates(ABILITY_ICE_BODY); }
    } WHEN {
        TURN { MOVE(player, move); MOVE(opponent, MOVE_SKILL_SWAP); }
    } SCENE {
        NONE_OF { HP_BAR(player); }
    }
}

SINGLE_BATTLE_TEST("INNATE: Ice Body recovers 1/16th of Max HP in hail.")
{
    u32 move;
    PARAMETRIZE { move = MOVE_HAIL; }
    PARAMETRIZE { move = MOVE_SNOWSCAPE; }
    GIVEN {
        PLAYER(SPECIES_GLALIE) { Ability(ABILITY_INNER_FOCUS); Innates(ABILITY_ICE_BODY); HP(1); MaxHP(100); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, move); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_ICE_BODY);
        HP_BAR(player, damage: -(100 / 16));
        MESSAGE("Glalie's Ice Body healed it a little bit!");
    }
}

SINGLE_BATTLE_TEST("INNATE: Ice Body doesn't recover HP if Cloud Nine/Air Lock is on the field")
{
    u32 move;
    PARAMETRIZE { move = MOVE_HAIL; }
    PARAMETRIZE { move = MOVE_SNOWSCAPE; }
    GIVEN {
        PLAYER(SPECIES_GLALIE) { Ability(ABILITY_INNER_FOCUS); Innates(ABILITY_ICE_BODY); HP(1); MaxHP(100); }
        OPPONENT(SPECIES_GOLDUCK) { Ability(ABILITY_DAMP); Innates(ABILITY_CLOUD_NINE); }
    } WHEN {
        TURN { MOVE(opponent, move); }
    } SCENE {
        NOT ABILITY_POPUP(player, ABILITY_ICE_BODY);
    }
}
