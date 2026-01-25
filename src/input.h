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
InputState* getInputState(void);

#endif // INPUT_H
