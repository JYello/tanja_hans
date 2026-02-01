#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

typedef struct {
    bool moveLeft;
    bool moveRight;
    bool jump;
    bool interact;
    bool pause;
} InputState;

void updateInput(void);
InputState* getInputState(void);      // Spieler 1 / Single Player
InputState* getTanjaInput(void);      // Zwei-Spieler: Tanja
InputState* getHansInput(void);       // Zwei-Spieler: Hans

#endif // INPUT_H
