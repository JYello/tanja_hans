#ifndef GAME_H
#define GAME_H

#include <grrlib.h>

// Spielzustände
typedef enum {
    STATE_MENU,
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAMEOVER
} GameState;

// Spielmodus
typedef enum {
    MODE_SINGLE_PLAYER,
    MODE_TWO_PLAYER
} GameMode;

// Vektor für Positionen und Geschwindigkeiten
typedef struct {
    float x;
    float y;
} Vector2;

// Rechteck für Kollision
typedef struct {
    float x;
    float y;
    float w;
    float h;
} Rect;

// Farben (COL_ prefix um libogc-Konflikt zu vermeiden)
#define COL_WHITE       0xFFFFFFFF
#define COL_BLACK       0x000000FF
#define COL_RED         0xFF0000FF
#define COL_YELLOW      0xFFFF00FF
#define COL_SKY         0x87CEEBFF
#define COL_GRASS       0x7CFC00FF
#define COL_BROWN       0x8B4513FF

// Bildschirm
#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480

#endif // GAME_H
