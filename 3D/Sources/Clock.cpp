#include "../Headers/CommonRendering.h"
#include "../Headers/Clock.h"

void updateClock(float currentTime) {
    if (currentTime - lastSecondUpdate >= 1.0f) {
        seconds++;

        if (seconds >= 60) {
            seconds = 0;
            minutes++;
        }
        if (minutes >= 60) {
            minutes = 0;
            hours++;
        }
        if (hours >= 24) {
            hours = 0;
        }
        lastSecondUpdate = currentTime;
    }
}

void drawClockScreen() {
    float digitSize = 0.12f;
    float spacing = digitSize * 1.3f;
    float startX = -spacing * 4.3f;
    float y = 0.0f;

    int h1 = hours / 10;
    int h2 = hours % 10;
    int m1 = minutes / 10;
    int m2 = minutes % 10;
    int s1 = seconds / 10;
    int s2 = seconds % 10;

    float x = startX;

    drawDigit(h1, x, y, digitSize);
    x += spacing;
    drawDigit(h2, x, y, digitSize);
    x += spacing;

    drawColon(x, y, digitSize);
    x += spacing;

    drawDigit(m1, x, y, digitSize);
    x += spacing;
    drawDigit(m2, x, y, digitSize);
    x += spacing;

    drawColon(x, y, digitSize);
    x += spacing;

    drawDigit(s1, x, y, digitSize);
    x += spacing;
    drawDigit(s2, x, y, digitSize);

    drawArrow(rightArrowX, arrowY, arrowSize);
}