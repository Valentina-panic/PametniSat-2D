#ifndef EKG_H
#define EKG_H

void updateBPM(float currentTime, float deltaTime);
void drawEKGBox();
void drawEKGGraph(float deltaTime);
void drawBPMNumber();
void drawWarningOverlay();
void drawHeartbeatScreen(float deltaTime);

#endif