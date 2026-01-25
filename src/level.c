#include "level.h"
#include <stdlib.h>
#include <string.h>

static Level currentLevel;

void initLevel(void) {
    // Einfaches Test-Level mit Plattformen
    currentLevel.tileCount = 10;
    currentLevel.tiles = malloc(sizeof(Tile) * currentLevel.tileCount);
    
    // Boden (durchgehend)
    for(int i = 0; i < 5; i++) {
        currentLevel.tiles[i].x = i * TILE_SIZE * 4;
        currentLevel.tiles[i].y = SCREEN_HEIGHT - TILE_SIZE;
        currentLevel.tiles[i].w = TILE_SIZE * 4;
        currentLevel.tiles[i].h = TILE_SIZE;
        currentLevel.tiles[i].solid = true;
        currentLevel.tiles[i].color = COL_GRASS;
    }
    
    // Plattform 1 (mittig, erhöht)
    currentLevel.tiles[5].x = 200.0f;
    currentLevel.tiles[5].y = 320.0f;
    currentLevel.tiles[5].w = TILE_SIZE * 3;
    currentLevel.tiles[5].h = TILE_SIZE;
    currentLevel.tiles[5].solid = true;
    currentLevel.tiles[5].color = COL_BROWN;
    
    // Plattform 2 (rechts, höher)
    currentLevel.tiles[6].x = 400.0f;
    currentLevel.tiles[6].y = 240.0f;
    currentLevel.tiles[6].w = TILE_SIZE * 2;
    currentLevel.tiles[6].h = TILE_SIZE;
    currentLevel.tiles[6].solid = true;
    currentLevel.tiles[6].color = COL_BROWN;
    
    // Plattform 3 (links, niedrig)
    currentLevel.tiles[7].x = 50.0f;
    currentLevel.tiles[7].y = 370.0f;
    currentLevel.tiles[7].w = TILE_SIZE * 2;
    currentLevel.tiles[7].h = TILE_SIZE;
    currentLevel.tiles[7].solid = true;
    currentLevel.tiles[7].color = COL_BROWN;
    
    // Zusätzliche Plattformen für interessanteres Level
    currentLevel.tiles[8].x = 300.0f;
    currentLevel.tiles[8].y = 180.0f;
    currentLevel.tiles[8].w = TILE_SIZE * 2;
    currentLevel.tiles[8].h = TILE_SIZE;
    currentLevel.tiles[8].solid = true;
    currentLevel.tiles[8].color = COL_BROWN;
    
    currentLevel.tiles[9].x = 520.0f;
    currentLevel.tiles[9].y = 340.0f;
    currentLevel.tiles[9].w = TILE_SIZE * 2;
    currentLevel.tiles[9].h = TILE_SIZE;
    currentLevel.tiles[9].solid = true;
    currentLevel.tiles[9].color = COL_BROWN;
    
    // Sammelobjekte
    currentLevel.collectibleCount = 5;
    currentLevel.collectibles = malloc(sizeof(Collectible) * currentLevel.collectibleCount);
    
    // Herzen
    currentLevel.collectibles[0].x = 220.0f;
    currentLevel.collectibles[0].y = 280.0f;
    currentLevel.collectibles[0].isHeart = true;
    currentLevel.collectibles[0].collected = false;
    
    currentLevel.collectibles[1].x = 420.0f;
    currentLevel.collectibles[1].y = 200.0f;
    currentLevel.collectibles[1].isHeart = true;
    currentLevel.collectibles[1].collected = false;
    
    // Sonnen
    currentLevel.collectibles[2].x = 100.0f;
    currentLevel.collectibles[2].y = 330.0f;
    currentLevel.collectibles[2].isHeart = false;
    currentLevel.collectibles[2].collected = false;
    
    currentLevel.collectibles[3].x = 320.0f;
    currentLevel.collectibles[3].y = 140.0f;
    currentLevel.collectibles[3].isHeart = false;
    currentLevel.collectibles[3].collected = false;
    
    currentLevel.collectibles[4].x = 540.0f;
    currentLevel.collectibles[4].y = 300.0f;
    currentLevel.collectibles[4].isHeart = false;
    currentLevel.collectibles[4].collected = false;
}

void cleanupLevel(void) {
    if(currentLevel.tiles) {
        free(currentLevel.tiles);
        currentLevel.tiles = NULL;
    }
    if(currentLevel.collectibles) {
        free(currentLevel.collectibles);
        currentLevel.collectibles = NULL;
    }
}

Tile* getLevelTiles(void) {
    return currentLevel.tiles;
}

int getLevelTileCount(void) {
    return currentLevel.tileCount;
}

Collectible* getCollectibles(void) {
    return currentLevel.collectibles;
}

int getCollectibleCount(void) {
    return currentLevel.collectibleCount;
}
