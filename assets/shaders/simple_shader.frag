#version 460

layout(location = 0) in vec3 inColour;
layout(location = 1) in vec3 fragPosWorld;
layout(location = 2) in vec3 fragNormalWorldSpace;

layout(location = 0) out vec4 outColour;

layout(set = 0, binding = 0) uniform GlobalUbo {

    mat4 projectionMatrix;
    mat4 viewMatrix;
    vec4 ambientLightColour;
    vec3 lightPosition;
    vec4 lightColour;
} ubo;

layout(push_constant) uniform Push {
    mat4 modelMatrix; // projection * view * model
    mat4 normalMatrix;
} push;

void main() {

    vec3 directionToLight = ubo.lightPosition - fragPosWorld;
    float attenutation = 1.0 / dot(directionToLight, directionToLight);

    vec3 lightColour = ubo.lightColour.xyz * ubo.lightColour.w * attenutation;
    vec3 ambientLight = ubo.ambientLightColour.xyz * ubo.ambientLightColour.w;
    vec3 diffuseLight = lightColour * max(dot(normalize(fragNormalWorldSpace), normalize(directionToLight)), 0);

    outColour = vec4((diffuseLight + ambientLight) * inColour, 1.0);
}