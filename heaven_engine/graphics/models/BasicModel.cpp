//
// Created by Admin on 27/12/2025.
//

#include "BasicModel.h"

#include <graphics/VulkanDevice.h>
#include <vector>

#include <cassert>

namespace heaven_engine {

    BasicModel::BasicModel(VulkanDevice &device, const std::vector<Vertex> &vertices) : device{device} {

        createVertexBuffer(vertices);
    }

    BasicModel::~BasicModel() {

        ::vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
        ::vkFreeMemory(device.device(), vertexBufferMemory, nullptr);
    }

    std::vector<VkVertexInputBindingDescription> BasicModel::Vertex::getBindingDescriptions() {

        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription> BasicModel::Vertex::getAttributeDescriptions() {

        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, colour);

        return attributeDescriptions;
    }


    auto BasicModel::begin(VkCommandBuffer commandBuffer) -> void {

        ::vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);

    }

    auto BasicModel::bind(VkCommandBuffer commandBuffer) -> void {

        VkBuffer buffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        ::vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }

    auto BasicModel::createVertexBuffer(const std::vector<Vertex> &vertices) -> void {

        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "Vertex count must be above 3");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

        device.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            vertexBuffer,
            vertexBufferMemory);

        void* data;
        ::vkMapMemory(device.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), bufferSize);
        ::vkUnmapMemory(device.device(), vertexBufferMemory);
    }
} // heaven_engine