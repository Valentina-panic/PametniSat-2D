#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <ctime>
#include "../Header/Util.h"
#include "../Header/Globals.h"        
#include "../Header/Initialization.h" 
#include "../Header/Callbacks.h"
#include "../Header/Clock.h"   
#include "../Header/Ekg.h"      
#include "../Header/Battery.h"      
#include "../Header/CommonRendering.h"

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    screenWidth = mode->width;
    screenHeight = mode->height;

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Pametni sat", monitor, NULL);
    if (window == NULL) return endProgram("Prozor nije uspeo da se kreira.");
    glfwMakeContextCurrent(window);

    glfwSetMouseButtonCallback(window, mouse_button_callback);

    cursor = loadImageToCursor("Resources/cursor.png");
    glfwSetCursor(window, cursor);

    if (glewInit() != GLEW_OK) return endProgram("GLEW nije uspeo da se inicijalizuje.");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.2f, 0.8f, 0.6f, 1.0f);

    basicShader = createShader("Shaders/basic.vert", "Shaders/basic.frag");
    gradientShader = createShader("Shaders/gradient.vert", "Shaders/gradient.frag");
    lineShader = createShader("Shaders/line.vert", "Shaders/line.frag");

    formVAO(rectangleVertices, sizeof(rectangleVertices), rectangleIndices, sizeof(rectangleIndices), VAOrect);
    formBackgroundVAO();
    formLineVAO();
    formEkgVAO();

    loadTextures();
    initializeTime();

    srand(time(NULL));
    double lastFrameTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        double initFrameTime = glfwGetTime();
        float deltaTime = initFrameTime - lastFrameTime;
        lastFrameTime = initFrameTime;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        if (currentScreen == HEARTBEAT_SCREEN) {
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                isRunning = true;
            }
            else {
                isRunning = false;
                runningTime = 0.0f;
            }
        }

        float currentTime = glfwGetTime();

        if (currentScreen == CLOCK_SCREEN) {
            updateClock(currentTime);
        }
        else if (currentScreen == HEARTBEAT_SCREEN) {
            updateBPM(currentTime, deltaTime);
        }
        else if (currentScreen == BATTERY_SCREEN) {
            updateBattery(currentTime);
        }

        glClear(GL_COLOR_BUFFER_BIT);

        drawGradientBackground();

        if (currentScreen == CLOCK_SCREEN) {
            drawClockScreen();
        }
        else if (currentScreen == HEARTBEAT_SCREEN) {
            drawHeartbeatScreen(deltaTime);
        }
        else if (currentScreen == BATTERY_SCREEN) {
            drawBatteryScreen();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
        while (glfwGetTime() - initFrameTime < 1 / 75.0) {}
    }

    glDeleteVertexArrays(1, &VAOrect);
    glDeleteVertexArrays(1, &VAObackground);
    glDeleteVertexArrays(1, &VAOline);
    glDeleteVertexArrays(1, &VAOekg);
    glDeleteProgram(basicShader);
    glDeleteProgram(gradientShader);
    glDeleteProgram(lineShader);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}