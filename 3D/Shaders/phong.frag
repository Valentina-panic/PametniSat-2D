#version 330 core

struct Light{ 
	vec3 pos; 
	vec3 kA; 
	vec3 kD; 
	vec3 kS; 
};

struct Material{ 
	vec3 kA;
	vec3 kD;
	vec3 kS;
	float shine;
};

in vec3 chNor;
in vec3 chFragPos;
in vec2 chTexCoords;

out vec4 outCol;

uniform Light uLight;
uniform Material uMaterial;
uniform vec3 uViewPos;
uniform sampler2D uTexture;

void main()
{
	// Ambient
	vec3 resA = uLight.kA * uMaterial.kA;

	// Diffuse
	vec3 normal = normalize(chNor);
	vec3 lightDirection = normalize(uLight.pos - chFragPos);
	float nD = max(dot(normal, lightDirection), 0.0);
	
	// Koristi teksturu
	vec3 texColor = texture(uTexture, chTexCoords).rgb;
	vec3 resD = uLight.kD * (nD * texColor);

	// Specular
	vec3 viewDirection = normalize(uViewPos - chFragPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float s = pow(max(dot(viewDirection, reflectionDirection), 0.0), uMaterial.shine);
	vec3 resS = uLight.kS * (s * uMaterial.kS);

	outCol = vec4(resA + resD + resS, 1.0);
}
