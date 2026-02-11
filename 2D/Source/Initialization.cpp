#include "../Header/Globals.h"
#include "../Header/Initialization.h"
#include <ctime>
#include <cstdio>
#include "../Header/Util.h"

void formVAO(float* vertices, size_t verticesSize, unsigned int* indices, size_t indicesSize, unsigned int& VAO) {
    unsigned int VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void formBackgroundVAO() {
    unsigned int VBO, EBO;

    glGenVertexArrays(1, &VAObackground);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAObackground);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundVertices), backgroundVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(backgroundIndices), backgroundIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void formLineVAO() {
    glGenVertexArrays(1, &VAOline);
    glBindVertexArray(VAOline);
    glBindVertexArray(0);
}

void formEkgVAO() {
    glGenVertexArrays(1, &VAOekg);
    glBindVertexArray(VAOekg);
    glBindVertexArray(0);
}

void preprocessTexture(unsigned& texture, const char* filepath) {
    texture = loadImageToTexture(filepath);
    glBindTexture(GL_TEXTURE_2D, texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void loadTextures() {
    preprocessTexture(digitTextures[0], "Resources/0.png");
    preprocessTexture(digitTextures[1], "Resources/1.png");
    preprocessTexture(digitTextures[2], "Resources/2.png");
    preprocessTexture(digitTextures[3], "Resources/3.png");
    preprocessTexture(digitTextures[4], "Resources/4.png");
    preprocessTexture(digitTextures[5], "Resources/5.png");
    preprocessTexture(digitTextures[6], "Resources/6.png");
    preprocessTexture(digitTextures[7], "Resources/7.png");
    preprocessTexture(digitTextures[8], "Resources/8.png");
    preprocessTexture(digitTextures[9], "Resources/9.png");
    preprocessTexture(colonTexture, "Resources/colon.png");
    preprocessTexture(arrowTexture, "Resources/arrow.png");
    preprocessTexture(infoTexture, "Resources/info.png");
    preprocessTexture(ekgTexture, "Resources/ekg.png");
    preprocessTexture(letterBTexture, "Resources/B.png");
    preprocessTexture(letterPTexture, "Resources/P.png");
    preprocessTexture(letterMTexture, "Resources/M.png");
    preprocessTexture(borderTexture, "Resources/border.png");
    preprocessTexture(redTexture, "Resources/red.png");
    preprocessTexture(batteryTexture, "Resources/battery.png");
    preprocessTexture(percentTexture, "Resources/percent.png");

    const char* letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < 26; i++) {
        char filename[64];
        sprintf_s(filename, "Resources/%c.png", letters[i]);
        preprocessTexture(letterTextures[i], filename);
    }
}

void initializeTime() {
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);

    hours = ltm.tm_hour;
    minutes = ltm.tm_min;
    seconds = ltm.tm_sec;
}