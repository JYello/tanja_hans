#include "player.h"
#include "input.h"
#include "level.h"
#include "audio.h"
#include <math.h>

static PlayerPair player;

void initPlayer(GameMode mode) {
    player.mode = mode;
    player.hearts = 5;
    player.suns = 0;
    player.invincibilityTimer = 0.0f;
    
    if(mode == MODE_SINGLE_PLAYER) {
        // Startposition (Mittelpunkt)
        player.position.x = 100.0f;
        player.position.y = 300.0f;
        player.velocity.x = 0.0f;
        player.velocity.y = 0.0f;
        player.grounded = false;
        
        // Tanja (links, pink) - Offset-Modus
        player.tanja.offset.x = -HAND_DISTANCE;
        player.tanja.offset.y = 0.0f;
        player.tanja.color = 0xFF69B4FF;
        player.tanja.animState = ANIM_IDLE;
        player.tanja.animTimer = 0.0f;
        player.tanja.animFrame = 0;
        player.tanja.facingRight = true;
        
        // Hans (rechts, blau) - Offset-Modus
        player.hans.offset.x = HAND_DISTANCE;
        player.hans.offset.y = 0.0f;
        player.hans.color = 0x4169E1FF;
        player.hans.animState = ANIM_IDLE;
        player.hans.animTimer = 0.0f;
        player.hans.animFrame = 0;
        player.hans.facingRight = true;
    } else {
        // Zwei-Spieler: Beide haben eigene Positionen
        player.tanja.position.x = 80.0f;
        player.tanja.position.y = 300.0f;
        player.tanja.velocity.x = 0.0f;
        player.tanja.velocity.y = 0.0f;
        player.tanja.grounded = false;
        player.tanja.color = 0xFF69B4FF;
        player.tanja.animState = ANIM_IDLE;
        player.tanja.animTimer = 0.0f;
        player.tanja.animFrame = 0;
        player.tanja.facingRight = true;
        
        player.hans.position.x = 120.0f;
        player.hans.position.y = 300.0f;
        player.hans.velocity.x = 0.0f;
        player.hans.velocity.y = 0.0f;
        player.hans.grounded = false;
        player.hans.color = 0x4169E1FF;
        player.hans.animState = ANIM_IDLE;
        player.hans.animTimer = 0.0f;
        player.hans.animFrame = 0;
        player.hans.facingRight = true;
    }
}

// Helper: Update eines Charakters (für Zwei-Spieler)
static void updateCharacter(Character* c, InputState* input, float dt) {
    // Horizontale Bewegung
    if(input->moveLeft) {
        c->velocity.x = -MOVE_SPEED;
        c->facingRight = false;
    } else if(input->moveRight) {
        c->velocity.x = MOVE_SPEED;
        c->facingRight = true;
    } else {
        c->velocity.x = 0.0f;
    }
    
    // Animations-Zustände
    if(!c->grounded) {
        c->animState = ANIM_JUMP;
    } else if(c->velocity.x != 0.0f) {
        c->animState = ANIM_WALK;
    } else {
        c->animState = ANIM_IDLE;
    }
    
    // Animation-Timer
    if(c->animState == ANIM_WALK) {
        c->animTimer += dt;
        if(c->animTimer >= 0.15f) {
            c->animFrame = (c->animFrame + 1) % 4;
            c->animTimer = 0.0f;
        }
    } else {
        c->animFrame = 0;
        c->animTimer = 0.0f;
    }
    
    // Springen
    if(input->jump && c->grounded) {
        c->velocity.y = JUMP_FORCE;
        c->grounded = false;
        playSound(SFX_JUMP);
    }
    
    // Gravitation
    c->velocity.y += GRAVITY;
    if(c->velocity.y > MAX_FALL_SPEED) {
        c->velocity.y = MAX_FALL_SPEED;
    }
    
    // Position aktualisieren
    c->position.x += c->velocity.x;
    c->position.y += c->velocity.y;
    
    // Bildschirmgrenzen
    if(c->position.x < PLAYER_WIDTH / 2) {
        c->position.x = PLAYER_WIDTH / 2;
    }
    if(c->position.x > SCREEN_WIDTH - PLAYER_WIDTH / 2) {
        c->position.x = SCREEN_WIDTH - PLAYER_WIDTH / 2;
    }
}

// Helper: Kollision für einen Charakter
static void checkCollisionForCharacter(Character* c) {
    Tile* tiles = getLevelTiles();
    int tileCount = getLevelTileCount();
    
    c->grounded = false;
    
    Rect charRect = {
        c->position.x - PLAYER_WIDTH / 2,
        c->position.y - PLAYER_HEIGHT / 2,
        PLAYER_WIDTH,
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
            
            if(charRect.x < tileRect.x + tileRect.w &&
               charRect.x + charRect.w > tileRect.x &&
               charRect.y < tileRect.y + tileRect.h &&
               charRect.y + charRect.h > tileRect.y) {
                
                float overlapLeft = (charRect.x + charRect.w) - tileRect.x;
                float overlapRight = (tileRect.x + tileRect.w) - charRect.x;
                float overlapTop = (charRect.y + charRect.h) - tileRect.y;
                float overlapBottom = (tileRect.y + tileRect.h) - charRect.y;
                
                float minOverlap = overlapTop;
                if(overlapBottom < minOverlap) minOverlap = overlapBottom;
                if(overlapLeft < minOverlap) minOverlap = overlapLeft;
                if(overlapRight < minOverlap) minOverlap = overlapRight;
                
                if(minOverlap == overlapTop && c->velocity.y > 0) {
                    c->position.y = tileRect.y - PLAYER_HEIGHT / 2 - 0.1f;
                    c->velocity.y = 0;
                    c->grounded = true;
                } else if(minOverlap == overlapBottom && c->velocity.y < 0) {
                    c->position.y = tileRect.y + tileRect.h + PLAYER_HEIGHT / 2 + 0.1f;
                    c->velocity.y = 0;
                }
            }
        }
    }
}

void updatePlayer(float dt) {
    // Unverwundbarkeits-Timer
    if(player.invincibilityTimer > 0.0f) {
        player.invincibilityTimer -= dt;
    }
    
    if(player.mode == MODE_SINGLE_PLAYER) {
        // === SINGLE-PLAYER MODUS ===
        InputState* input = getInputState();
        
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
        
        // Animations-Zustände
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
        
        // Animation-Timer
        if(player.tanja.animState == ANIM_WALK) {
            player.tanja.animTimer += dt;
            player.hans.animTimer += dt;
            
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
        
        // Springen
        if(input->jump && player.grounded) {
            player.velocity.y = JUMP_FORCE;
            player.grounded = false;
            playSound(SFX_JUMP);
        }
        
        // Gravitation
        player.velocity.y += GRAVITY;
        if(player.velocity.y > MAX_FALL_SPEED) {
            player.velocity.y = MAX_FALL_SPEED;
        }
        
        // Position aktualisieren
        player.position.x += player.velocity.x;
        player.position.y += player.velocity.y;
        
        // Bildschirmgrenzen
        if(player.position.x < PLAYER_WIDTH / 2) {
            player.position.x = PLAYER_WIDTH / 2;
        }
        if(player.position.x > SCREEN_WIDTH - PLAYER_WIDTH / 2) {
            player.position.x = SCREEN_WIDTH - PLAYER_WIDTH / 2;
        }
        
        // Kollision (wie vorher)
        Tile* tiles = getLevelTiles();
        int tileCount = getLevelTileCount();
        
        player.grounded = false;
        
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
                
                if(playerRect.x < tileRect.x + tileRect.w &&
                   playerRect.x + playerRect.w > tileRect.x &&
                   playerRect.y < tileRect.y + tileRect.h &&
                   playerRect.y + playerRect.h > tileRect.y) {
                    
                    float overlapLeft = (playerRect.x + playerRect.w) - tileRect.x;
                    float overlapRight = (tileRect.x + tileRect.w) - playerRect.x;
                    float overlapTop = (playerRect.y + playerRect.h) - tileRect.y;
                    float overlapBottom = (tileRect.y + tileRect.h) - playerRect.y;
                    
                    float minOverlap = overlapTop;
                    if(overlapBottom < minOverlap) minOverlap = overlapBottom;
                    if(overlapLeft < minOverlap) minOverlap = overlapLeft;
                    if(overlapRight < minOverlap) minOverlap = overlapRight;
                    
                    if(minOverlap == overlapTop && player.velocity.y > 0) {
                        player.position.y = tileRect.y - PLAYER_HEIGHT / 2 - 0.1f;
                        player.velocity.y = 0;
                        player.grounded = true;
                    } else if(minOverlap == overlapBottom && player.velocity.y < 0) {
                        player.position.y = tileRect.y + tileRect.h + PLAYER_HEIGHT / 2 + 0.1f;
                        player.velocity.y = 0;
                    }
                }
            }
        }
        
    } else {
        // === ZWEI-SPIELER MODUS ===
        InputState* tanjaInput = getTanjaInput();
        InputState* hansInput = getHansInput();
        
        // Update beide Charaktere separat
        updateCharacter(&player.tanja, tanjaInput, dt);
        updateCharacter(&player.hans, hansInput, dt);
        
        // Kollision für beide
        checkCollisionForCharacter(&player.tanja);
        checkCollisionForCharacter(&player.hans);
        
        // Elastische Verbindung (ziehen wenn zu weit weg)
        float dx = player.hans.position.x - player.tanja.position.x;
        float dy = player.hans.position.y - player.tanja.position.y;
        float distance = sqrtf(dx * dx + dy * dy);
        
        if(distance > MAX_DISTANCE) {
            // Zu weit auseinander - ziehen zusammen
            float pullStrength = (distance - MAX_DISTANCE) * 0.3f;
            float angle = atan2f(dy, dx);
            
            // Tanja wird zu Hans gezogen
            player.tanja.position.x += cosf(angle) * pullStrength;
            player.tanja.position.y += sinf(angle) * pullStrength;
            
            // Hans wird zu Tanja gezogen
            player.hans.position.x -= cosf(angle) * pullStrength;
            player.hans.position.y -= sinf(angle) * pullStrength;
        }
        
        // Mittelpunkt für Sammelobjekt-Kollision
        player.position.x = (player.tanja.position.x + player.hans.position.x) / 2.0f;
        player.position.y = (player.tanja.position.y + player.hans.position.y) / 2.0f;
        
        // Grounded wenn mindestens einer am Boden ist
        player.grounded = player.tanja.grounded || player.hans.grounded;
    }
    
    // Sammelobjekte einsammeln (beide Modi gleich, nutzen player.position)
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
