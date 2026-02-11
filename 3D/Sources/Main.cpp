#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <ctime>
#include "../Headers/Util.h"
#include "../Headers/Globals.h"        
#include "../Headers/Initialization.h" 
#include "../Headers/Callbacks.h"
#include "../Headers/Clock.h"   
#include "../Headers/Ekg.h"      
#include "../Headers/Battery.h"      
#include "../Headers/CommonRendering.h"
#include "../Headers/shader.hpp"
#include "../Headers/model.hpp"

void renderScreenToTexture(float deltaTime);
void render3DScene(Model& armModel, Model& buildingModel, Shader& modelShader, float deltaTime);

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

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Pametni sat 3D - Valentina Panic RA 144/2022", monitor, NULL);
    if (window == NULL) return endProgram("Prozor nije uspeo da se kreira.");
    glfwMakeContextCurrent(window);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    cursor = loadImageToCursor("Resources/cursor.png");

    if (glewInit() != GLEW_OK) return endProgram("GLEW nije uspeo da se inicijalizuje.");

    // Enable depth testing and face culling
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    if (!depthTestEnabled) {
        glDisable(GL_DEPTH_TEST);
    }
    if (!faceCullingEnabled) {
        glDisable(GL_CULL_FACE);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.4f, 0.6f, 0.9f, 1.0f);

    basicShader = createShader("Shaders/basic2d.vert", "Shaders/basic2d.frag");
    gradientShader = createShader("Shaders/gradient.vert", "Shaders/gradient.frag");
    lineShader = createShader("Shaders/line.vert", "Shaders/line.frag");
    sceneShader = createShader("Shaders/scene.vert", "Shaders/scene.frag");
    screenDisplayShader = createShader("Shaders/screen.vert", "Shaders/screen.frag");

    formVAO(rectangleVertices, sizeof(rectangleVertices), rectangleIndices, sizeof(rectangleIndices), VAOrect);
    formBackgroundVAO();
    formLineVAO();
    formEkgVAO();
    setupGroundVAO();
    setupFramebuffer();

    loadTextures();
    initializeTime();

    Model armModel("Models/hand.obj");
    Model buildingModel("Models/building.obj");

    Shader modelShader("Shaders/model.vert", "Shaders/model.frag");

    srand(time(NULL));
    double lastFrameTime = glfwGetTime();

    std::cout << "\n========================================" << std::endl;
    std::cout << "KONTROLE:" << std::endl;
    std::cout << "Z - Toggle Depth Test" << std::endl;
    std::cout << "X - Toggle Face Culling" << std::endl;
    std::cout << "========================================\n" << std::endl;

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
            }
        }
        else {
            isRunning = false;
        }

        if (isRunning && currentScreen == HEARTBEAT_SCREEN) {
            float bobAmount = 0.05f;
            float bobSpeed = 10.0f;
            float bobOffset = sin(initFrameTime * bobSpeed) * bobAmount;

            normalCameraPos.y = 1.5f + bobOffset;
            focusedCameraPos.y = 1.3f + bobOffset;

            groundOffset += groundSpeed * deltaTime;

            if (groundOffset >= segmentLength) {
                groundOffset -= segmentLength;
            }
        }
        else {
            normalCameraPos.y = 1.5f;
            focusedCameraPos.y = 1.3f;
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

        renderScreenToTexture(deltaTime);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, screenWidth, screenHeight);
        glClearColor(0.4f, 0.6f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render3DScene(armModel, buildingModel, modelShader, deltaTime);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Frame limiter: 75 FPS
        while (glfwGetTime() - initFrameTime < 1.0 / 75.0) {}
    }

    glDeleteVertexArrays(1, &VAOrect);
    glDeleteVertexArrays(1, &VAObackground);
    glDeleteVertexArrays(1, &VAOline);
    glDeleteVertexArrays(1, &VAOekg);
    glDeleteVertexArrays(1, &VAOground);
    glDeleteProgram(basicShader);
    glDeleteProgram(gradientShader);
    glDeleteProgram(lineShader);
    glDeleteProgram(sceneShader);
    glDeleteProgram(screenDisplayShader);
    glDeleteFramebuffers(1, &screenFBO);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void renderScreenToTexture(float deltaTime) {
    // Render to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);
    glViewport(0, 0, 1024, 1024);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    drawGradientBackground();

    switch (currentScreen) {
    case CLOCK_SCREEN:
        drawClockScreen();
        break;
    case HEARTBEAT_SCREEN:
        drawHeartbeatScreen(deltaTime);
        break;
    case BATTERY_SCREEN:
        drawBatteryScreen();
        break;
    }

    if (depthTestEnabled)
        glEnable(GL_DEPTH_TEST);

    if (faceCullingEnabled)
        glEnable(GL_CULL_FACE);
}

void render3DScene(Model& armModel, Model& buildingModel, Shader& modelShader, float deltaTime) {
    glm::vec3 currentCameraPos = watchFocused ? focusedCameraPos : normalCameraPos;
    glm::vec3 lookTarget = watchFocused ? watchPosition : (cameraPos + cameraFront);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
        (float)screenWidth / (float)screenHeight,
        0.1f, 100.0f);

    glm::mat4 view = glm::lookAt(currentCameraPos, lookTarget, cameraUp);

    glUseProgram(sceneShader);
    glUniformMatrix4fv(glGetUniformLocation(sceneShader, "uProjection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(sceneShader, "uView"), 1, GL_FALSE, glm::value_ptr(view));
    glUniform3fv(glGetUniformLocation(sceneShader, "uLightPos"), 1, glm::value_ptr(sunLightPos));
    glUniform3fv(glGetUniformLocation(sceneShader, "uLightColor"), 1, glm::value_ptr(sunLightColor));
    glUniform3fv(glGetUniformLocation(sceneShader, "uViewPos"), 1, glm::value_ptr(currentCameraPos));
    glUniform3fv(glGetUniformLocation(sceneShader, "uScreenLightPos"), 1, glm::value_ptr(watchPosition));
    glUniform3f(glGetUniformLocation(sceneShader, "uScreenLightColor"), 0.5f, 0.5f, 1.0f);
    glUniform1f(glGetUniformLocation(sceneShader, "uScreenLightIntensity"), screenLightIntensity);
    glUniform1i(glGetUniformLocation(sceneShader, "uUseScreenLight"), 1);
    glUniform1i(glGetUniformLocation(sceneShader, "uUseTexture"), 1);

    // GROUND
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, groundTexture);
    glBindVertexArray(VAOground);

    glm::mat4 groundModel = glm::mat4(1.0f);
    groundModel = glm::translate(groundModel, glm::vec3(0.0f, 0.0f, groundOffset));

    glUniformMatrix4fv(glGetUniformLocation(sceneShader, "uModel"), 1, GL_FALSE, glm::value_ptr(groundModel));
    glUniform3f(glGetUniformLocation(sceneShader, "uObjectColor"), 0.5f, 0.5f, 0.5f);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);

    modelShader.use();
    modelShader.setMat4("uP", projection);
    modelShader.setMat4("uV", view);
    modelShader.setVec3("uLightPos", sunLightPos);
    modelShader.setVec3("uViewPos", currentCameraPos);
    modelShader.setVec3("uLightColor", sunLightColor);
    modelShader.setVec3("uScreenLightPos", watchPosition);
    modelShader.setVec3("uScreenLightColor", glm::vec3(0.5f, 0.5f, 1.0f));
    modelShader.setFloat("uScreenLightIntensity", screenLightIntensity);
    modelShader.setBool("uUseScreenLight", true);
    modelShader.setBool("uHasTexture", false);
    modelShader.setVec3("uMaterialColor", glm::vec3(0.7f, 0.7f, 0.65f));

    struct BuildingType {
        float scale;
        float xOffset;
    };

    BuildingType rightBuildings[] = {
        {1.5f, 5.0f},
        {2.2f, 6.5f},
        {1.8f, 8.0f},
        {2.5f, 10.0f}
    };

    glDisable(GL_CULL_FACE);

    for (int type = 0; type < 4; type++) {
        for (int j = 0; j < 20; j++) {
            float xPos = rightBuildings[type].xOffset;
            float scale = rightBuildings[type].scale;
            float baseZ = -8.0f - (j * 8.0f);
            float zPos = baseZ + groundOffset;
            while (zPos > 10.0f) {
                zPos -= 160.0f;
            }

            glm::mat4 buildingModelMat = glm::mat4(1.0f);
            buildingModelMat = glm::translate(buildingModelMat, glm::vec3(xPos, -0.1f, zPos));
            buildingModelMat = glm::scale(buildingModelMat, glm::vec3(scale, scale, scale));

            modelShader.setMat4("uM", buildingModelMat);
            buildingModel.Draw(modelShader);
        }
    }

    for (int type = 0; type < 4; type++) {
        for (int j = 0; j < 20; j++) {
            float xPos = -rightBuildings[type].xOffset;
            float scale = rightBuildings[type].scale;
            float baseZ = -8.0f - (j * 8.0f);
            float zPos = baseZ + groundOffset;
            while (zPos > 10.0f) {
                zPos -= 160.0f;
            }

            glm::mat4 buildingModelMat = glm::mat4(1.0f);
            buildingModelMat = glm::translate(buildingModelMat, glm::vec3(xPos, -0.1f, zPos));
            buildingModelMat = glm::scale(buildingModelMat, glm::vec3(scale, scale, scale));

            modelShader.setMat4("uM", buildingModelMat);
            buildingModel.Draw(modelShader);
        }
    }

    if (faceCullingEnabled) {
        glEnable(GL_CULL_FACE);
    }

    glm::mat4 armModelMat = glm::mat4(1.0f);

    if (watchFocused) {
        armModelMat = glm::translate(armModelMat, watchPosition + glm::vec3(0.05f, -0.15f, -0.25f));

        if (isRunning && currentScreen == HEARTBEAT_SCREEN) {
            float bobAmount = 0.02f;
            float bobSpeed = 10.0f;
            armModelMat = glm::translate(armModelMat, glm::vec3(0.0f, sin(glfwGetTime() * bobSpeed) * bobAmount, 0.0f));
        }

        armModelMat = glm::rotate(armModelMat, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        armModelMat = glm::rotate(armModelMat, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        armModelMat = glm::rotate(armModelMat, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        armModelMat = glm::scale(armModelMat, glm::vec3(-0.1f, 0.1f, 0.1f));
    }
    else {
        armModelMat = glm::translate(armModelMat, watchPosition + glm::vec3(0.05f, 0.1f, -0.1f));

        if (isRunning && currentScreen == HEARTBEAT_SCREEN) {
            float bobAmount = 0.02f;
            float bobSpeed = 10.0f;
            armModelMat = glm::translate(armModelMat, glm::vec3(0.0f, sin(glfwGetTime() * bobSpeed) * bobAmount, 0.0f));
        }

        armModelMat = glm::rotate(armModelMat, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        armModelMat = glm::rotate(armModelMat, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        armModelMat = glm::rotate(armModelMat, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        armModelMat = glm::scale(armModelMat, glm::vec3(-0.1f, 0.1f, 0.1f));
    }

    modelShader.setBool("uHasTexture", false);
    modelShader.setVec3("uMaterialColor", glm::vec3(1.0f, 0.85f, 0.7f));
    modelShader.setVec3("uLightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    modelShader.setMat4("uM", armModelMat);

    glDisable(GL_CULL_FACE);
    armModel.Draw(modelShader);
    if (faceCullingEnabled) {
        glEnable(GL_CULL_FACE);
    }

    modelShader.setVec3("uLightColor", sunLightColor);

    glUseProgram(screenDisplayShader);
    glUniformMatrix4fv(glGetUniformLocation(screenDisplayShader, "uProjection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(screenDisplayShader, "uView"), 1, GL_FALSE, glm::value_ptr(view));
    glUniform3fv(glGetUniformLocation(screenDisplayShader, "uViewPos"), 1, glm::value_ptr(currentCameraPos));
    glUniform3fv(glGetUniformLocation(screenDisplayShader, "uLightPos"), 1, glm::value_ptr(sunLightPos));
    glUniform3fv(glGetUniformLocation(screenDisplayShader, "uLightColor"), 1, glm::value_ptr(sunLightColor));

    float sw = 0.18f;
    float sh = 0.22f;
    float screenVertices[] = {
        -sw,  sh, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
        -sw, -sh, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
         sw, -sh, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
         sw,  sh, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f
    };

    unsigned int screenIndices[] = { 0, 1, 2, 0, 2, 3 };

    unsigned int screenVAO, screenVBO, screenEBO;
    glGenVertexArrays(1, &screenVAO);
    glGenBuffers(1, &screenVBO);
    glGenBuffers(1, &screenEBO);

    glBindVertexArray(screenVAO);
    glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screenEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(screenIndices), screenIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glm::mat4 screenModelMat = glm::mat4(1.0f);
    screenModelMat = glm::translate(screenModelMat, watchPosition + glm::vec3(0.0f, 0.0f, 0.01f));

    glUniformMatrix4fv(glGetUniformLocation(screenDisplayShader, "uModel"), 1, GL_FALSE, glm::value_ptr(screenModelMat));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screenRenderTexture);
    glUniform1i(glGetUniformLocation(screenDisplayShader, "uScreenTexture"), 0);

    glDisable(GL_CULL_FACE);
    glBindVertexArray(screenVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    if (faceCullingEnabled) {
        glEnable(GL_CULL_FACE);
    }

    glDeleteVertexArrays(1, &screenVAO);
    glDeleteBuffers(1, &screenVBO);
    glDeleteBuffers(1, &screenEBO);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glUseProgram(basicShader);

    glUniform2f(glGetUniformLocation(basicShader, "uPosition"), 0.77f, 0.89f);
    glUniform2f(glGetUniformLocation(basicShader, "uScale"), 0.40f, 0.18f);
    glUniform1f(glGetUniformLocation(basicShader, "uAlpha"), 0.6f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, infoTexture);
    glUniform1i(glGetUniformLocation(basicShader, "uTexture"), 0);

    glBindVertexArray(VAOrect);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glEnable(GL_DEPTH_TEST);
}