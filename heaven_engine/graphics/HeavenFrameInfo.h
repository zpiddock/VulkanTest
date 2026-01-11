//
// Created by Admin on 01/01/2026.
//
#pragma once

#include <graphics/HvnCamera.h>
#include <GameObject.h>

namespace heaven_engine {

    #define MAX_LIGHTS 10

    struct FrameInfo {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        HvnCamera& camera;
        VkDescriptorSet globalDescriptorSet;
        GameObject::Map &gameObjects;
    };

    struct PointLight {

        glm::vec4 position{};
        glm::vec4 colour{};
    };

    struct GlobalUbo {
        glm::mat4 projection{1.f};
        glm::mat4 view{1.f};
        glm::vec4 ambientLightColour{1.f, 1.f, 1.f, .02f};
        PointLight pointLights[MAX_LIGHTS];
        int numLights;
    };
}
