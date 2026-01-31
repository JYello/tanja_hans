#ifndef AUDIO_H
#define AUDIO_H

typedef enum {
    SFX_JUMP,
    SFX_COLLECT_HEART,
    SFX_COLLECT_SUN,
    SFX_HIT,
    SFX_LEVEL_COMPLETE,
    SFX_COUNT
} SoundEffect;

void initAudio(void);
void playSound(SoundEffect sfx);
void cleanupAudio(void);

#endif // AUDIO_H
