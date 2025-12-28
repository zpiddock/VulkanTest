//
// Created by Admin on 27/12/2025.
//

#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <graphics/HeavenVkDevice.h>
#include <vulkan/vulkan_core.h>

namespace heaven_engine {

    class BasicModel {

    public:

        struct Vertex {
            glm::vec2 position;
            glm::vec3 colour;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };

        BasicModel(HeavenVkDevice& device, const std::vector<Vertex>& vertices);
        ~BasicModel();

        BasicModel(const BasicModel&) = delete;
        BasicModel& operator=(const BasicModel&) = delete;

        auto bind(VkCommandBuffer commandBuffer) -> void;
        auto begin(VkCommandBuffer commandBuffer) -> void;

    private:
        auto createVertexBuffer(const std::vector<Vertex>& vertices) -> void;

        HeavenVkDevice& device;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
    };
} // vulkangame