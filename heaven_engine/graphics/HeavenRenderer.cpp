//
// Created by Admin on 23/12/2025.
//

#include "HeavenRenderer.h"

#include <array>
#include <format>

#include <vulkan/vk_enum_string_helper.h>

namespace heaven_engine {

    HeavenRenderer::HeavenRenderer(HeavenWindow& window, HeavenVkDevice& device) : gameWindow(window), vulkanDevice(device) {

        recreateSwapChain();
        createCommandBuffers();
    }

    HeavenRenderer::~HeavenRenderer() {

        freeCommandBuffers();
    }



    auto HeavenRenderer::recreateSwapChain() -> void {

        auto extent = gameWindow.getExtent();
        while (extent.width == 0 || extent.height == 0) {

            extent = gameWindow.getExtent();
            ::glfwWaitEvents();
        }

        ::vkDeviceWaitIdle(vulkanDevice.device());

        if (vulkanSwapChain == nullptr) {
            vulkanSwapChain = std::make_unique<HeavenVkSwapChain>(vulkanDevice, extent);
        } else {
            std::shared_ptr oldSwapChain = std::move(vulkanSwapChain);
            vulkanSwapChain = std::make_unique<HeavenVkSwapChain>(vulkanDevice, extent, oldSwapChain);

            if (!oldSwapChain->compareSwapFormats(*vulkanSwapChain)) {
                throw std::runtime_error("Swap chain image or depth format has changed!");
            }
        }
    }

    auto HeavenRenderer::createCommandBuffers() -> void {

        commandBuffers.resize(HeavenVkSwapChain::MAX_FRAMES_IN_FLIGHT);

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
    }

    auto HeavenRenderer::freeCommandBuffers() -> void {

        ::vkFreeCommandBuffers(vulkanDevice.device(), vulkanDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
        commandBuffers.clear();
    }

    auto HeavenRenderer::beginFrame() -> VkCommandBuffer {
        assert(!isFrameBegun && "Can't call beginFrame while already in progress");

        auto result = vulkanSwapChain->acquireNextImage(&currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return nullptr;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Failed to acquire swap chain image!");
        }

        isFrameBegun = true;

        auto commandBuffer = getCurrentCommandBuffer();

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (::vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("Failed to begin command buffer!");
        }
        return commandBuffer;
    }

    auto HeavenRenderer::endFrame() -> void {

        assert(isFrameBegun && "Can't call endFrame while not in progress");
        auto commandBuffer = getCurrentCommandBuffer();

        if (::vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("Failed to record command buffer!");
        }

        auto result = vulkanSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || gameWindow.wasWindowResized()) {
            gameWindow.resetWindowResizedFlag();
            recreateSwapChain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error(std::format("Failed to submit command buffer!: {}", ::string_VkResult(result)));
        }

        isFrameBegun = false;
        currentFrameIndex = (currentFrameIndex + 1) % HeavenVkSwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    auto HeavenRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) -> void {

        assert(isFrameBegun && "Can't call beginSwapChainRenderPass while not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on cmdBuffer from different frame");

        VkRenderPassBeginInfo renderPassBeginInfo{};
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass = vulkanSwapChain->getRenderPass();
        renderPassBeginInfo.framebuffer = vulkanSwapChain->getFrameBuffer(currentImageIndex);

        renderPassBeginInfo.renderArea.offset = { 0, 0 };
        renderPassBeginInfo.renderArea.extent = vulkanSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassBeginInfo.pClearValues = clearValues.data();

        ::vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(vulkanSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(vulkanSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, vulkanSwapChain->getSwapChainExtent()};
        ::vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        ::vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    }

    auto HeavenRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) -> void {

        assert(isFrameBegun && "Can't call endSwapChainRenderPass while not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "Can't end render pass on cmdBuffer from different frame");

        ::vkCmdEndRenderPass(commandBuffer);
    }
} // heaven_engine