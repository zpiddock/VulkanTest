//
// Created by Admin on 27/12/2025.
//

#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <memory>
#include <glm/glm.hpp>
#include <graphics/HeavenVkDevice.h>
#include <vulkan/vulkan_core.h>

#include <utils/utils.h>

namespace heaven_engine {

    class BasicModel {

    public:

        struct Vertex {
            glm::vec3 position{};
            glm::vec3 colour{};
            glm::vec3 normal{};
            glm::vec2 uv{};

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

            bool operator==(const Vertex& other) const {
                return position == other.position
                && normal == other.normal
                && uv == other.uv
                && colour == other.colour;
            }
        };

        struct ModelData {
            std::vector<Vertex> vertices{};
            std::vector<uint32_t> indices{};

            auto loadFromFile(const std::string& filepath) -> void;
        };

        BasicModel(HeavenVkDevice& device, const ModelData& modelData);
        ~BasicModel();

        BasicModel(const BasicModel&) = delete;
        BasicModel& operator=(const BasicModel&) = delete;

        auto bind(VkCommandBuffer commandBuffer) -> void;
        auto begin(VkCommandBuffer commandBuffer) -> void;

        static auto createFromFile(HeavenVkDevice& device, const std::string& filepath) -> std::unique_ptr<BasicModel>;

    private:
        auto createVertexBuffer(const std::vector<Vertex>& vertices) -> void;
        auto createIndexBuffer(const std::vector<uint32_t>& indices) -> void;

        HeavenVkDevice& device;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;

        bool hasIndexBuffer = false;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;
        uint32_t indexCount;
    };
} // vulkangame