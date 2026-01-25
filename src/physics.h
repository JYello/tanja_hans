#ifndef PHYSICS_H
#define PHYSICS_H

#include "game.h"

// Kollisionsprüfung
bool checkCollision(Rect a, Rect b);

// Physik-Update
void updatePhysics(void);

#endif // PHYSICS_H
