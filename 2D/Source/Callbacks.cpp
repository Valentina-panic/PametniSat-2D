#include "../Header/Globals.h"
#include "../Header/Callbacks.h"

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        float xposNorm = (xpos / screenWidth) * 2 - 1;
        float yposNorm = -((ypos / screenHeight) * 2 - 1);

        if (xposNorm < rightArrowX + arrowWidth / 2 && xposNorm > rightArrowX - arrowWidth / 2 &&
            yposNorm < arrowY + arrowHeight / 2 && yposNorm > arrowY - arrowHeight / 2) {

            if (currentScreen == CLOCK_SCREEN) {
                currentScreen = HEARTBEAT_SCREEN;
            }
            else if (currentScreen == HEARTBEAT_SCREEN) {
                currentScreen = BATTERY_SCREEN;
            }
        }

        if (xposNorm < leftArrowX + arrowWidth / 2 && xposNorm > leftArrowX - arrowWidth / 2 &&
            yposNorm < arrowY + arrowHeight / 2 && yposNorm > arrowY - arrowHeight / 2) {

            if (currentScreen == HEARTBEAT_SCREEN) {
                currentScreen = CLOCK_SCREEN;
            }
            else if (currentScreen == BATTERY_SCREEN) {
                currentScreen = HEARTBEAT_SCREEN;
            }
        }
    }
}