#version 460

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 fragColour;

layout(set = 0, binding = 0) uniform GlobalUbo {

    mat4 projectionViewMatrix;
    vec4 ambientLightColour;
    vec3 lightPosition;
    vec4 lightColour;
} ubo;

layout(push_constant) uniform Push {
    mat4 modelMatrix; // projection * view * model
    mat4 normalMatrix;
} push;


void main() {
    vec4 positionWorld = push.modelMatrix * vec4(position, 1.0);
    gl_Position = ubo.projectionViewMatrix * positionWorld;

    vec3 normalWorldSpace = normalize(mat3(push.normalMatrix) * normal);

    vec3 directionToLight = ubo.lightPosition - positionWorld.xyz;
    float attenutation = 1.0 / dot(directionToLight, directionToLight);

    vec3 lightColour = ubo.lightColour.xyz * ubo.lightColour.w * attenutation;
    vec3 ambientLight = ubo.ambientLightColour.xyz * ubo.ambientLightColour.w;
    vec3 diffuseLight = lightColour * max(dot(normalWorldSpace, normalize(directionToLight)), 0);

    fragColour = (diffuseLight + ambientLight) * colour;
}
