#version 330 core
out vec4 FragColor;

in vec3 chNormal;  
in vec3 chFragPos;  
in vec2 chUV;
  
uniform vec3 uLightPos; 
uniform vec3 uViewPos; 
uniform vec3 uLightColor;

// ✅ NOVO - Osvetljenje ekrana sata
uniform vec3 uScreenLightPos;        // Pozicija ekrana sata
uniform vec3 uScreenLightColor;      // Boja svetlosti (plava)
uniform float uScreenLightIntensity; // Jačina (0.3)
uniform bool uUseScreenLight;        // Da li koristimo screen light

uniform sampler2D uDiffMap1;
uniform bool uHasTexture;  // Da li model ima teksturu
uniform vec3 uMaterialColor;  // Fallback boja za modele bez teksture

void main()
{    
    float ambientStrength = 0.3;  // Povećao ambient da zgrade ne budu previše tamne
    vec3 ambient = ambientStrength * uLightColor;
  	
    // ========================================================================
    // 1. SUNCE (glavni izvor svetlosti)
    // ========================================================================
    vec3 norm = normalize(chNormal);
    vec3 lightDir = normalize(uLightPos - chFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(uViewPos - chFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * uLightColor;

    // ========================================================================
    // 2. EKRAN SATA (slabi izvor svetlosti) ✅ NOVO
    // ========================================================================
    vec3 screenContribution = vec3(0.0);
    
    if (uUseScreenLight) {
        vec3 screenLightDir = normalize(uScreenLightPos - chFragPos);
        float screenDistance = length(uScreenLightPos - chFragPos);
        
        // Atenuacija - svetlost slabi sa distancom
        float attenuation = 1.0 / (1.0 + 0.5 * screenDistance + 0.1 * screenDistance * screenDistance);
        
        // Difuzno osvetljenje od ekrana
        float screenDiff = max(dot(norm, screenLightDir), 0.0);
        
        screenContribution = uScreenLightIntensity * attenuation * screenDiff * uScreenLightColor;
    }

    // ========================================================================
    // 3. KOMBINUJ SVE
    // ========================================================================
    // Izaberi boju: tekstura ili materijal
    vec3 objectColor;
    if (uHasTexture) {
        objectColor = texture(uDiffMap1, chUV).rgb;
    } else {
        objectColor = uMaterialColor;
    }

    // Saberi sunce + ekran sata
    vec3 finalLighting = ambient + diffuse + specular + screenContribution;

    FragColor = vec4(objectColor * finalLighting, 1.0);
}
