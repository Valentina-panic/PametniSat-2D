#include "../Headers/Globals.h"
#include "../Headers/Initialization.h"
#include <ctime>
#include <cstdio>
#include "../Headers/Util.h"
#include <iostream>

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
    preprocessTexture(groundTexture, "Resources/road.png");

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

void setupGroundVAO() {
    const float GROUND_Y = -0.1f; 
    const float GROUND_WIDTH = 6.0f;
    const float GROUND_START_Z = 5.0f;
    const float GROUND_END_Z = -95.0f;
    const float TEX_REPEAT = 20.0f; 

    float groundVertices[] = {
        // FRONT LEFT (blizu kamere)
        -GROUND_WIDTH / 2, GROUND_Y, GROUND_START_Z,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,

        // FRONT RIGHT
        +GROUND_WIDTH / 2, GROUND_Y, GROUND_START_Z,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,

        // BACK RIGHT (daleko)
        +GROUND_WIDTH / 2, GROUND_Y, GROUND_END_Z,     0.0f, 1.0f, 0.0f,   1.0f, TEX_REPEAT,

        // BACK LEFT (daleko)
        -GROUND_WIDTH / 2, GROUND_Y, GROUND_END_Z,     0.0f, 1.0f, 0.0f,   0.0f, TEX_REPEAT
    };

    unsigned int groundIndices[] = {
        0, 1, 2, 
        0, 2, 3   
    };

    unsigned int VBO, EBO;
    glGenVertexArrays(1, &VAOground);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAOground);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundIndices), groundIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void setupFramebuffer() {
    glGenFramebuffers(1, &screenFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);

    glGenTextures(1, &screenRenderTexture);
    glBindTexture(GL_TEXTURE_2D, screenRenderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenRenderTexture, 0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 1024);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR: Framebuffer is not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}