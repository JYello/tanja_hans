#include "physics.h"
#include "player.h"
#include "level.h"

bool checkCollision(Rect a, Rect b) {
    return (a.x < b.x + b.w &&
            a.x + a.w > b.x &&
            a.y < b.y + b.h &&
            a.y + a.h > b.y);
}

void updatePhysics(void) {
    // Hier können später erweiterte Physik-Berechnungen rein
    // Für Phase 1: wird in player.c direkt behandelt
}
