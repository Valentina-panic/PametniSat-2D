#version 330 core

in vec2 fragCoord;
out vec4 outCol;

uniform vec3 colorTop;
uniform vec3 colorBottom;

void main()
{
    vec3 color = mix(colorBottom, colorTop, fragCoord.y);
    outCol = vec4(color, 1.0);
}