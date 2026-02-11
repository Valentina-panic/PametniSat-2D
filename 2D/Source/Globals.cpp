#include "../Header/Globals.h"

int screenWidth = 1920;
int screenHeight = 1080;

Screen currentScreen = CLOCK_SCREEN;

int hours = 0;
int minutes = 0;
int seconds = 0;
float lastSecondUpdate = 0.0f;

float rightArrowX = 0.55f;
float leftArrowX = -0.55f;
float arrowY = 0.0f;
float arrowWidth = 0.15f;
float arrowHeight = 0.15f;
float arrowSize = 0.12f;

float ekgOffsetX = 0.0f;
float ekgSpeed = 0.3f;
float currentBPM = 70.0f;
float lastBPMUpdate = 0.0f;
bool warningActive = false;

bool isRunning = false;
float runningTime = 0.0f;
float baseBPM = 70.0f;
float bpmIncreaseRate = 15.0f;

float batteryLevel = 100.0f;
float lastBatteryUpdate = 0.0f;

unsigned int VAOrect;
unsigned int VAObackground;
unsigned int VAOline;
unsigned int VAOekg;

unsigned int basicShader;
unsigned int gradientShader;
unsigned int lineShader;

unsigned int digitTextures[10];
unsigned int arrowTexture;
unsigned int infoTexture;
unsigned int colonTexture;
unsigned int ekgTexture;
unsigned int borderTexture;
unsigned int bpmTextTexture;
unsigned int letterBTexture;
unsigned int letterPTexture;
unsigned int letterMTexture;
unsigned int redTexture;
unsigned int letterTextures[26];
unsigned int batteryTexture;
unsigned int percentTexture;

GLFWcursor* cursor;

float rectangleVertices[] = {
    -0.7f,  0.7f,    0.0f, 1.0f,
    -0.7f, -0.7f,    0.0f, 0.0f,
     0.7f, -0.7f,    1.0f, 0.0f,
     0.7f,  0.7f,    1.0f, 1.0f
};

unsigned int rectangleIndices[] = {
    0, 1, 2,
    0, 2, 3
};

float backgroundVertices[] = {
    -1.0f,  1.0f,
    -1.0f, -1.0f,
     1.0f, -1.0f,
     1.0f,  1.0f
};

unsigned int backgroundIndices[] = {
    0, 1, 2,
    0, 2, 3
};