#include "player.h"
#include "input.h"
#include "level.h"
#include "audio.h"
#include <math.h>

static PlayerPair player;

void initPlayer(void) {
    // Startposition
    player.position.x = 100.0f;
    player.position.y = 300.0f;
    
    player.velocity.x = 0.0f;
    player.velocity.y = 0.0f;
    
    player.grounded = false;
    player.hearts = 5;
    player.suns = 0;
    player.invincibilityTimer = 0.0f;
    
    // Tanja (links, pink)
    player.tanja.offset.x = -HAND_DISTANCE;
    player.tanja.offset.y = 0.0f;
    player.tanja.color = 0xFF69B4FF; // Hot Pink
    player.tanja.animState = ANIM_IDLE;
    player.tanja.animTimer = 0.0f;
    player.tanja.animFrame = 0;
    player.tanja.facingRight = true;
    
    // Hans (rechts, blau)
    player.hans.offset.x = HAND_DISTANCE;
    player.hans.offset.y = 0.0f;
    player.hans.color = 0x4169E1FF; // Royal Blue
    player.hans.animState = ANIM_IDLE;
    player.hans.animTimer = 0.0f;
    player.hans.animFrame = 0;
    player.hans.facingRight = true;
}

void updatePlayer(float dt) {
    InputState* input = getInputState();
    
    // Unverwundbarkeits-Timer
    if(player.invincibilityTimer > 0.0f) {
        player.invincibilityTimer -= dt;
    }
    
    // Horizontale Bewegung
    if(input->moveLeft) {
        player.velocity.x = -MOVE_SPEED;
        player.tanja.facingRight = false;
        player.hans.facingRight = false;
    } else if(input->moveRight) {
        player.velocity.x = MOVE_SPEED;
        player.tanja.facingRight = true;
        player.hans.facingRight = true;
    } else {
        player.velocity.x = 0.0f;
    }
    
    // Animations-Zustände aktualisieren
    if(!player.grounded) {
        player.tanja.animState = ANIM_JUMP;
        player.hans.animState = ANIM_JUMP;
    } else if(player.velocity.x != 0.0f) {
        player.tanja.animState = ANIM_WALK;
        player.hans.animState = ANIM_WALK;
    } else {
        player.tanja.animState = ANIM_IDLE;
        player.hans.animState = ANIM_IDLE;
    }
    
    // Animation-Timer (für Walk-Animation)
    if(player.tanja.animState == ANIM_WALK) {
        player.tanja.animTimer += dt;
        player.hans.animTimer += dt;
        
        // 4 Frames für Walk-Cycle, 0.15s pro Frame
        if(player.tanja.animTimer >= 0.15f) {
            player.tanja.animFrame = (player.tanja.animFrame + 1) % 4;
            player.hans.animFrame = (player.hans.animFrame + 1) % 4;
            player.tanja.animTimer = 0.0f;
            player.hans.animTimer = 0.0f;
        }
    } else {
        player.tanja.animFrame = 0;
        player.hans.animFrame = 0;
        player.tanja.animTimer = 0.0f;
        player.hans.animTimer = 0.0f;
    }
    
    // Springen (nur wenn auf dem Boden)
    if(input->jump && player.grounded) {
        player.velocity.y = JUMP_FORCE;
        player.grounded = false;
        playSound(SFX_JUMP);
    }
    
    // Gravitation (immer anwenden)
    player.velocity.y += GRAVITY;
    
    // Maximale Fallgeschwindigkeit
    if(player.velocity.y > MAX_FALL_SPEED) {
        player.velocity.y = MAX_FALL_SPEED;
    }
    
    // Horizontale Position aktualisieren
    player.position.x += player.velocity.x;
    
    // Bildschirmgrenzen (temporär)
    if(player.position.x < PLAYER_WIDTH / 2) {
        player.position.x = PLAYER_WIDTH / 2;
    }
    if(player.position.x > SCREEN_WIDTH - PLAYER_WIDTH / 2) {
        player.position.x = SCREEN_WIDTH - PLAYER_WIDTH / 2;
    }
    
    // Vertikale Position aktualisieren
    player.position.y += player.velocity.y;
    
    // Boden-Kollision
    Tile* tiles = getLevelTiles();
    int tileCount = getLevelTileCount();
    
    player.grounded = false;
    
    // Spieler-Bounding-Box (Mittelpunkt, groß genug für beide)
    Rect playerRect = {
        player.position.x - HAND_DISTANCE - PLAYER_WIDTH / 2,
        player.position.y - PLAYER_HEIGHT / 2,
        (HAND_DISTANCE * 2) + PLAYER_WIDTH,
        PLAYER_HEIGHT
    };
    
    for(int i = 0; i < tileCount; i++) {
        if(tiles[i].solid) {
            Rect tileRect = {
                tiles[i].x,
                tiles[i].y,
                tiles[i].w,
                tiles[i].h
            };
            
            // AABB-Kollision
            if(playerRect.x < tileRect.x + tileRect.w &&
               playerRect.x + playerRect.w > tileRect.x &&
               playerRect.y < tileRect.y + tileRect.h &&
               playerRect.y + playerRect.h > tileRect.y) {
                
                // Berechne Überlappung in jede Richtung
                float overlapLeft = (playerRect.x + playerRect.w) - tileRect.x;
                float overlapRight = (tileRect.x + tileRect.w) - playerRect.x;
                float overlapTop = (playerRect.y + playerRect.h) - tileRect.y;
                float overlapBottom = (tileRect.y + tileRect.h) - playerRect.y;
                
                // Finde kleinste Überlappung
                float minOverlap = overlapTop;
                if(overlapBottom < minOverlap) minOverlap = overlapBottom;
                if(overlapLeft < minOverlap) minOverlap = overlapLeft;
                if(overlapRight < minOverlap) minOverlap = overlapRight;
                
                // Nur vertikale Kollision behandeln (für Phase 1)
                if(minOverlap == overlapTop && player.velocity.y > 0) {
                    // Von oben auf Plattform
                    player.position.y = tileRect.y - PLAYER_HEIGHT / 2 - 0.1f;
                    player.velocity.y = 0;
                    player.grounded = true;
                } else if(minOverlap == overlapBottom && player.velocity.y < 0) {
                    // Kopf an Decke
                    player.position.y = tileRect.y + tileRect.h + PLAYER_HEIGHT / 2 + 0.1f;
                    player.velocity.y = 0;
                }
            }
        }
    }
    
    // Sammelobjekte einsammeln
    Collectible* collectibles = getCollectibles();
    int collectibleCount = getCollectibleCount();
    
    for(int i = 0; i < collectibleCount; i++) {
        if(!collectibles[i].collected) {
            // Kollision mit Sammelobjekt (15px Radius)
            float dx = player.position.x - collectibles[i].x;
            float dy = player.position.y - collectibles[i].y;
            float distance = sqrtf(dx * dx + dy * dy);
            
            if(distance < 30.0f) {  // Großzügiger Sammelradius
                collectibles[i].collected = true;
                
                if(collectibles[i].isHeart) {
                    player.hearts++;
                    playSound(SFX_COLLECT_HEART);
                } else {
                    player.suns++;
                    playSound(SFX_COLLECT_SUN);
                }
            }
        }
    }
}

PlayerPair* getPlayer(void) {
    return &player;
}
