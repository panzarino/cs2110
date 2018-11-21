#include "gba.h"
#include "logic.h"
#include "graphics.h"
#include "images/trip.h"
#include "images/gg.h"

#include <stdio.h>
#include <stdlib.h>

// AppState enum definition
typedef enum {
    // TA-TODO: Add any additional states you need for your app.
    START,
    START_NODRAW,
    APP_INIT,
    APP,
    APP_EXIT,
    APP_EXIT_NODRAW,
} GBAState;

int main(void) {
    // TA-TODO: Manipulate REG_DISPCNT here to set Mode 3.
    REG_DISPCNT = MODE3 | BG2_ENABLE;

    GBAState state = START;

    // We store the "current" and "next" states.
    AppState currentAppState, nextAppState;

    // We store the current and previous values of the button input.
    u32 previousButtons = BUTTONS;
    u32 currentButtons = BUTTONS;

    while(1) {
        // Load the current state of the buttons
        currentButtons = BUTTONS;

        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
            state = START;
        }

        // TA-TODO: Manipulate the state machine below as needed.
        switch(state) {
        case START:
            // Wait for VBlank
            waitForVBlank();

            // TA-TODO: Draw the start state here.
            drawFullScreenImageDMA(trip);
            drawCenteredString(WIDTH / 2 - 25, HEIGHT / 2 - 25, 50, 50, "PRESS A TO START", BLACK);

            state = START_NODRAW;
            break;
        case START_NODRAW:
            // TA-TODO: Check for a button press here to start the app.
            // Start the app by switching the state to APP_INIT.
            if (KEY_DOWN(BUTTON_A, currentButtons)) {
                state = APP_INIT;
            }

            break;
        case APP_INIT:
            // Initialize the app. Switch to the APP state.
            initializeAppState(&currentAppState);

            // Draw the initial state of the app
            fullDrawAppState(&currentAppState);

            state = APP;
            break;
        case APP:
            // Process the app for one frame, store the next state
            nextAppState = processAppState(&currentAppState, previousButtons, currentButtons);

            // Wait for VBlank before we do any drawing.
            waitForVBlank();

            // Undraw the previous state
            undrawAppState(&currentAppState);

            // Draw the current state
            drawAppState(&nextAppState);

            // Now set the current state as the next state for the next iter.
            currentAppState = nextAppState;

            // Check if the app is exiting. If it is, then go to the exit state.
            if (nextAppState.gameOver) state = APP_EXIT;

            break;
        case APP_EXIT:
            // Wait for VBlank
            waitForVBlank();

            // TA-TODO: Draw the exit / gameover screen
            drawFullScreenImageDMA(gg);

            drawCenteredString(WIDTH / 2 - 25, 10, 50, 10, "PRESS B TO RESTART", WHITE);

            state = APP_EXIT_NODRAW;
            break;
        case APP_EXIT_NODRAW:
            // TA-TODO: Check for a button press here to go back to the start screen
            if (KEY_DOWN(BUTTON_B, currentButtons)) {
                state = START;
            }

            break;
        }

        // Store the current state of the buttons
        previousButtons = currentButtons;
    }

    return 0;
}
