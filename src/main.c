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

// Globale Variablen
static bool running = true;
static GameState gameState = STATE_PLAYING;

int main(int argc, char **argv) {
    // Video initialisieren
    GRRLIB_Init();
    WPAD_Init();
    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
    
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
        
        // Game Over Check
        if(player->hearts <= 0) {
            gameState = STATE_GAMEOVER;
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
                // Neustart
                initPlayer();
                initEnemies();
                gameState = STATE_PLAYING;
            }
        }
        
        // Rendern
        GRRLIB_FillScreen(0x87CEEBFF); // Himmelblau
        
        if(gameState == STATE_PLAYING) {
            renderLevel();
            renderPlayer();
            renderHUD();
        } else if(gameState == STATE_GAMEOVER) {
            // Game Over Screen
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
        }
        
        GRRLIB_Render();
    }
    
    // Aufräumen
    cleanupLevel();
    GRRLIB_Exit();
    
    return 0;
}
