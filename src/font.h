#ifndef FONT_H
#define FONT_H

#include <grrlib.h>

// Einfaches 5x7 Bitmap-Font für Großbuchstaben, Zahlen und Satzzeichen
void drawText(const char* text, float x, float y, u32 color, float scale);
void drawTextCentered(const char* text, float y, u32 color, float scale);

#endif // FONT_H
