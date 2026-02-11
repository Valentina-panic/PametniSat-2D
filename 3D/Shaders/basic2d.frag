#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D uTexture;
uniform float uAlpha;

void main()
{
    vec4 texColor = texture(uTexture, TexCoord);
    FragColor = vec4(texColor.rgb, texColor.a * uAlpha);
}
