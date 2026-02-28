#include "sprites.h"
#include <stdio.h>
#include <stdlib.h>

// Eingebettete Sprite-Daten (werden vom Makefile eingefügt)
extern const u8 sprites_png[];
extern const u32 sprites_png_size;

SpriteSheet tanjaSprites;
SpriteSheet hansSprites;

bool loadSprites(void) {
    // Lade PNG aus eingebetteten Daten
    GRRLIB_texImg* spriteSheet = GRRLIB_LoadTexture(sprites_png);
    
    if(!spriteSheet) {
        return false;
    }
    
    // Annahme: th_pixel3.png hat Tanja links, Hans rechts
    // Wir teilen das Bild in der Mitte
    int fullWidth = spriteSheet->w;
    int fullHeight = spriteSheet->h;
    
    // Für Animationen: Wir nehmen an dass jede Figur mehrere Frames hat
    // Format: [Idle] [Walk1] [Walk2] [Walk3] [Jump]
    // Jeder Frame ist 64x64 Pixel
    
    tanjaSprites.texture = spriteSheet;
    tanjaSprites.frameWidth = 64;   // 64x64 pro Frame
    tanjaSprites.frameHeight = 64;
    tanjaSprites.scale = 1.0f;      // Keine zusätzliche Skalierung (64px ist schon gut)
    
    hansSprites.texture = spriteSheet;
    hansSprites.frameWidth = 64;
    hansSprites.frameHeight = 64;
    hansSprites.scale = 1.0f;
    
    GRRLIB_SetHandle(spriteSheet, 0, 0);
    
    return true;
}

void cleanupSprites(void) {
    if(tanjaSprites.texture) {
        GRRLIB_FreeTexture(tanjaSprites.texture);
        tanjaSprites.texture = NULL;
        hansSprites.texture = NULL;  // Beide nutzen dieselbe Texture
    }
}

void drawCharacterSprite(float x, float y, Character* character, float scale) {
    if(!tanjaSprites.texture) return;
    
    // Bestimme Frame basierend auf Animation
    int frameX = 0;
    int frameY = 0;
    
    // Neues Frame-Layout im Sprite-Sheet (10 Frames breit, GESPIEGELT):
    // Zeile 0: Tanja LINKS: [Idle] [Walk1] [Walk2] [Walk3] [Jump] | Tanja RECHTS: [Jump] [Walk3] [Walk2] [Walk1] [Idle]
    // Zeile 1: Hans LINKS:  [Idle] [Walk1] [Walk2] [Walk3] [Jump] | Hans RECHTS:  [Jump] [Walk3] [Walk2] [Walk1] [Idle]
    
    bool isTanja = (character->color == 0xFF69B4FF);  // Pink = Tanja
    
    frameY = isTanja ? 0 : 1;  // Tanja oben, Hans unten
    
    int baseFrame = 0;
    switch(character->animState) {
        case ANIM_IDLE:
            baseFrame = 0;
            break;
            
        case ANIM_WALK:
            // Walk-Cycle: Frames 1-3
            baseFrame = 1 + (character->animFrame % 3);
            break;
            
        case ANIM_JUMP:
            baseFrame = 4;
            break;
    }
    
    // Wenn nach RECHTS schauend: gespiegelte Frames 5-9
    if(character->facingRight) {
        // Spiegelung: Idle→9, Walk1→8, Walk2→7, Walk3→6, Jump→5
        frameX = 9 - baseFrame;
    } else {
        // Links: normal 0-4
        frameX = baseFrame;
    }
    
    // Berechne Position im Sprite-Sheet
    float srcX = frameX * tanjaSprites.frameWidth;
    float srcY = frameY * tanjaSprites.frameHeight;
    
    // Finale Größe
    float drawScale = scale * tanjaSprites.scale;
    
    // Zeichne den Frame (OHNE Flipping!)
    GRRLIB_DrawPart(
        x - (tanjaSprites.frameWidth * drawScale) / 2,  // Zentriert
        y - (tanjaSprites.frameHeight * drawScale) / 2,
        srcX,
        srcY,
        tanjaSprites.frameWidth,
        tanjaSprites.frameHeight,
        tanjaSprites.texture,
        0,  // Rotation
        drawScale,  // Normale Skalierung
        drawScale,  // Keine negative Skalierung!
        0xFFFFFFFF  // Weiß
    );
}
