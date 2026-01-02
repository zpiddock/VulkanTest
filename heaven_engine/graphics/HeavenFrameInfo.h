//
// Created by Admin on 01/01/2026.
//
#pragma once

#include <graphics/HvnCamera.h>

#include "HeavenVkBuffer.h"

namespace heaven_engine {

    struct FrameInfo {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        HvnCamera& camera;
        VkDescriptorSet globalDescriptorSet;
        GameObject::Map &gameObjects;
    };
}
