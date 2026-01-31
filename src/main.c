#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <grrlib.h>

#include "game.h"
#include "player.h"
#include "physics.h"
#include "level.h"
#include "input.h"
#include "render.h"
#include "enemy.h"
#include "audio.h"
#include "font.h"

// Globale Variablen
static bool running = true;
static GameState gameState = STATE_PLAYING;
static float levelCompleteTimer = 0.0f;

int main(int argc, char **argv) {
    // Video initialisieren
    GRRLIB_Init();
    WPAD_Init();
    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
    
    // Audio initialisieren
    initAudio();
    
    // Level laden
    initLevel();
    
    // Spieler initialisieren
    initPlayer();
    
    // Gegner initialisieren
    initEnemies();
    
    // Hauptschleife
    while(running) {
        // WPAD_ScanPads wird jetzt hier gemacht, nicht in updateInput()!
        WPAD_ScanPads();
        
        u32 pressed = WPAD_ButtonsDown(WPAD_CHAN_0);
        
        // Beenden mit HOME
        if(pressed & WPAD_BUTTON_HOME) {
            running = false;
        }
        
        PlayerPair* player = getPlayer();
        
        // Game Over Check - nur wenn man runterfällt
        if(player->position.y > SCREEN_HEIGHT + 100) {
            gameState = STATE_GAMEOVER;
        }
        
        // Level Complete Check
        if(gameState == STATE_PLAYING && player->suns >= getTotalSuns()) {
            levelCompleteTimer += 1.0f / 60.0f;
            
            if(levelCompleteTimer >= 2.0f) {  // 2 Sekunden Pause
                int nextLevel = getCurrentLevel() + 1;
                
                if(nextLevel <= 2) {  // Wir haben 2 Levels
                    loadLevel(nextLevel);
                    initPlayer();
                    initEnemies();
                    levelCompleteTimer = 0.0f;
                    playSound(SFX_LEVEL_COMPLETE);
                } else {
                    // Alle Levels geschafft - Victory!
                    gameState = STATE_GAMEOVER;
                    levelCompleteTimer = -1.0f;  // Marker für Victory (negativ!)
                    playSound(SFX_LEVEL_COMPLETE);
                }
            }
        } else if(gameState == STATE_PLAYING) {
            levelCompleteTimer = 0.0f;
        }
        
        // Input verarbeiten
        updateInput();
        
        if(gameState == STATE_PLAYING) {
            // Spiellogik aktualisieren (60 FPS = ~16.67ms pro Frame)
            updatePlayer(1.0f / 60.0f);
            updateEnemies(1.0f / 60.0f);
            updatePhysics();
        } else if(gameState == STATE_GAMEOVER) {
            // Bei Game Over: A zum Neustart
            if(pressed & WPAD_BUTTON_A) {
                // Neustart von Level 1
                loadLevel(1);
                initPlayer();
                initEnemies();
                gameState = STATE_PLAYING;
                levelCompleteTimer = 0.0f;
            }
        }
        
        // Rendern
        GRRLIB_FillScreen(0x87CEEBFF); // Himmelblau
        
        if(gameState == STATE_PLAYING) {
            renderLevel();
            renderPlayer();
            renderHUD();
            
            // Level Complete Nachricht
            if(player->suns >= getTotalSuns() && levelCompleteTimer > 0.0f) {
                GRRLIB_Rectangle(
                    SCREEN_WIDTH / 2 - 150,
                    SCREEN_HEIGHT / 2 - 40,
                    300,
                    80,
                    0x00FF00CC,  // Grün halbtransparent
                    true
                );
                GRRLIB_Rectangle(
                    SCREEN_WIDTH / 2 - 150,
                    SCREEN_HEIGHT / 2 - 40,
                    300,
                    80,
                    COL_WHITE,
                    false
                );
                
                drawTextCentered("LEVEL GESCHAFFT!", SCREEN_HEIGHT / 2 - 20, COL_WHITE, 2.0f);
                
                if(getCurrentLevel() < 2) {
                    drawTextCentered("NAECHSTES LEVEL...", SCREEN_HEIGHT / 2 + 10, COL_YELLOW, 1.5f);
                }
            }
        } else if(gameState == STATE_GAMEOVER) {
            // Victory oder Game Over Screen
            if(levelCompleteTimer < 0.0f) {
                // VICTORY!
                GRRLIB_Rectangle(
                    SCREEN_WIDTH / 2 - 180,
                    SCREEN_HEIGHT / 2 - 80,
                    360,
                    160,
                    0xFFD700CC,  // Gold halbtransparent
                    true
                );
                GRRLIB_Rectangle(
                    SCREEN_WIDTH / 2 - 180,
                    SCREEN_HEIGHT / 2 - 80,
                    360,
                    160,
                    COL_WHITE,
                    false
                );
                
                drawTextCentered("GEWONNEN!", SCREEN_HEIGHT / 2 - 50, COL_WHITE, 3.0f);
                drawTextCentered("TANJA UND HANS", SCREEN_HEIGHT / 2 - 10, COL_WHITE, 2.0f);
                drawTextCentered("HAND IN HAND!", SCREEN_HEIGHT / 2 + 20, COL_WHITE, 2.0f);
                drawTextCentered("DRUECKE A", SCREEN_HEIGHT / 2 + 55, COL_YELLOW, 1.5f);
            } else {
                // Game Over (runtergefallen)
                GRRLIB_Rectangle(
                    SCREEN_WIDTH / 2 - 150,
                    SCREEN_HEIGHT / 2 - 60,
                    300,
                    120,
                    0x000000CC,  // Halbtransparent schwarz
                    true
                );
                GRRLIB_Rectangle(
                    SCREEN_WIDTH / 2 - 150,
                    SCREEN_HEIGHT / 2 - 60,
                    300,
                    120,
                    COL_WHITE,
                    false
                );
                
                drawTextCentered("GAME OVER", SCREEN_HEIGHT / 2 - 25, COL_RED, 2.5f);
                drawTextCentered("DRUECKE A ZUM", SCREEN_HEIGHT / 2 + 10, COL_WHITE, 1.5f);
                drawTextCentered("NEUSTARTEN", SCREEN_HEIGHT / 2 + 35, COL_WHITE, 1.5f);
            }
        }
        
        GRRLIB_Render();
    }
    
    // Aufräumen
    cleanupLevel();
    cleanupAudio();
    GRRLIB_Exit();
    
    return 0;
}
