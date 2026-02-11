#include <cstdlib>
#include "../Header/CommonRendering.h"
#include "../Header/Ekg.h"

void updateBPM(float currentTime, float deltaTime) {
    if (isRunning) {
        float targetBPM = baseBPM + (runningTime * bpmIncreaseRate);

        if (currentBPM < targetBPM) {
            currentBPM += deltaTime * 15.0f;

            if (currentBPM > targetBPM) {
                currentBPM = targetBPM;
            }
        }

        runningTime += deltaTime;

        if (currentBPM >= 200.0f) {  
            warningActive = true;
        }

        if (currentBPM > 220.0f) {
            currentBPM = 220.0f;
        }
    }
    else {
        runningTime = 0.0f;

        if (currentBPM > 80.0f) {
            currentBPM -= deltaTime * 10.0f;

            if (currentBPM < 80.0f) {
                currentBPM = 80.0f;
            }
        }
        else if (currentBPM >= 60.0f && currentBPM <= 80.0f) {
            if (currentTime - lastBPMUpdate >= 2.0f) {
                currentBPM = 60.0f + (rand() % 21);
                lastBPMUpdate = currentTime;
            }
        }

        if (currentBPM < 200.0f) { 
            warningActive = false;
        }
    }
}

void drawEKGBox() {
    float x1 = -0.38f;
    float x2 = 0.38f;
    float y1 = -0.46f;
    float y2 = 0.26f;

    glUseProgram(lineShader);
    glUniform3f(glGetUniformLocation(lineShader, "uColor"), 0.0f, 1.0f, 0.0f);

    glBindVertexArray(VAOline);

    float lines[] = {
        x1, y2,   x2, y2,
        x1, y1,   x2, y1,
        x1, y1,   x1, y2,
        x2, y1,   x2, y2
    };

    GLuint VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glLineWidth(3.0f);

    glDrawArrays(GL_LINES, 0, 8);

    glDeleteBuffers(1, &VBO);
}

void drawEKGGraph(float deltaTime) {
    ekgOffsetX += ekgSpeed * deltaTime;

    if (ekgOffsetX >= 1.0f) {
        ekgOffsetX -= 1.0f;
    }

    glUseProgram(basicShader);

    float baseWidth = 0.74f;
    float minWidth = 0.2f;

    float bpmFactor = (currentBPM - 60.0f) / 160.0f;
    if (bpmFactor < 0.0f) bpmFactor = 0.0f;
    if (bpmFactor > 1.0f) bpmFactor = 1.0f;
    float graphWidth = baseWidth - (baseWidth - minWidth) * bpmFactor;

    float graphHeight = 0.34f;
    float graphX = 0.0f;
    float graphY = -0.1f;

    glUniform2f(glGetUniformLocation(basicShader, "uPosition"), graphX, graphY);
    glUniform2f(glGetUniformLocation(basicShader, "uScale"), graphWidth, graphHeight);
    glUniform1f(glGetUniformLocation(basicShader, "uAlpha"), 1.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ekgTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    float baseRepeat = 3.0f;
    float maxRepeat = 15.0f;
    float repeatCount = baseRepeat + (maxRepeat - baseRepeat) * bpmFactor;

    float vertexScale = 0.75f / graphWidth;
    float vx = 0.5f * vertexScale;
    float vy = 0.5f * vertexScale;

    float animatedVertices[] = {
        -vx,  vy,    ekgOffsetX, 1.0f,
        -vx, -vy,    ekgOffsetX, 0.0f,
         vx, -vy,    ekgOffsetX + repeatCount, 0.0f,
         vx,  vy,    ekgOffsetX + repeatCount, 1.0f
    };

    glBindVertexArray(VAOekg);
    GLuint VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(animatedVertices), animatedVertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void drawBPMNumber() {
    float digitSize = 0.12f;
    float spacing = digitSize * 1.2f;
    float y = 0.45f;

    int displayBPM = (int)currentBPM;

    int bpm1, bpm2, bpm3;
    float startX;

    if (displayBPM >= 100) {
        bpm1 = displayBPM / 100;
        bpm2 = (displayBPM / 10) % 10;
        bpm3 = displayBPM % 10;

        startX = -spacing * 1.8f;

        drawDigit(bpm1, startX, y, digitSize);
        drawDigit(bpm2, startX + spacing, y, digitSize);
        drawDigit(bpm3, startX + spacing * 2, y, digitSize);

        float letterSize = 0.06f;
        float letterSpacing = letterSize * 1.3f;
        float letterStartX = startX + spacing * 3.4f;
        float letterY = y + 0.02f;

        glUseProgram(basicShader);
        glBindVertexArray(VAOrect);

        glUniform2f(glGetUniformLocation(basicShader, "uPosition"), letterStartX, letterY);
        glUniform2f(glGetUniformLocation(basicShader, "uScale"), letterSize, letterSize * 1.5f);
        glUniform1f(glGetUniformLocation(basicShader, "uAlpha"), 1.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, letterBTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glUniform2f(glGetUniformLocation(basicShader, "uPosition"), letterStartX + letterSpacing, letterY);
        glUniform1f(glGetUniformLocation(basicShader, "uAlpha"), 1.0f);
        glBindTexture(GL_TEXTURE_2D, letterPTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glUniform2f(glGetUniformLocation(basicShader, "uPosition"), letterStartX + letterSpacing * 2, letterY);
        glUniform1f(glGetUniformLocation(basicShader, "uAlpha"), 1.0f);
        glBindTexture(GL_TEXTURE_2D, letterMTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    else {
        bpm1 = displayBPM / 10;
        bpm2 = displayBPM % 10;

        startX = -spacing * 1.2f;

        drawDigit(bpm1, startX, y, digitSize);
        drawDigit(bpm2, startX + spacing, y, digitSize);

        float letterSize = 0.06f;
        float letterSpacing = letterSize * 1.3f;
        float letterStartX = startX + spacing * 2.4f;
        float letterY = y + 0.02f;

        glUseProgram(basicShader);
        glBindVertexArray(VAOrect);

        glUniform2f(glGetUniformLocation(basicShader, "uPosition"), letterStartX, letterY);
        glUniform2f(glGetUniformLocation(basicShader, "uScale"), letterSize, letterSize * 1.5f);
        glUniform1f(glGetUniformLocation(basicShader, "uAlpha"), 1.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, letterBTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glUniform2f(glGetUniformLocation(basicShader, "uPosition"), letterStartX + letterSpacing, letterY);
        glUniform1f(glGetUniformLocation(basicShader, "uAlpha"), 1.0f);
        glBindTexture(GL_TEXTURE_2D, letterPTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glUniform2f(glGetUniformLocation(basicShader, "uPosition"), letterStartX + letterSpacing * 2, letterY);
        glUniform1f(glGetUniformLocation(basicShader, "uAlpha"), 1.0f);
        glBindTexture(GL_TEXTURE_2D, letterMTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}

void drawWarningOverlay() {
    glUseProgram(basicShader);
    glBindVertexArray(VAOrect);

    glUniform2f(glGetUniformLocation(basicShader, "uPosition"), 0.0f, 0.0f);
    glUniform2f(glGetUniformLocation(basicShader, "uScale"), 2.0f, 2.0f);
    glUniform1f(glGetUniformLocation(basicShader, "uAlpha"), 1.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, redTexture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    int displayBPM = (int)currentBPM;

    int bpm1 = displayBPM / 100;
    int bpm2 = (displayBPM / 10) % 10;
    int bpm3 = displayBPM % 10;

    float digitSize = 0.20f;
    float spacing = digitSize * 1.3f;
    float startX = -spacing * 1.5f;
    float y = 0.0f; 

    drawDigit(bpm1, startX, y, digitSize);
    drawDigit(bpm2, startX + spacing, y, digitSize);
    drawDigit(bpm3, startX + spacing * 2, y, digitSize);

    float letterSize = 0.08f;
    float letterSpacing = letterSize * 1.3f;
    float letterStartX = startX + spacing * 3.2f; 
    float letterY = y; 

    glUseProgram(basicShader);
    glBindVertexArray(VAOrect);

    glUniform2f(glGetUniformLocation(basicShader, "uPosition"), letterStartX, letterY);
    glUniform2f(glGetUniformLocation(basicShader, "uScale"), letterSize, letterSize * 1.5f);
    glUniform1f(glGetUniformLocation(basicShader, "uAlpha"), 1.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, letterBTexture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glUniform2f(glGetUniformLocation(basicShader, "uPosition"), letterStartX + letterSpacing, letterY);
    glUniform1f(glGetUniformLocation(basicShader, "uAlpha"), 1.0f);
    glBindTexture(GL_TEXTURE_2D, letterPTexture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glUniform2f(glGetUniformLocation(basicShader, "uPosition"), letterStartX + letterSpacing * 2, letterY);
    glUniform1f(glGetUniformLocation(basicShader, "uAlpha"), 1.0f);
    glBindTexture(GL_TEXTURE_2D, letterMTexture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    float msgSize = 0.10f;
    float msgY1 = -0.35f;  
    float msgY2 = -0.53f;

    drawText("ZAUSTAVI SE", -0.55f, msgY1, msgSize);
    drawText("I ODMORI SE", -0.55f, msgY2, msgSize);

    drawInfoPanel(1.0f);
    glBindVertexArray(0);
}

void drawHeartbeatScreen(float deltaTime) {
    drawArrow(leftArrowX, arrowY, arrowSize, true);
    drawArrow(rightArrowX, arrowY, arrowSize, false);

    drawBPMNumber();
    drawEKGBox();
    drawEKGGraph(deltaTime);
    drawInfoPanel();

    if (warningActive) {
        drawWarningOverlay();
    }
}