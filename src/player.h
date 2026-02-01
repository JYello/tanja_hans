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
#define MAX_DISTANCE        80.0f  // Maximaler Abstand im Zwei-Spieler-Modus

// Animations-Zustände
typedef enum {
    ANIM_IDLE,
    ANIM_WALK,
    ANIM_JUMP
} AnimState;

// Charakter (Tanja oder Hans)
typedef struct {
    Vector2 position;    // Im Zwei-Spieler-Modus: eigene Position
    Vector2 offset;      // Im Single-Player: Relativer Versatz zum Mittelpunkt
    Vector2 velocity;    // Im Zwei-Spieler-Modus: eigene Velocity
    u32 color;
    AnimState animState;
    float animTimer;
    int animFrame;
    bool facingRight;
    bool grounded;       // Im Zwei-Spieler-Modus: eigener Ground-Status
} Character;

// Spieler-Paar
typedef struct {
    Vector2 position;    // Gemeinsame Mittelpunkt-Position (Single-Player)
    Vector2 velocity;    // Gemeinsame Velocity (Single-Player)
    bool grounded;       // Gemeinsamer Ground-Status (Single-Player)
    int hearts;
    int suns;
    
    Character tanja;
    Character hans;
    
    float invincibilityTimer;
    GameMode mode;       // Single oder Zwei-Spieler
} PlayerPair;

// Funktionen
void initPlayer(GameMode mode);
void updatePlayer(float dt);
PlayerPair* getPlayer(void);

#endif // PLAYER_H
