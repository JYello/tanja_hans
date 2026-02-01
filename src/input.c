#include "input.h"
#include <wiiuse/wpad.h>

static InputState input;       // Single Player oder Spieler 1
static InputState tanjaInput;  // Zwei-Spieler: Wiimote 1
static InputState hansInput;   // Zwei-Spieler: Wiimote 2

void updateInput(void) {
    // WPAD_ScanPads wird in main.c gemacht!
    u32 held1 = WPAD_ButtonsHeld(WPAD_CHAN_0);
    u32 pressed1 = WPAD_ButtonsDown(WPAD_CHAN_0);
    
    u32 held2 = WPAD_ButtonsHeld(WPAD_CHAN_1);
    u32 pressed2 = WPAD_ButtonsDown(WPAD_CHAN_1);
    
    // Single-Player Input (Wiimote 1, beide Figuren)
    input.moveLeft = (held1 & WPAD_BUTTON_LEFT) != 0 || (held1 & WPAD_BUTTON_UP) != 0;
    input.moveRight = (held1 & WPAD_BUTTON_RIGHT) != 0 || (held1 & WPAD_BUTTON_DOWN) != 0;
    input.jump = (pressed1 & WPAD_BUTTON_A) != 0 || (pressed1 & WPAD_BUTTON_2) != 0;
    input.interact = (pressed1 & WPAD_BUTTON_B) != 0 || (pressed1 & WPAD_BUTTON_1) != 0;
    input.pause = (pressed1 & WPAD_BUTTON_PLUS) != 0;
    
    // Zwei-Spieler Input
    // Tanja (Wiimote 1)
    tanjaInput.moveLeft = (held1 & WPAD_BUTTON_LEFT) != 0 || (held1 & WPAD_BUTTON_UP) != 0;
    tanjaInput.moveRight = (held1 & WPAD_BUTTON_RIGHT) != 0 || (held1 & WPAD_BUTTON_DOWN) != 0;
    tanjaInput.jump = (pressed1 & WPAD_BUTTON_A) != 0 || (pressed1 & WPAD_BUTTON_2) != 0;
    tanjaInput.interact = (pressed1 & WPAD_BUTTON_B) != 0 || (pressed1 & WPAD_BUTTON_1) != 0;
    tanjaInput.pause = (pressed1 & WPAD_BUTTON_PLUS) != 0;
    
    // Hans (Wiimote 2)
    hansInput.moveLeft = (held2 & WPAD_BUTTON_LEFT) != 0 || (held2 & WPAD_BUTTON_UP) != 0;
    hansInput.moveRight = (held2 & WPAD_BUTTON_RIGHT) != 0 || (held2 & WPAD_BUTTON_DOWN) != 0;
    hansInput.jump = (pressed2 & WPAD_BUTTON_A) != 0 || (pressed2 & WPAD_BUTTON_2) != 0;
    hansInput.interact = (pressed2 & WPAD_BUTTON_B) != 0 || (pressed2 & WPAD_BUTTON_1) != 0;
    hansInput.pause = (pressed2 & WPAD_BUTTON_PLUS) != 0;
}

InputState* getInputState(void) {
    return &input;
}

InputState* getTanjaInput(void) {
    return &tanjaInput;
}

InputState* getHansInput(void) {
    return &hansInput;
}
