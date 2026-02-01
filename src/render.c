#include "render.h"
#include "game.h"
#include "level.h"
#include "player.h"
#include "enemy.h"
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
    
    // Gegner zeichnen
    Enemy* enemies = getEnemies();
    int enemyCount = getEnemyCount();
    
    for(int i = 0; i < enemyCount; i++) {
        if(enemies[i].active) {
            // Gegner als Kreis mit Augen
            GRRLIB_Circle(
                enemies[i].position.x,
                enemies[i].position.y,
                10,
                enemies[i].color,
                true
            );
            
            // Augen (kleine schwarze Punkte)
            GRRLIB_Circle(
                enemies[i].position.x - 4,
                enemies[i].position.y - 2,
                2,
                COL_BLACK,
                true
            );
            GRRLIB_Circle(
                enemies[i].position.x + 4,
                enemies[i].position.y - 2,
                2,
                COL_BLACK,
                true
            );
        }
    }
}

void renderPlayer(void) {
    PlayerPair* player = getPlayer();
    
    float tanjaX, tanjaY, hansX, hansY;
    
    if(player->mode == MODE_SINGLE_PLAYER) {
        // Single-Player: Offset-basiert
        tanjaX = player->position.x + player->tanja.offset.x;
        tanjaY = player->position.y + player->tanja.offset.y;
        hansX = player->position.x + player->hans.offset.x;
        hansY = player->position.y + player->hans.offset.y;
    } else {
        // Zwei-Spieler: Eigene Positionen
        tanjaX = player->tanja.position.x;
        tanjaY = player->tanja.position.y;
        hansX = player->hans.position.x;
        hansY = player->hans.position.y;
    }
    
    // Händchen-Verbindung (Linie)
    // Blinkt bei Unverwundbarkeit
    if(player->invincibilityTimer <= 0.0f || ((int)(player->invincibilityTimer * 10) % 2 == 0)) {
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
    }
    
    // Tanja zeichnen mit Animation
    float tanjaWidth = PLAYER_WIDTH;
    float tanjaHeight = PLAYER_HEIGHT;
    
    // Walk-Animation: Größe pulsiert leicht
    if(player->tanja.animState == ANIM_WALK) {
        float pulse = (player->tanja.animFrame % 2 == 0) ? 1.0f : 0.9f;
        tanjaHeight *= pulse;
    }
    // Jump-Animation: gestreckt
    else if(player->tanja.animState == ANIM_JUMP) {
        tanjaHeight *= 1.1f;
        tanjaWidth *= 0.9f;
    }
    
    // Blinken bei Unverwundbarkeit
    if(player->invincibilityTimer > 0.0f && ((int)(player->invincibilityTimer * 10) % 2 == 1)) {
        // Nicht zeichnen (Blink-Effekt)
    } else {
        GRRLIB_Rectangle(
            tanjaX - tanjaWidth / 2,
            tanjaY - tanjaHeight / 2,
            tanjaWidth,
            tanjaHeight,
            player->tanja.color,
            true
        );
        
        // Umrandung
        GRRLIB_Rectangle(
            tanjaX - tanjaWidth / 2,
            tanjaY - tanjaHeight / 2,
            tanjaWidth,
            tanjaHeight,
            COL_WHITE,
            false
        );
        
        // Gesicht (Augen)
        float eyeOffset = player->tanja.facingRight ? 4.0f : -4.0f;
        GRRLIB_Circle(tanjaX + eyeOffset - 3, tanjaY - 5, 2, COL_BLACK, true);
        GRRLIB_Circle(tanjaX + eyeOffset + 3, tanjaY - 5, 2, COL_BLACK, true);
    }
    
    // Hans zeichnen mit Animation
    float hansWidth = PLAYER_WIDTH;
    float hansHeight = PLAYER_HEIGHT;
    
    // Walk-Animation
    if(player->hans.animState == ANIM_WALK) {
        float pulse = (player->hans.animFrame % 2 == 0) ? 1.0f : 0.9f;
        hansHeight *= pulse;
    }
    // Jump-Animation
    else if(player->hans.animState == ANIM_JUMP) {
        hansHeight *= 1.1f;
        hansWidth *= 0.9f;
    }
    
    // Blinken bei Unverwundbarkeit
    if(player->invincibilityTimer > 0.0f && ((int)(player->invincibilityTimer * 10) % 2 == 1)) {
        // Nicht zeichnen
    } else {
        GRRLIB_Rectangle(
            hansX - hansWidth / 2,
            hansY - hansHeight / 2,
            hansWidth,
            hansHeight,
            player->hans.color,
            true
        );
        
        // Umrandung
        GRRLIB_Rectangle(
            hansX - hansWidth / 2,
            hansY - hansHeight / 2,
            hansWidth,
            hansHeight,
            COL_WHITE,
            false
        );
        
        // Gesicht (Augen)
        float eyeOffset = player->hans.facingRight ? 4.0f : -4.0f;
        GRRLIB_Circle(hansX + eyeOffset - 3, hansY - 5, 2, COL_BLACK, true);
        GRRLIB_Circle(hansX + eyeOffset + 3, hansY - 5, 2, COL_BLACK, true);
    }
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
    
    // Sonnen-Zähler (oben rechts) - visuell
    for(int i = 0; i < player->suns; i++) {
        GRRLIB_Circle(
            SCREEN_WIDTH - 30 - (i * 20),
            25,
            8,
            COL_YELLOW,
            true
        );
    }
}
