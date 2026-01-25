#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"

// Spieler-Konstanten
#define PLAYER_WIDTH        24.0f
#define PLAYER_HEIGHT       32.0f
#define MOVE_SPEED          3.0f
#define JUMP_FORCE          -12.0f
#define GRAVITY             0.5f
#define MAX_FALL_SPEED      10.0f
#define HAND_DISTANCE       30.0f  // Abstand zwischen Tanja und Hans

// Charakter (Tanja oder Hans)
typedef struct {
    Vector2 offset;      // Relativer Versatz zum Mittelpunkt
    u32 color;           // Farbe (für Prototyp)
} Character;

// Spieler-Paar
typedef struct {
    Vector2 position;    // Gemeinsame Mittelpunkt-Position
    Vector2 velocity;
    bool grounded;
    int hearts;
    int suns;
    
    Character tanja;
    Character hans;
} PlayerPair;

// Funktionen
void initPlayer(void);
void updatePlayer(float dt);
PlayerPair* getPlayer(void);

#endif // PLAYER_H
