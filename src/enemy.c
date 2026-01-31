#include "enemy.h"
#include "player.h"
#include "audio.h"
#include <math.h>

#define ENEMY_SIZE 20.0f
#define ENEMY_SPEED 1.5f

static Enemy enemies[MAX_ENEMIES];
static int enemyCount = 0;

void initEnemies(void) {
    enemyCount = 3;
    
    // Gegner 1: Patrol (hin und her)
    enemies[0].position.x = 250.0f;
    enemies[0].position.y = 300.0f;
    enemies[0].startPos.x = 250.0f;
    enemies[0].startPos.y = 300.0f;
    enemies[0].velocity.x = ENEMY_SPEED;
    enemies[0].velocity.y = 0.0f;
    enemies[0].moveRange = 80.0f;
    enemies[0].timer = 0.0f;
    enemies[0].type = ENEMY_PATROL;
    enemies[0].active = true;
    enemies[0].color = 0xFF6347FF;  // Tomaten-Rot
    
    // Gegner 2: Hover (auf und ab)
    enemies[1].position.x = 450.0f;
    enemies[1].position.y = 220.0f;
    enemies[1].startPos.x = 450.0f;
    enemies[1].startPos.y = 220.0f;
    enemies[1].velocity.x = 0.0f;
    enemies[1].velocity.y = ENEMY_SPEED;
    enemies[1].moveRange = 60.0f;
    enemies[1].timer = 0.0f;
    enemies[1].type = ENEMY_HOVER;
    enemies[1].active = true;
    enemies[1].color = 0x9370DBFF;  // Lila
    
    // Gegner 3: Circle (im Kreis)
    enemies[2].position.x = 100.0f;
    enemies[2].position.y = 350.0f;
    enemies[2].startPos.x = 100.0f;
    enemies[2].startPos.y = 350.0f;
    enemies[2].velocity.x = 0.0f;
    enemies[2].velocity.y = 0.0f;
    enemies[2].moveRange = 40.0f;  // Radius
    enemies[2].timer = 0.0f;
    enemies[2].type = ENEMY_CIRCLE;
    enemies[2].active = true;
    enemies[2].color = 0xFF8C00FF;  // Orange
}

void updateEnemies(float dt) {
    for(int i = 0; i < enemyCount; i++) {
        if(!enemies[i].active) continue;
        
        enemies[i].timer += dt;
        
        switch(enemies[i].type) {
            case ENEMY_PATROL:
                // Hin und her
                enemies[i].position.x += enemies[i].velocity.x;
                
                if(enemies[i].position.x > enemies[i].startPos.x + enemies[i].moveRange) {
                    enemies[i].velocity.x = -ENEMY_SPEED;
                } else if(enemies[i].position.x < enemies[i].startPos.x - enemies[i].moveRange) {
                    enemies[i].velocity.x = ENEMY_SPEED;
                }
                break;
                
            case ENEMY_HOVER:
                // Auf und ab
                enemies[i].position.y += enemies[i].velocity.y;
                
                if(enemies[i].position.y > enemies[i].startPos.y + enemies[i].moveRange) {
                    enemies[i].velocity.y = -ENEMY_SPEED;
                } else if(enemies[i].position.y < enemies[i].startPos.y - enemies[i].moveRange) {
                    enemies[i].velocity.y = ENEMY_SPEED;
                }
                break;
                
            case ENEMY_CIRCLE:
                // Im Kreis
                enemies[i].position.x = enemies[i].startPos.x + cosf(enemies[i].timer) * enemies[i].moveRange;
                enemies[i].position.y = enemies[i].startPos.y + sinf(enemies[i].timer) * enemies[i].moveRange;
                break;
        }
        
        // Kollision mit Spieler (mit Unverwundbarkeit)
        PlayerPair* player = getPlayer();
        
        if(player->invincibilityTimer <= 0.0f) {
            float dx = player->position.x - enemies[i].position.x;
            float dy = player->position.y - enemies[i].position.y;
            float distance = sqrtf(dx * dx + dy * dy);
            
            if(distance < 35.0f) {  // Kollisionsradius
                // Kein Herzverlust mehr! Nur sanfter Rückstoß
                player->invincibilityTimer = 1.0f;  // 1 Sekunde Unverwundbarkeit
                playSound(SFX_HIT);
                
                // Sanfter Rückstoß (kleiner als vorher)
                if(dx > 0) {
                    player->velocity.x = 3.0f;
                } else {
                    player->velocity.x = -3.0f;
                }
                player->velocity.y = -3.0f;
            }
        }
    }
}

Enemy* getEnemies(void) {
    return enemies;
}

int getEnemyCount(void) {
    return enemyCount;
}
