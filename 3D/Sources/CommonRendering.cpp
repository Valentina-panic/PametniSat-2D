#include "../Headers/CommonRendering.h"

void drawGradientBackground() {
    glUseProgram(gradientShader);

    float colorTop[3];
    float colorBottom[3];

    switch (currentScreen) {
    case CLOCK_SCREEN:
        colorTop[0] = 0.49f; colorTop[1] = 0.32f; colorTop[2] = 0.81f;
        colorBottom[0] = 0.12f; colorBottom[1] = 0.24f; colorBottom[2] = 0.45f;
        break;
    case HEARTBEAT_SCREEN:
        colorTop[0] = 0.1f; colorTop[1] = 0.1f; colorTop[2] = 0.1f;
        colorBottom[0] = 0.18f; colorBottom[1] = 0.11f; colorBottom[2] = 0.11f;
        break;
    case BATTERY_SCREEN:
        colorTop[0] = 0.12f; colorTop[1] = 0.35f; colorTop[2] = 0.25f;
        colorBottom[0] = 0.04f; colorBottom[1] = 0.24f; colorBottom[2] = 0.17f;
        break;
    }

    glUniform3fv(glGetUniformLocation(gradientShader, "colorTop"), 1, colorTop);
    glUniform3fv(glGetUniformLocation(gradientShader, "colorBottom"), 1, colorBottom);
    glUniform1f(glGetUniformLocation(gradientShader, "time"), glfwGetTime());

    glBindVertexArray(VAObackground);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void drawDigit(int digit, float x, float y, float size) {
    if (digit < 0) digit = 0;
    if (digit > 9) digit = 9;

    glUseProgram(basicShader);

    glUniform2f(glGetUniformLocation(basicShader, "uPosition"), x, y);
    glUniform2f(glGetUniformLocation(basicShader, "uScale"), size, size * 1.5f);
    glUniform1f(glGetUniformLocation(basicShader, "uAlpha"), 1.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, digitTextures[digit]);

    glBindVertexArray(VAOrect);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void drawColon(float x, float y, float size) {
    glUseProgram(basicShader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colonTexture);

    glUniform2f(glGetUniformLocation(basicShader, "uPosition"), x, y);
    glUniform2f(glGetUniformLocation(basicShader, "uScale"), size * 1.9f, size * 1.9f);
    glUniform1f(glGetUniformLocation(basicShader, "uAlpha"), 1.0f);

    glBindVertexArray(VAOrect);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void drawArrow(float x, float y, float size, bool flipped) {
    glUseProgram(basicShader);

    glUniform2f(glGetUniformLocation(basicShader, "uPosition"), x, y);
    float scaleX = flipped ? -size : size;
    glUniform2f(glGetUniformLocation(basicShader, "uScale"), scaleX, size * 0.8f);
    glUniform1f(glGetUniformLocation(basicShader, "uAlpha"), 1.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, arrowTexture);

    glBindVertexArray(VAOrect);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void drawText(const char* text, float startX, float y, float size) {
    glUseProgram(basicShader);
    glBindVertexArray(VAOrect);

    float spacing = size * 1.1f;
    float x = startX;

    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];

        if (c == ' ') {
            x += spacing * 0.6f;
            continue;
        }

        if (c >= 'A' && c <= 'Z') {
            int index = c - 'A';

            glUniform2f(glGetUniformLocation(basicShader, "uPosition"), x, y);
            glUniform2f(glGetUniformLocation(basicShader, "uScale"), size * 0.8f, size * 1.2f);
            glUniform1f(glGetUniformLocation(basicShader, "uAlpha"), 1.0f);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, letterTextures[index]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        x += spacing;
    }

    glBindVertexArray(0);
}

void drawInfoPanel(float alpha) {
    glUseProgram(basicShader);
    glBindVertexArray(VAOrect);
    glUniform2f(glGetUniformLocation(basicShader, "uPosition"), 0.77f, 0.89f);
    glUniform2f(glGetUniformLocation(basicShader, "uScale"), 0.40f, 0.18f);
    glUniform1f(glGetUniformLocation(basicShader, "uAlpha"), alpha);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, infoTexture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}