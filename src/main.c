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
    
    // Hauptschleife
    while(running) {
        // WPAD_ScanPads wird jetzt hier gemacht, nicht in updateInput()!
        WPAD_ScanPads();
        
        u32 pressed = WPAD_ButtonsDown(WPAD_CHAN_0);
        
        // Beenden mit HOME
        if(pressed & WPAD_BUTTON_HOME) {
            running = false;
        }
        
        // Input verarbeiten
        updateInput();
        
        // Spiellogik aktualisieren (60 FPS = ~16.67ms pro Frame)
        updatePlayer(1.0f / 60.0f);
        updatePhysics();
        
        // Rendern
        GRRLIB_FillScreen(0x87CEEBFF); // Himmelblau
        
        renderLevel();
        renderPlayer();
        renderHUD();
        
        GRRLIB_Render();
    }
    
    // Aufräumen
    cleanupLevel();
    GRRLIB_Exit();
    
    return 0;
}
