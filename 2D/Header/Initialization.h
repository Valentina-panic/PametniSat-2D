#ifndef INITIALIZATION_H
#define INITIALIZATION_H

void formVAO(float* vertices, size_t verticesSize, unsigned int* indices, size_t indicesSize, unsigned int& VAO);
void formBackgroundVAO();
void formLineVAO();
void formEkgVAO();
void preprocessTexture(unsigned& texture, const char* filepath);
void loadTextures();
void initializeTime();

#endif