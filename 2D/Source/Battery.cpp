#include "../Header/CommonRendering.h"
#include "../Header/Battery.h"

void updateBattery(float currentTime) {
    if (currentTime - lastBatteryUpdate >= 10.0f) {
        batteryLevel -= 1.0f;

        if (batteryLevel < 0.0f) {
            batteryLevel = 0.0f;
        }

        lastBatteryUpdate = currentTime;
    }
}

void drawBatteryScreen() {
    drawArrow(leftArrowX, arrowY, arrowSize, true);
    drawInfoPanel();

    int batteryPercent = (int)batteryLevel;

    float digitSize = 0.10f;
    float spacing = digitSize * 1.2f;
    float percentY = 0.35f;

    if (batteryPercent >= 100) {
        drawDigit(1, -spacing * 1.8f, percentY, digitSize);
        drawDigit(0, -spacing * 0.6f, percentY, digitSize);
        drawDigit(0, spacing * 0.6f, percentY, digitSize);

        glUseProgram(basicShader);
        glBindVertexArray(VAOrect);
        glUniform2f(glGetUniformLocation(basicShader, "uPosition"), spacing * 1.8f, percentY);
        glUniform2f(glGetUniformLocation(basicShader, "uScale"), digitSize * 0.8f, digitSize * 1.2f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, percentTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    else if (batteryPercent >= 10) {
        int d1 = batteryPercent / 10;
        int d2 = batteryPercent % 10;

        drawDigit(d1, -spacing * 1.2f, percentY, digitSize);
        drawDigit(d2, 0.0f, percentY, digitSize);

        glUseProgram(basicShader);
        glBindVertexArray(VAOrect);
        glUniform2f(glGetUniformLocation(basicShader, "uPosition"), spacing * 1.2f, percentY);
        glUniform2f(glGetUniformLocation(basicShader, "uScale"), digitSize * 0.8f, digitSize * 1.2f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, percentTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    else {
        drawDigit(batteryPercent, -spacing * 0.6f, percentY, digitSize);

        glUseProgram(basicShader);
        glBindVertexArray(VAOrect);
        glUniform2f(glGetUniformLocation(basicShader, "uPosition"), spacing * 0.6f, percentY);
        glUniform2f(glGetUniformLocation(basicShader, "uScale"), digitSize * 0.8f, digitSize * 1.2f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, percentTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    float r, g, b;
    if (batteryLevel > 20.0f) {
        r = 0.0f; g = 1.0f; b = 0.0f;
    }
    else if (batteryLevel > 10.0f) {
        r = 1.0f; g = 1.0f; b = 0.0f;
    }
    else {
        r = 1.0f; g = 0.0f; b = 0.0f;
    }

    float batteryInnerLeft = -0.385f;
    float batteryInnerRight = 0.345f;
    float batteryInnerWidth = batteryInnerRight - batteryInnerLeft;
    float batteryInnerHeight = 0.26f;
    float batteryY = 0.0f;

    float fillWidth = (batteryLevel / 100.0f) * batteryInnerWidth;

    float fillRight = batteryInnerRight;
    float fillLeft = fillRight - fillWidth;

    glUseProgram(lineShader);
    glUniform3f(glGetUniformLocation(lineShader, "uColor"), r, g, b);

    float fillVertices[] = {
        fillLeft,  batteryY + batteryInnerHeight / 2,
        fillLeft,  batteryY - batteryInnerHeight / 2,
        fillRight, batteryY - batteryInnerHeight / 2,
        fillRight, batteryY + batteryInnerHeight / 2
    };

    unsigned int fillIndices[] = {
        0, 1, 2,
        0, 2, 3
    };

    unsigned int fillVAO, fillVBO, fillEBO;
    glGenVertexArrays(1, &fillVAO);
    glGenBuffers(1, &fillVBO);
    glGenBuffers(1, &fillEBO);

    glBindVertexArray(fillVAO);
    glBindBuffer(GL_ARRAY_BUFFER, fillVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fillVertices), fillVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fillEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(fillIndices), fillIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &fillVAO);
    glDeleteBuffers(1, &fillVBO);
    glDeleteBuffers(1, &fillEBO);

    float batteryX = 0.0f;
    float batteryWidth = 0.6f;
    float batteryHeight = 0.4f;

    glUseProgram(basicShader);
    glBindVertexArray(VAOrect);

    glUniform2f(glGetUniformLocation(basicShader, "uPosition"), batteryX, batteryY);
    glUniform2f(glGetUniformLocation(basicShader, "uScale"), batteryWidth, batteryHeight);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, batteryTexture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}