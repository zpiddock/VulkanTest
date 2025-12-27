//
// Created by Admin on 23/12/2025.
//

#include "GameProgram.h"

#include <exception>
#include <iostream>
#include <array>

namespace vulkangame {

    GameProgram::GameProgram() {

        loadModels();
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    GameProgram::~GameProgram() {

        ::vkDestroyPipelineLayout(vulkanDevice.device(), pipelineLayout, nullptr);
    }

    auto GameProgram::loadModels() -> void {

        std::vector<BasicModel::Vertex> vertices = {
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        };

        model = std::make_unique<BasicModel>(vulkanDevice, vertices);
    }

    auto GameProgram::createPipelineLayout() -> void {

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
        pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutCreateInfo.setLayoutCount = 0;
        pipelineLayoutCreateInfo.pSetLayouts = nullptr;
        pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
        pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

        if (::vkCreatePipelineLayout(
            vulkanDevice.device(),
            &pipelineLayoutCreateInfo,
            nullptr,
            &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline layout!");
            }
    }

    auto GameProgram::createPipeline() -> void {

        auto pipelineConfig = ShaderPipeline::defaultPipelineConfigInfo(vulkanSwapChain.width(), vulkanSwapChain.height());
        pipelineConfig.renderPass = vulkanSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;

        shaderPipeline = std::make_unique<ShaderPipeline>(
            vulkanDevice,
            "simple_shader",
            pipelineConfig);
    }

    auto GameProgram::createCommandBuffers() -> void {

        commandBuffers.resize(vulkanSwapChain.imageCount());
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = vulkanDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (::vkAllocateCommandBuffers(
            vulkanDevice.device(),
            &allocInfo,
            commandBuffers.data()
            ) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate command buffers!");
        }

        for (int i = 0; i < commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (::vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("Failed to begin command buffer!");
            }

            VkRenderPassBeginInfo renderPassBeginInfo{};
            renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassBeginInfo.renderPass = vulkanSwapChain.getRenderPass();
            renderPassBeginInfo.framebuffer = vulkanSwapChain.getFrameBuffer(i);

            renderPassBeginInfo.renderArea.offset = { 0, 0 };
            renderPassBeginInfo.renderArea.extent = vulkanSwapChain.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
            clearValues[1].depthStencil = { 1.0f, 0 };

            renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassBeginInfo.pClearValues = clearValues.data();

            ::vkCmdBeginRenderPass(commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

            shaderPipeline->bind(commandBuffers[i]);
            model->bind(commandBuffers[i]);
            model->begin(commandBuffers[i]);

            ::vkCmdEndRenderPass(commandBuffers[i]);
            if (::vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("Failed to record command buffer!");
            }
        }
    }

    auto GameProgram::drawFrame() -> void {

        uint32_t imageIndex;
        auto result = vulkanSwapChain.acquireNextImage(&imageIndex);

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Failed to acquire swap chain image!");
        }

        result = vulkanSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to submit command buffer!");
        }
    }

    auto GameProgram::run() -> void {

        //Gameloop
        while(!gameWindow.shouldClose()) {
            ::glfwPollEvents();
            drawFrame();
        }

        ::vkDeviceWaitIdle(vulkanDevice.device());
    }
} // vulkangame