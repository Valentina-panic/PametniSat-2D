#include "../Headers/Globals.h"
#include "../Headers/Callbacks.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (!watchFocused) return;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
            (float)screenWidth / (float)screenHeight,
            0.1f, 100.0f);

        glm::vec3 currentCameraPos = focusedCameraPos;
        glm::mat4 view = glm::lookAt(currentCameraPos, watchPosition, cameraUp);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, watchPosition);

        glm::mat4 mvp = projection * view * model;

        float sw = 0.18f;
        float sh = 0.22f;

        glm::vec4 corners[4] = {
            glm::vec4(-sw,  sh, 0.0f, 1.0f),  // top-left
            glm::vec4(sw,  sh, 0.0f, 1.0f),  // top-right
            glm::vec4(-sw, -sh, 0.0f, 1.0f),  // bottom-left
            glm::vec4(sw, -sh, 0.0f, 1.0f)   // bottom-right
        };


        glm::vec2 screenCorners[4];
        for (int i = 0; i < 4; i++) {
            glm::vec4 clipSpace = mvp * corners[i];
            glm::vec3 ndc = glm::vec3(clipSpace) / clipSpace.w;

            screenCorners[i].x = (ndc.x + 1.0f) * 0.5f * screenWidth;
            screenCorners[i].y = (1.0f - ndc.y) * 0.5f * screenHeight;
        }


        float minX = screenCorners[0].x;
        float maxX = screenCorners[0].x;
        float minY = screenCorners[0].y;
        float maxY = screenCorners[0].y;

        for (int i = 1; i < 4; i++) {
            if (screenCorners[i].x < minX) minX = screenCorners[i].x;
            if (screenCorners[i].x > maxX) maxX = screenCorners[i].x;
            if (screenCorners[i].y < minY) minY = screenCorners[i].y;
            if (screenCorners[i].y > maxY) maxY = screenCorners[i].y;
        }

        float normalizedX = ((xpos - minX) / (maxX - minX)) * 2.0f - 1.0f;
        float normalizedY = -((ypos - minY) / (maxY - minY)) * 2.0f + 1.0f;

        float actualArrowWidth = arrowSize * 1.4f;
        float actualArrowHeight = arrowSize * 0.8f * 1.4f;

        if (normalizedX >= (rightArrowX - actualArrowWidth / 2.0f) &&
            normalizedX <= (rightArrowX + actualArrowWidth / 2.0f) &&
            normalizedY >= (arrowY - actualArrowHeight / 2.0f) &&
            normalizedY <= (arrowY + actualArrowHeight / 2.0f)) {


            if (currentScreen == CLOCK_SCREEN) {
                currentScreen = HEARTBEAT_SCREEN;
            }
            else if (currentScreen == HEARTBEAT_SCREEN) {
                currentScreen = BATTERY_SCREEN;
            }
        }

        if (normalizedX >= (leftArrowX - actualArrowWidth / 2.0f) &&
            normalizedX <= (leftArrowX + actualArrowWidth / 2.0f) &&
            normalizedY >= (arrowY - actualArrowHeight / 2.0f) &&
            normalizedY <= (arrowY + actualArrowHeight / 2.0f)) {

            if (currentScreen == HEARTBEAT_SCREEN) {
                currentScreen = CLOCK_SCREEN;
            }
            else if (currentScreen == BATTERY_SCREEN) {
                currentScreen = HEARTBEAT_SCREEN;
            }
        }
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (watchFocused) return;

    if (firstMouse) {
        lastMouseY = ypos;
        firstMouse = false;
    }

    double yoffset = lastMouseY - ypos;
    lastMouseY = ypos;

    float sensitivity = 0.1f;
    yoffset *= sensitivity;

    cameraPitch += yoffset;

    if (cameraPitch > 45.0f)
        cameraPitch = 45.0f;
    if (cameraPitch < -45.0f)
        cameraPitch = -45.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    front.y = sin(glm::radians(cameraPitch));
    front.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraFront = glm::normalize(front);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        watchFocused = !watchFocused;

        if (watchFocused) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursor(window, cursor);
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }

    if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
        depthTestEnabled = !depthTestEnabled;
        if (depthTestEnabled)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);
    }

    if (key == GLFW_KEY_X && action == GLFW_PRESS) {
        faceCullingEnabled = !faceCullingEnabled;
        if (faceCullingEnabled)
            glEnable(GL_CULL_FACE);
        else
            glDisable(GL_CULL_FACE);
    }
}