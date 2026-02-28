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

// Funktionen
bool loadSprites(void);
void cleanupSprites(void);
void drawCharacterSprite(float x, float y, Character* character, float scale);

#endif // SPRITES_H
