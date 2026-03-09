#ifndef SPRITES_H
#define SPRITES_H

#include <grrlib.h>
#include "player.h"

// Sprite-Frames
typedef struct {
    GRRLIB_texImg* texture;  // Gesamtes Sprite-Sheet
    int frameWidth;          // Breite eines Frames
    int frameHeight;         // Höhe eines Frames
    float scale;             // Skalierungsfaktor
} SpriteSheet;

// Globale Sprite-Daten
extern SpriteSheet tanjaSprites;
extern SpriteSheet hansSprites;
extern GRRLIB_texImg* heartSprite;
extern GRRLIB_texImg* sunSprite;
extern GRRLIB_texImg* enemySprites;  // 3 Monster nebeneinander

// Funktionen
bool loadSprites(void);
void cleanupSprites(void);
void drawCharacterSprite(float x, float y, Character* character, float scale);
void drawHeart(float x, float y, float scale);
void drawSun(float x, float y, float scale);
void drawEnemy(float x, float y, int enemyType, float scale);  // enemyType: 0-2

#endif // SPRITES_H
