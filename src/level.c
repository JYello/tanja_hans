#include "level.h"
#include <stdlib.h>
#include <string.h>

static Level currentLevel;
static int currentLevelNumber = 1;

void loadLevel(int levelNumber) {
    // Altes Level aufräumen
    if(currentLevel.tiles) {
        free(currentLevel.tiles);
        currentLevel.tiles = NULL;
    }
    if(currentLevel.collectibles) {
        free(currentLevel.collectibles);
        currentLevel.collectibles = NULL;
    }
    
    currentLevelNumber = levelNumber;
    
    if(levelNumber == 1) {
        // LEVEL 1: Frühlingswiese (Original)
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
        
        // Sonnen (3 zum Sammeln)
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
        
        currentLevel.totalSuns = 3;
    }
    else if(levelNumber == 2) {
        // LEVEL 2: Blumengarten (entspannt, ohne Lücken)
        currentLevel.tileCount = 11;
        currentLevel.tiles = malloc(sizeof(Tile) * currentLevel.tileCount);
        
        // Durchgehender Boden (KEINE LÜCKEN!)
        for(int i = 0; i < 5; i++) {
            currentLevel.tiles[i].x = i * TILE_SIZE * 4;
            currentLevel.tiles[i].y = SCREEN_HEIGHT - TILE_SIZE;
            currentLevel.tiles[i].w = TILE_SIZE * 4;
            currentLevel.tiles[i].h = TILE_SIZE;
            currentLevel.tiles[i].solid = true;
            currentLevel.tiles[i].color = 0x98FB98FF;  // Hellgrün
        }
        
        // Sanfte Hügel-Plattformen
        currentLevel.tiles[5].x = 100.0f;
        currentLevel.tiles[5].y = 360.0f;
        currentLevel.tiles[5].w = TILE_SIZE * 3;
        currentLevel.tiles[5].h = TILE_SIZE;
        currentLevel.tiles[5].solid = true;
        currentLevel.tiles[5].color = 0xFFB6C1FF;  // Rosa
        
        currentLevel.tiles[6].x = 280.0f;
        currentLevel.tiles[6].y = 320.0f;
        currentLevel.tiles[6].w = TILE_SIZE * 3;
        currentLevel.tiles[6].h = TILE_SIZE;
        currentLevel.tiles[6].solid = true;
        currentLevel.tiles[6].color = 0xFFB6C1FF;
        
        currentLevel.tiles[7].x = 460.0f;
        currentLevel.tiles[7].y = 280.0f;
        currentLevel.tiles[7].w = TILE_SIZE * 3;
        currentLevel.tiles[7].h = TILE_SIZE;
        currentLevel.tiles[7].solid = true;
        currentLevel.tiles[7].color = 0xFFB6C1FF;
        
        // Niedrige dekorative Plattformen
        currentLevel.tiles[8].x = 180.0f;
        currentLevel.tiles[8].y = 240.0f;
        currentLevel.tiles[8].w = TILE_SIZE * 2;
        currentLevel.tiles[8].h = TILE_SIZE;
        currentLevel.tiles[8].solid = true;
        currentLevel.tiles[8].color = 0xDDA0DDFF;  // Lila
        
        currentLevel.tiles[9].x = 380.0f;
        currentLevel.tiles[9].y = 200.0f;
        currentLevel.tiles[9].w = TILE_SIZE * 2;
        currentLevel.tiles[9].h = TILE_SIZE;
        currentLevel.tiles[9].solid = true;
        currentLevel.tiles[9].color = 0xDDA0DDFF;
        
        currentLevel.tiles[10].x = 50.0f;
        currentLevel.tiles[10].y = 280.0f;
        currentLevel.tiles[10].w = TILE_SIZE * 1.5f;
        currentLevel.tiles[10].h = TILE_SIZE;
        currentLevel.tiles[10].solid = true;
        currentLevel.tiles[10].color = 0xDDA0DDFF;
        
        // Sammelobjekte (mehr Herzen!)
        currentLevel.collectibleCount = 9;
        currentLevel.collectibles = malloc(sizeof(Collectible) * currentLevel.collectibleCount);
        
        // Viele Herzen (4 Stück - großzügig!)
        currentLevel.collectibles[0].x = 150.0f;
        currentLevel.collectibles[0].y = 320.0f;
        currentLevel.collectibles[0].isHeart = true;
        currentLevel.collectibles[0].collected = false;
        
        currentLevel.collectibles[1].x = 330.0f;
        currentLevel.collectibles[1].y = 280.0f;
        currentLevel.collectibles[1].isHeart = true;
        currentLevel.collectibles[1].collected = false;
        
        currentLevel.collectibles[2].x = 510.0f;
        currentLevel.collectibles[2].y = 240.0f;
        currentLevel.collectibles[2].isHeart = true;
        currentLevel.collectibles[2].collected = false;
        
        currentLevel.collectibles[3].x = 200.0f;
        currentLevel.collectibles[3].y = 200.0f;
        currentLevel.collectibles[3].isHeart = true;
        currentLevel.collectibles[3].collected = false;
        
        // Sonnen (5 zum Sammeln)
        currentLevel.collectibles[4].x = 120.0f;
        currentLevel.collectibles[4].y = 420.0f;
        currentLevel.collectibles[4].isHeart = false;
        currentLevel.collectibles[4].collected = false;
        
        currentLevel.collectibles[5].x = 300.0f;
        currentLevel.collectibles[5].y = 380.0f;
        currentLevel.collectibles[5].isHeart = false;
        currentLevel.collectibles[5].collected = false;
        
        currentLevel.collectibles[6].x = 480.0f;
        currentLevel.collectibles[6].y = 340.0f;
        currentLevel.collectibles[6].isHeart = false;
        currentLevel.collectibles[6].collected = false;
        
        currentLevel.collectibles[7].x = 430.0f;
        currentLevel.collectibles[7].y = 160.0f;
        currentLevel.collectibles[7].isHeart = false;
        currentLevel.collectibles[7].collected = false;
        
        currentLevel.collectibles[8].x = 60.0f;
        currentLevel.collectibles[8].y = 240.0f;
        currentLevel.collectibles[8].isHeart = false;
        currentLevel.collectibles[8].collected = false;
        
        currentLevel.totalSuns = 5;
    }
}

void initLevel(void) {
    loadLevel(1);
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

int getCurrentLevel(void) {
    return currentLevelNumber;
}

int getTotalSuns(void) {
    return currentLevel.totalSuns;
}
