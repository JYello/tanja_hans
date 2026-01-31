#include "audio.h"
#include <asndlib.h>
#include <gccore.h>
#include <ogc/lwp_watchdog.h>
#include <math.h>
#include <stdlib.h>

#define SAMPLE_RATE 48000
#define PI 3.14159265359f

static bool audioInitialized = false;

// Einfache Delay-Funktion mit Wii Timer
static void delayMs(int ms) {
    u64 start = gettime();
    u64 end = start + (ms * 1000ULL);  // Mikrosekunden
    while(gettime() < end) {
        // Warten
    }
}

// Einfache Tongeneration
static void generateTone(s16* buffer, int samples, float frequency, float duration, float volume) {
    for(int i = 0; i < samples; i++) {
        float t = (float)i / SAMPLE_RATE;
        float wave = sinf(2.0f * PI * frequency * t);
        
        // Envelope (Fade out)
        float envelope = 1.0f - (t / duration);
        if(envelope < 0.0f) envelope = 0.0f;
        
        buffer[i] = (s16)(wave * envelope * volume * 32767.0f);
    }
}

static void playTone(float frequency, float duration, float volume) {
    if(!audioInitialized) return;
    
    int samples = (int)(duration * SAMPLE_RATE);
    s16* buffer = (s16*)malloc(samples * sizeof(s16));
    
    if(buffer) {
        generateTone(buffer, samples, frequency, duration, volume);
        ASND_SetVoice(0, VOICE_MONO_16BIT, SAMPLE_RATE, 0, buffer, samples * sizeof(s16), volume * 255, volume * 255, NULL);
        // Note: Memory wird nicht sofort freigegeben, ASND braucht es noch
        // In Production sollte man hier besser Memory-Management machen
    }
}

void initAudio(void) {
    ASND_Init();
    ASND_Pause(0);
    audioInitialized = true;
}

void playSound(SoundEffect sfx) {
    if(!audioInitialized) return;
    
    switch(sfx) {
        case SFX_JUMP:
            // Boing! (Aufsteigender Ton)
            playTone(220.0f, 0.1f, 0.3f);  // A3
            break;
            
        case SFX_COLLECT_HEART:
            // Warmer Ding (Zweiklang)
            playTone(523.25f, 0.15f, 0.25f);  // C5
            // Kurze Pause, dann zweiter Ton
            delayMs(50);
            playTone(659.25f, 0.15f, 0.25f);  // E5
            break;
            
        case SFX_COLLECT_SUN:
            // Fröhlicher Pling (höher)
            playTone(880.0f, 0.12f, 0.25f);  // A5
            break;
            
        case SFX_HIT:
            // Sanftes Bonk (tiefer Ton)
            playTone(110.0f, 0.15f, 0.2f);  // A2
            break;
            
        case SFX_LEVEL_COMPLETE:
            // Kleine aufsteigende Melodie
            playTone(523.25f, 0.2f, 0.3f);  // C5
            delayMs(150);
            playTone(659.25f, 0.2f, 0.3f);  // E5
            delayMs(150);
            playTone(783.99f, 0.3f, 0.3f);  // G5
            break;
            
        default:
            break;
    }
}

void cleanupAudio(void) {
    if(audioInitialized) {
        ASND_End();
        audioInitialized = false;
    }
}
