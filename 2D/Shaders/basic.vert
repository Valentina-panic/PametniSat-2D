#version 330 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTex;
out vec2 chTex;

uniform vec2 uPosition;
uniform vec2 uScale;

void main()
{
    vec2 scaledPos = inPos * uScale;
    gl_Position = vec4(scaledPos.x + uPosition.x, scaledPos.y + uPosition.y, 0.0, 1.0);
    
    chTex = inTex;
}
