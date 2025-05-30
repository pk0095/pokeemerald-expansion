#include "global.h"
#include "test/battle.h"

ASSUMPTIONS {
    ASSUME(GetMoveEffect(MOVE_RAIN_DANCE) == EFFECT_RAIN_DANCE);
}

SINGLE_BATTLE_TEST("ABILITY: Rain Dish recovers 1/16th of Max HP in Rain")
{
    GIVEN {
        PLAYER(SPECIES_LUDICOLO) { Ability(ABILITY_RAIN_DISH); HP(1); MaxHP(100); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_RAIN_DANCE); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_RAIN_DISH);
        MESSAGE("Ludicolo's Rain Dish restored its HP a little!");
        HP_BAR(player, damage:  -(100 / 16));
    }
}

SINGLE_BATTLE_TEST("ABILITY: Rain Dish doesn't recover HP if Cloud Nine/Air Lock is on the field")
{
    GIVEN {
        PLAYER(SPECIES_LUDICOLO) { Ability(ABILITY_RAIN_DISH); HP(1); MaxHP(100); }
        OPPONENT(SPECIES_GOLDUCK) { Ability(ABILITY_CLOUD_NINE); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_RAIN_DANCE); }
    } SCENE {
        NOT ABILITY_POPUP(player, ABILITY_RAIN_DISH);
    }
}

SINGLE_BATTLE_TEST("INNATE: Rain Dish recovers 1/16th of Max HP in Rain")
{
    GIVEN {
        PLAYER(SPECIES_LUDICOLO) { Ability(ABILITY_SWIFT_SWIM); Innates(ABILITY_RAIN_DISH); HP(1); MaxHP(100); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(opponent, MOVE_RAIN_DANCE); }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_RAIN_DISH);
        MESSAGE("Ludicolo's Rain Dish restored its HP a little!");
        HP_BAR(player, damage:  -(100 / 16));
    }
}

SINGLE_BATTLE_TEST("INNATE: Rain Dish doesn't recover HP if Cloud Nine/Air Lock is on the field")
{
    GIVEN {
        PLAYER(SPECIES_LUDICOLO) { Ability(ABILITY_SWIFT_SWIM); Innates(ABILITY_RAIN_DISH); HP(1); MaxHP(100); }
        OPPONENT(SPECIES_GOLDUCK) { Ability(ABILITY_DAMP); Innates(ABILITY_CLOUD_NINE); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_RAIN_DANCE); }
    } SCENE {
        NOT ABILITY_POPUP(player, ABILITY_RAIN_DISH);
    }
}
