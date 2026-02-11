#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D uScreenTexture;
uniform vec3 uViewPos;
uniform vec3 uLightPos;
uniform vec3 uLightColor;

void main()
{
    // Get screen content
    vec3 screenColor = texture(uScreenTexture, TexCoord).rgb;
    
    // Add subtle lighting to make it look more realistic
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(uLightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0) * 0.2; // Subtle diffuse
    
    vec3 ambient = 0.8 * screenColor; // Screen is self-illuminating
    vec3 diffuse = diff * uLightColor * screenColor;
    
    vec3 result = ambient + diffuse;
    
    // Screen emits light
    FragColor = vec4(result, 1.0);
}
