#ifndef LEVEL_H
#define LEVEL_H

#include "game.h"
#include <stdbool.h>

// Tile-Größe
#define TILE_SIZE 32

// Tile-Typen
typedef struct {
    float x, y;
    float w, h;
    bool solid;
    u32 color;
} Tile;

// Sammelobjekt
typedef struct {
    float x, y;
    bool collected;
    bool isHeart;  // true = Herz, false = Sonne
} Collectible;

// Level-Daten
typedef struct {
    Tile* tiles;
    int tileCount;
    
    Collectible* collectibles;
    int collectibleCount;
} Level;

// Funktionen
void initLevel(void);
void cleanupLevel(void);
Tile* getLevelTiles(void);
int getLevelTileCount(void);
Collectible* getCollectibles(void);
int getCollectibleCount(void);

#endif // LEVEL_H
