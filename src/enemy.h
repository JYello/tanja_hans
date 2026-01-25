#ifndef ENEMY_H
#define ENEMY_H

#include "game.h"

#define MAX_ENEMIES 10

typedef enum {
    ENEMY_PATROL,     // Läuft hin und her
    ENEMY_HOVER,      // Schwebt auf und ab
    ENEMY_CIRCLE      // Kreist um einen Punkt
} EnemyType;

typedef struct {
    Vector2 position;
    Vector2 velocity;
    Vector2 startPos;     // Für Patrol/Circle
    float moveRange;      // Bewegungsreichweite
    float timer;          // Für Animationen
    EnemyType type;
    bool active;
    u32 color;
} Enemy;

void initEnemies(void);
void updateEnemies(float dt);
Enemy* getEnemies(void);
int getEnemyCount(void);

#endif // ENEMY_H
