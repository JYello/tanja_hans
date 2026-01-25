#include "render.h"
#include "game.h"
#include "level.h"
#include "player.h"
#include <grrlib.h>
#include <stdio.h>
#include <math.h>

void renderLevel(void) {
    Tile* tiles = getLevelTiles();
    int tileCount = getLevelTileCount();
    
    // Tiles zeichnen
    for(int i = 0; i < tileCount; i++) {
        GRRLIB_Rectangle(
            tiles[i].x,
            tiles[i].y,
            tiles[i].w,
            tiles[i].h,
            tiles[i].color,
            true
        );
        
        // Umrandung für bessere Sichtbarkeit
        GRRLIB_Rectangle(
            tiles[i].x,
            tiles[i].y,
            tiles[i].w,
            tiles[i].h,
            COL_BLACK,
            false
        );
    }
    
    // Sammelobjekte zeichnen
    Collectible* collectibles = getCollectibles();
    int collectibleCount = getCollectibleCount();
    
    for(int i = 0; i < collectibleCount; i++) {
        if(!collectibles[i].collected) {
            if(collectibles[i].isHeart) {
                // Herz (rotes Rechteck mit weißem Rand)
                GRRLIB_Rectangle(
                    collectibles[i].x - 8,
                    collectibles[i].y - 8,
                    16,
                    16,
                    COL_RED,
                    true
                );
                GRRLIB_Rectangle(
                    collectibles[i].x - 8,
                    collectibles[i].y - 8,
                    16,
                    16,
                    COL_WHITE,
                    false
                );
            } else {
                // Sonne (gelber Kreis)
                GRRLIB_Circle(
                    collectibles[i].x,
                    collectibles[i].y,
                    8,
                    COL_YELLOW,
                    true
                );
            }
        }
    }
}

void renderPlayer(void) {
    PlayerPair* player = getPlayer();
    
    // Tanja-Position berechnen
    float tanjaX = player->position.x + player->tanja.offset.x;
    float tanjaY = player->position.y + player->tanja.offset.y;
    
    // Hans-Position berechnen
    float hansX = player->position.x + player->hans.offset.x;
    float hansY = player->position.y + player->hans.offset.y;
    
    // Händchen-Verbindung (Linie)
    GRRLIB_Line(
        tanjaX,
        tanjaY,
        hansX,
        hansY,
        0xFFFFFFFF  // Weiß
    );
    
    // Herz in der Mitte der Verbindung
    float midX = (tanjaX + hansX) / 2.0f;
    float midY = (tanjaY + hansY) / 2.0f;
    
    GRRLIB_Rectangle(
        midX - 4,
        midY - 4,
        8,
        8,
        0xFF1493FF,  // Deep Pink
        true
    );
    
    // Tanja zeichnen (Rechteck)
    GRRLIB_Rectangle(
        tanjaX - PLAYER_WIDTH / 2,
        tanjaY - PLAYER_HEIGHT / 2,
        PLAYER_WIDTH,
        PLAYER_HEIGHT,
        player->tanja.color,
        true
    );
    
    // Umrandung
    GRRLIB_Rectangle(
        tanjaX - PLAYER_WIDTH / 2,
        tanjaY - PLAYER_HEIGHT / 2,
        PLAYER_WIDTH,
        PLAYER_HEIGHT,
        COL_WHITE,
        false
    );
    
    // Hans zeichnen (Rechteck)
    GRRLIB_Rectangle(
        hansX - PLAYER_WIDTH / 2,
        hansY - PLAYER_HEIGHT / 2,
        PLAYER_WIDTH,
        PLAYER_HEIGHT,
        player->hans.color,
        true
    );
    
    // Umrandung
    GRRLIB_Rectangle(
        hansX - PLAYER_WIDTH / 2,
        hansY - PLAYER_HEIGHT / 2,
        PLAYER_WIDTH,
        PLAYER_HEIGHT,
        COL_WHITE,
        false
    );
}

void renderHUD(void) {
    PlayerPair* player = getPlayer();
    
    // Herzen (oben links)
    for(int i = 0; i < player->hearts; i++) {
        GRRLIB_Rectangle(
            10 + (i * 25),
            10,
            20,
            20,
            COL_RED,
            true
        );
        GRRLIB_Rectangle(
            10 + (i * 25),
            10,
            20,
            20,
            COL_WHITE,
            false
        );
    }
    
    // DEBUG: Grounded-Status visuell (großes Rechteck oben rechts)
    u32 groundedColor = player->grounded ? 0x00FF00FF : 0xFF0000FF; // Grün = grounded, Rot = in der Luft
    GRRLIB_Rectangle(
        SCREEN_WIDTH - 50,
        10,
        40,
        40,
        groundedColor,
        true
    );
    GRRLIB_Rectangle(
        SCREEN_WIDTH - 50,
        10,
        40,
        40,
        COL_WHITE,
        false
    );
    
    // DEBUG: Jump-Input visuell (kleines Rechteck daneben)
    InputState* input = getInputState();
    u32 jumpColor = input->jump ? 0xFFFF00FF : 0x666666FF; // Gelb = Jump gedrückt
    GRRLIB_Rectangle(
        SCREEN_WIDTH - 100,
        10,
        40,
        40,
        jumpColor,
        true
    );
    GRRLIB_Rectangle(
        SCREEN_WIDTH - 100,
        10,
        40,
        40,
        COL_WHITE,
        false
    );
}
