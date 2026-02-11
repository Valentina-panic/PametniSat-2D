#ifndef GLOBALS_H
#define GLOBALS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

extern int screenWidth;
extern int screenHeight;

enum Screen {
    CLOCK_SCREEN,
    HEARTBEAT_SCREEN,
    BATTERY_SCREEN
};

extern Screen currentScreen;

extern int hours;
extern int minutes;
extern int seconds;
extern float lastSecondUpdate;

extern float rightArrowX;
extern float leftArrowX;
extern float arrowY;
extern float arrowWidth;
extern float arrowHeight;
extern float arrowSize;

extern float ekgOffsetX;
extern float ekgSpeed;
extern float currentBPM;
extern float lastBPMUpdate;
extern bool warningActive;

extern bool isRunning;
extern float runningTime;
extern float baseBPM;
extern float bpmIncreaseRate;

extern float batteryLevel;
extern float lastBatteryUpdate;

extern unsigned int VAOrect;
extern unsigned int VAObackground;
extern unsigned int VAOline;
extern unsigned int VAOekg;

extern unsigned int basicShader;
extern unsigned int gradientShader;
extern unsigned int lineShader;

extern unsigned int digitTextures[10];
extern unsigned int arrowTexture;
extern unsigned int infoTexture;
extern unsigned int colonTexture;
extern unsigned int ekgTexture;
extern unsigned int borderTexture;
extern unsigned int bpmTextTexture;
extern unsigned int letterBTexture;
extern unsigned int letterPTexture;
extern unsigned int letterMTexture;
extern unsigned int redTexture;
extern unsigned int letterTextures[26];
extern unsigned int batteryTexture;
extern unsigned int percentTexture;

extern GLFWcursor* cursor;

extern float rectangleVertices[16];
extern unsigned int rectangleIndices[6];
extern float backgroundVertices[8];
extern unsigned int backgroundIndices[6];

#endif