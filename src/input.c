#include "input.h"
#include <wiiuse/wpad.h>

static InputState input;

void updateInput(void) {
    // WPAD_ScanPads wird in main.c gemacht!
    u32 held = WPAD_ButtonsHeld(WPAD_CHAN_0);
    u32 pressed = WPAD_ButtonsDown(WPAD_CHAN_0);
    
    // Bewegung (funktioniert horizontal und vertikal)
    input.moveLeft = (held & WPAD_BUTTON_LEFT) != 0 || (held & WPAD_BUTTON_UP) != 0;
    input.moveRight = (held & WPAD_BUTTON_RIGHT) != 0 || (held & WPAD_BUTTON_DOWN) != 0;
    
    // Sprung (A oder 2)
    input.jump = (pressed & WPAD_BUTTON_A) != 0 || (pressed & WPAD_BUTTON_2) != 0;
    
    // Interaktion (B oder 1)
    input.interact = (pressed & WPAD_BUTTON_B) != 0 || (pressed & WPAD_BUTTON_1) != 0;
    
    // Pause
    input.pause = (pressed & WPAD_BUTTON_PLUS) != 0;
}

InputState* getInputState(void) {
    return &input;
}
