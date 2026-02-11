#ifndef GLOBALS_H
#define GLOBALS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

extern int screenWidth;
extern int screenHeight;

enum Screen {
    CLOCK_SCREEN,
    HEARTBEAT_SCREEN,
    BATTERY_SCREEN
};

extern Screen currentScreen;

// Clock variables
extern int hours;
extern int minutes;
extern int seconds;
extern float lastSecondUpdate;

// Arrow variables
extern float rightArrowX;
extern float leftArrowX;
extern float arrowY;
extern float arrowWidth;
extern float arrowHeight;
extern float arrowSize;

// EKG variables
extern float ekgOffsetX;
extern float ekgSpeed;
extern float currentBPM;
extern float lastBPMUpdate;
extern bool warningActive;

// Running variables
extern bool isRunning;
extern float runningTime;
extern float baseBPM;
extern float bpmIncreaseRate;
extern float totalRunningTime;  

// Battery variables
extern float batteryLevel;
extern float lastBatteryUpdate;

// 3D Camera variables
extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;
extern float cameraYaw;
extern float cameraPitch;
extern float cameraSpeed;
extern double lastMouseY;
extern bool firstMouse;

// Watch interaction
extern bool watchFocused;
extern glm::vec3 watchPosition;
extern glm::vec3 focusedCameraPos;
extern glm::vec3 normalCameraPos;

// Ground movement
extern float groundOffset;
extern float groundSpeed;
extern const int groundSegments;
extern const float segmentLength;

// Lighting
extern glm::vec3 sunLightPos;
extern glm::vec3 sunLightColor;
extern float screenLightIntensity;

// VAOs for 2D screen rendering
extern unsigned int VAOrect;
extern unsigned int VAObackground;
extern unsigned int VAOline;
extern unsigned int VAOekg;

// VAOs for 3D scene
extern unsigned int VAOground;
extern unsigned int VAOwatch;
extern unsigned int VAOscreen;

// Shaders
extern unsigned int basicShader;          // 2D screen shader
extern unsigned int gradientShader;       // 2D gradient shader
extern unsigned int lineShader;           // 2D line shader
extern unsigned int sceneShader;          // 3D scene shader with lighting
extern unsigned int screenDisplayShader;  // Shader for watch screen
extern unsigned int groundShader;         // Shader for ground

// Textures for 2D screen
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

// Textures for 3D scene
extern unsigned int groundTexture;
extern unsigned int screenRenderTexture;
extern unsigned int screenFBO;

extern GLFWcursor* cursor;

extern float rectangleVertices[16];
extern unsigned int rectangleIndices[6];
extern float backgroundVertices[8];
extern unsigned int backgroundIndices[6];

extern bool depthTestEnabled;
extern bool faceCullingEnabled;

#endif