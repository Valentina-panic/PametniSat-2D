#ifndef COMMON_RENDERING_H
#define COMMON_RENDERING_H

#include "Globals.h"

void drawGradientBackground();
void drawDigit(int digit, float x, float y, float size);
void drawColon(float x, float y, float size);
void drawArrow(float x, float y, float size, bool flipped = false);
void drawText(const char* text, float startX, float y, float size);
void drawInfoPanel(float alpha = 0.6f);

#endif