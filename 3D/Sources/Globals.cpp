#include "../Headers/Globals.h"

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
float totalRunningTime = 0.0f; 

// 3D Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 1.5f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraYaw = -90.0f;
float cameraPitch = 0.0f;
float cameraSpeed = 2.5f;
double lastMouseY = 0.0;
bool firstMouse = true;

// Watch
bool watchFocused = false;
glm::vec3 watchPosition = glm::vec3(0.5f, 1.3f, 0.5f);
glm::vec3 focusedCameraPos = glm::vec3(0.5f, 1.3f, 1.2f);
glm::vec3 normalCameraPos = glm::vec3(0.0f, 1.5f, 3.0f);

// Ground
float groundOffset = 0.0f;
float groundSpeed = 2.0f;
const int groundSegments = 10;
const float segmentLength = 5.0f;

// Lighting
glm::vec3 sunLightPos = glm::vec3(0.0f, 10.0f, 5.0f);
glm::vec3 sunLightColor = glm::vec3(1.0f, 1.0f, 0.9f);
float screenLightIntensity = 0.3f;

// Battery
float batteryLevel = 100.0f;
float lastBatteryUpdate = 0.0f;

unsigned int VAOrect;
unsigned int VAObackground;
unsigned int VAOline;
unsigned int VAOekg;
unsigned int VAOground;
unsigned int VAOwatch;
unsigned int VAOscreen;

unsigned int basicShader;
unsigned int gradientShader;
unsigned int lineShader;
unsigned int sceneShader;
unsigned int screenDisplayShader;
unsigned int groundShader;

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

unsigned int groundTexture;
unsigned int screenRenderTexture;
unsigned int screenFBO;

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

bool depthTestEnabled = true;
bool faceCullingEnabled = true;