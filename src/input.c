#include "input.h"
#include <wiiuse/wpad.h>

static InputState input;

void updateInput(void) {
    // WPAD_ScanPads wird jetzt in main.c gemacht!
    u32 held = WPAD_ButtonsHeld(WPAD_CHAN_0);
    u32 pressed = WPAD_ButtonsDown(WPAD_CHAN_0);
    
    // Alle möglichen Bewegungs-Kombinationen testen
    // Vertikal gehalten:
    input.moveLeft = (held & WPAD_BUTTON_LEFT) != 0;
    input.moveRight = (held & WPAD_BUTTON_RIGHT) != 0;
    
    // Horizontal gehalten:
    if(!input.moveLeft) input.moveLeft = (held & WPAD_BUTTON_UP) != 0;
    if(!input.moveRight) input.moveRight = (held & WPAD_BUTTON_DOWN) != 0;
    
    // Sprung - ALLE möglichen Tasten testen!
    input.jump = (pressed & WPAD_BUTTON_A) != 0 ||
                 (pressed & WPAD_BUTTON_B) != 0 ||
                 (pressed & WPAD_BUTTON_1) != 0 ||
                 (pressed & WPAD_BUTTON_2) != 0 ||
                 (pressed & WPAD_BUTTON_MINUS) != 0 ||
                 (pressed & WPAD_BUTTON_PLUS) != 0;
    
    // Interaktion (später)
    input.interact = false;
    
    // Pause
    input.pause = false;
}

InputState* getInputState(void) {
    return &input;
}
