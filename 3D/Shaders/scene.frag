#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 uLightPos;
uniform vec3 uLightColor;
uniform vec3 uViewPos;
uniform vec3 uObjectColor;
uniform sampler2D uTexture;
uniform bool uUseTexture;

// Additional light for watch screen
uniform vec3 uScreenLightPos;
uniform vec3 uScreenLightColor;
uniform float uScreenLightIntensity;
uniform bool uUseScreenLight;

void main()
{
    // Ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * uLightColor;

    // Diffuse - sun light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(uLightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;

    // Diffuse - screen light (if enabled)
    vec3 screenDiffuse = vec3(0.0);
    if (uUseScreenLight) {
        vec3 screenLightDir = normalize(uScreenLightPos - FragPos);
        float screenDiff = max(dot(norm, screenLightDir), 0.0);
        float distance = length(uScreenLightPos - FragPos);
        float attenuation = uScreenLightIntensity / (1.0 + 0.5 * distance + 0.1 * distance * distance);
        screenDiffuse = screenDiff * uScreenLightColor * attenuation;
    }

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(uViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * uLightColor;

    vec3 objectColor = uObjectColor;
    if (uUseTexture) {
        objectColor = texture(uTexture, TexCoord).rgb;
    }

    vec3 result = (ambient + diffuse + screenDiffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
