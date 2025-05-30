#include "global.h"
#include "test/battle.h"

DOUBLE_BATTLE_TEST("ABILITY: Frisk does not trigger when pokemon hold no items")
{
    GIVEN {
        PLAYER(SPECIES_FURRET) { Ability(ABILITY_FRISK); };
        PLAYER(SPECIES_FURRET) { Ability(ABILITY_FRISK); };
        OPPONENT(SPECIES_SENTRET) { Ability(ABILITY_FRISK); };
        OPPONENT(SPECIES_SENTRET) { Ability(ABILITY_FRISK); };
    } WHEN {
        TURN { ; }
    } SCENE {
        NONE_OF {
            ABILITY_POPUP(playerLeft, ABILITY_FRISK);
            ABILITY_POPUP(playerRight, ABILITY_FRISK);
            ABILITY_POPUP(opponentLeft, ABILITY_FRISK);
            ABILITY_POPUP(opponentRight, ABILITY_FRISK);
        }
    }
}

SINGLE_BATTLE_TEST("ABILITY: Frisk triggers in a Single Battle")
{
    GIVEN {
        PLAYER(SPECIES_FURRET) { Ability(ABILITY_FRISK); Item(ITEM_POTION); };
        OPPONENT(SPECIES_SENTRET) { Ability(ABILITY_FRISK); Item(ITEM_POTION); };
    } WHEN {
        TURN { ; }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_FRISK);
        MESSAGE("Furret frisked the opposing Sentret and found its Potion!");
        ABILITY_POPUP(opponent, ABILITY_FRISK);
        MESSAGE("The opposing Sentret frisked Furret and found its Potion!");
    }
}

DOUBLE_BATTLE_TEST("ABILITY: Frisk triggers for player in a Double Battle after switching-in after fainting")
{
    struct BattlePokemon *target = NULL;
    PARAMETRIZE { target = playerLeft; }
    PARAMETRIZE { target = playerRight; }

    GIVEN {
        ASSUME(!IsBattleMoveStatus(MOVE_POUND));
        PLAYER(SPECIES_WOBBUFFET) { HP(1); }
        PLAYER(SPECIES_WOBBUFFET) { HP(1); }
        PLAYER(SPECIES_FURRET) { Ability(ABILITY_FRISK); };
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_POTION); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_POUND, target: target); SEND_OUT(target, 2); }
    } SCENE {
        MESSAGE("The opposing Wynaut used Pound!");
        MESSAGE("Wobbuffet fainted!");
        ABILITY_POPUP(target, ABILITY_FRISK);
        MESSAGE("Furret frisked the opposing Wynaut and found its Potion!");
    }
}

DOUBLE_BATTLE_TEST("ABILITY: Frisk triggers for opponent in a Double Battle after switching-in after fainting")
{
    struct BattlePokemon *target = NULL;
    PARAMETRIZE { target = opponentLeft; }
    PARAMETRIZE { target = opponentRight; }

    GIVEN {
        ASSUME(!IsBattleMoveStatus(MOVE_POUND));
        PLAYER(SPECIES_WYNAUT) { Item(ITEM_POTION); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); }
        OPPONENT(SPECIES_FURRET) { Ability(ABILITY_FRISK); };
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_POUND, target: target); SEND_OUT(target, 2); }
    } SCENE {
        MESSAGE("Wynaut used Pound!");
        MESSAGE("The opposing Wobbuffet fainted!");
        ABILITY_POPUP(target, ABILITY_FRISK);
        MESSAGE("The opposing Furret frisked Wynaut and found its Potion!");
    }
}

DOUBLE_BATTLE_TEST("INNATE: Frisk does not trigger when pokemon hold no items")
{
    GIVEN {
        PLAYER(SPECIES_FURRET) { Ability(ABILITY_RUN_AWAY); Innates(ABILITY_FRISK); };
        PLAYER(SPECIES_FURRET) { Ability(ABILITY_RUN_AWAY); Innates(ABILITY_FRISK); };
        OPPONENT(SPECIES_SENTRET) { Ability(ABILITY_RUN_AWAY); Innates(ABILITY_FRISK); };
        OPPONENT(SPECIES_SENTRET) { Ability(ABILITY_RUN_AWAY); Innates(ABILITY_FRISK); };
    } WHEN {
        TURN { ; }
    } SCENE {
        NONE_OF {
            ABILITY_POPUP(playerLeft, ABILITY_FRISK);
            ABILITY_POPUP(playerRight, ABILITY_FRISK);
            ABILITY_POPUP(opponentLeft, ABILITY_FRISK);
            ABILITY_POPUP(opponentRight, ABILITY_FRISK);
        }
    }
}

SINGLE_BATTLE_TEST("INNATE: Frisk triggers in a Single Battle")
{
    GIVEN {
        PLAYER(SPECIES_FURRET) { Ability(ABILITY_RUN_AWAY); Innates(ABILITY_FRISK); Item(ITEM_POTION); };
        OPPONENT(SPECIES_SENTRET) { Ability(ABILITY_RUN_AWAY); Innates(ABILITY_FRISK); Item(ITEM_POTION); };
    } WHEN {
        TURN { ; }
    } SCENE {
        ABILITY_POPUP(player, ABILITY_FRISK);
        MESSAGE("Furret frisked the opposing Sentret and found its Potion!");
        ABILITY_POPUP(opponent, ABILITY_FRISK);
        MESSAGE("The opposing Sentret frisked Furret and found its Potion!");
    }
}

DOUBLE_BATTLE_TEST("INNATE: Frisk triggers for player in a Double Battle after switching-in after fainting")
{
    struct BattlePokemon *target = NULL;
    PARAMETRIZE { target = playerLeft; }
    PARAMETRIZE { target = playerRight; }

    GIVEN {
        ASSUME(!IsBattleMoveStatus(MOVE_POUND));
        PLAYER(SPECIES_WOBBUFFET) { HP(1); }
        PLAYER(SPECIES_WOBBUFFET) { HP(1); }
        PLAYER(SPECIES_FURRET) { Ability(ABILITY_RUN_AWAY); Innates(ABILITY_FRISK); };
        OPPONENT(SPECIES_WYNAUT) { Item(ITEM_POTION); }
        OPPONENT(SPECIES_WYNAUT);
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_POUND, target: target); SEND_OUT(target, 2); }
    } SCENE {
        MESSAGE("The opposing Wynaut used Pound!");
        MESSAGE("Wobbuffet fainted!");
        ABILITY_POPUP(target, ABILITY_FRISK);
        MESSAGE("Furret frisked the opposing Wynaut and found its Potion!");
    }
}

DOUBLE_BATTLE_TEST("INNATE: Frisk triggers for opponent in a Double Battle after switching-in after fainting")
{
    struct BattlePokemon *target = NULL;
    PARAMETRIZE { target = opponentLeft; }
    PARAMETRIZE { target = opponentRight; }

    GIVEN {
        ASSUME(!IsBattleMoveStatus(MOVE_POUND));
        PLAYER(SPECIES_WYNAUT) { Item(ITEM_POTION); }
        PLAYER(SPECIES_WYNAUT);
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(1); }
        OPPONENT(SPECIES_FURRET) { Ability(ABILITY_RUN_AWAY); Innates(ABILITY_FRISK); };
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_POUND, target: target); SEND_OUT(target, 2); }
    } SCENE {
        MESSAGE("Wynaut used Pound!");
        MESSAGE("The opposing Wobbuffet fainted!");
        ABILITY_POPUP(target, ABILITY_FRISK);
        MESSAGE("The opposing Furret frisked Wynaut and found its Potion!");
    }
}
