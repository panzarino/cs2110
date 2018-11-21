#include "logic.h"
#include "images/smile.h"
#include "images/xxxtentacion.h"

void initializeAppState(AppState* appState) {
    // TA-TODO: Initialize everything that's part of this AppState struct here.
    // Suppose the struct contains random values, make sure everything gets
    // the value it should have when the app begins.
    appState->gameOver = 0;
    appState->score = 0;
    appState->time = 30;
    Character player;
    player.x = WIDTH / 2;
    player.y = HEIGHT / 2;
    player.size = SMILE_HEIGHT;
    player.image = smile;
    appState->player = player;
    Character enemy;
    enemy.size = XXXTENTACION_HEIGHT;
    enemy.x = randint(0, WIDTH - enemy.size);
    enemy.y = randint(0, HEIGHT - enemy.size);
    enemy.image = xxxtentacion;
    appState->enemy = enemy;
}

// TA-TODO: Add any process functions for sub-elements of your app here.
// For example, for a snake game, you could have a processSnake function
// or a createRandomFood function or a processFoods function.
//
// e.g.:
// static Snake processSnake(Snake* currentSnake);
// static void generateRandomFoods(AppState* currentAppState, AppState* nextAppState);

int intersect(Character a, Character b) {
    return (a.x + a.size > b.x && a.x < b.x + b.size) && (a.y + a.size > b.y && a.y < b.y + b.size);
}

// This function processes your current app state and returns the new (i.e. next)
// state of your application.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    /* TA-TODO: Do all of your app processing here. This function gets called
     * every frame.
     *
     * To check for key presses, use the KEY_JUST_PRESSED macro for cases where
     * you want to detect each key press once, or the KEY_DOWN macro for checking
     * if a button is still down.
     *
     * To count time, suppose that the GameBoy runs at a fixed FPS (60fps) and
     * that VBlank is processed once per frame. Use the vBlankCounter variable
     * and the modulus % operator to do things once every (n) frames. Note that
     * you want to process button every frame regardless (otherwise you will
     * miss inputs.)
     *
     * Do not do any drawing here.
     *
     * TA-TODO: VERY IMPORTANT! READ THIS PART.
     * You need to perform all calculations on the currentAppState passed to you,
     * and perform all state updates on the nextAppState state which we define below
     * and return at the end of the function. YOU SHOULD NOT MODIFY THE CURRENTSTATE.
     * Modifying the currentAppState will mean the undraw function will not be able
     * to undraw it later.
     */

    UNUSED(keysPressedBefore);

    AppState nextAppState = *currentAppState;

    if (vBlankCounter % 60 == 0) {
        nextAppState.time--;
    }
    if (currentAppState->time == 0) {
        nextAppState.gameOver = 1;
    }

    if (KEY_DOWN(BUTTON_UP, keysPressedNow) && currentAppState->player.y > 0) {
        nextAppState.player.y--;
    }
    if (KEY_DOWN(BUTTON_LEFT, keysPressedNow) && currentAppState->player.x > 0) {
        nextAppState.player.x--;
    }
    if (KEY_DOWN(BUTTON_DOWN, keysPressedNow) && currentAppState->player.y < HEIGHT - currentAppState->player.size) {
        nextAppState.player.y++;
    }
    if (KEY_DOWN(BUTTON_RIGHT, keysPressedNow) && currentAppState->player.x < WIDTH - currentAppState->player.size) {
        nextAppState.player.x++;
    }

    if (intersect(nextAppState.player, nextAppState.enemy)) {
        nextAppState.score++;
        Character enemy;
        enemy.size = XXXTENTACION_HEIGHT;
        enemy.x = randint(0, WIDTH - enemy.size);
        enemy.y = randint(0, HEIGHT - enemy.size);
        enemy.image = xxxtentacion;
        nextAppState.enemy = enemy;
    }

    return nextAppState;
}
