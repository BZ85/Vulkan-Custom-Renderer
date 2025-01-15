#version 450

layout(location = 0) in vec3 fragCol;
layout(location = 1) in vec2 fragTex;
layout(location = 2) in vec3 viewPosition;
layout(location = 3) in vec3 viewNormal;
layout(location = 4) in mat4 viewMatrix;

layout(location = 0) out vec4 outColor; 	// Final output colour, must also have location

layout(set = 1, binding = 0) uniform LightingParameters {
vec4 La;
vec4 Ld;
vec4 Ls;
vec4 ka;
vec4 kd;
vec4 ks;
vec4 lightDirection;
float alpha;
} lightingParameters;

layout(set = 2, binding = 0) uniform sampler2D textureSampler;

void main() {
	outColor = vec4(fragCol, 1.0);

	//outColor = texture(textureSampler, fragTex);

vec3 viewLightDirection = (viewMatrix *  lightingParameters.lightDirection).xyz;

vec3 eyedir = normalize(vec3(0, 0, 0) - viewPosition);

vec3 reflectDir = -reflect(viewLightDirection, viewNormal);

float d = max(dot(viewLightDirection, viewNormal), 0.0f);
float s = max(dot(reflectDir, eyedir), 0.0f);

vec4 shadingValue = lightingParameters.ka * lightingParameters.La + d * lightingParameters.kd * lightingParameters.Ld + pow(s, lightingParameters.alpha) * lightingParameters.ks * lightingParameters.Ls;

outColor =  shadingValue * texture(textureSampler, fragTex);

//outColor = lightingParameters.lightDirection;
//outColor = vec4(viewNormal, 1.0f);
}