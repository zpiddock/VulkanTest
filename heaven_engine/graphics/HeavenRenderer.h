//
// Created by Admin on 23/12/2025.
//

#pragma once
#include <bits/unique_ptr.h>
#include <graphics/models/BasicModel.h>

#include "HeavenWindow.h"
#include "HeavenVkSwapChain.h"

namespace heaven_engine {

    class HeavenRenderer {

    public:

        HeavenRenderer(HeavenWindow& window, HeavenVkDevice& device);
        ~HeavenRenderer();

        HeavenRenderer(const HeavenRenderer&) = delete;
        HeavenRenderer& operator=(const HeavenRenderer&) = delete;

        auto beginFrame() -> VkCommandBuffer;
        auto endFrame() -> void;

        auto beginSwapChainRenderPass(VkCommandBuffer commandBuffer) -> void;
        auto endSwapChainRenderPass(VkCommandBuffer commandBuffer) -> void;

        auto getSwapChainRenderPass() const -> VkRenderPass { return vulkanSwapChain->getRenderPass(); };

        auto getAspectRatio() const -> float { return vulkanSwapChain->extentAspectRatio(); }

        auto getImageCount() const -> uint32_t { return vulkanSwapChain->imageCount(); }

        auto isFrameInProgress() const -> bool {
            return isFrameBegun;
        };

        // We assume that the frame is in progress when returning the current command buffer
        auto getCurrentCommandBuffer() const -> VkCommandBuffer {
            assert(isFrameBegun && "Can't call getCurrentCommandBuffer while frame is not in progress");
            return commandBuffers[currentFrameIndex];
        };

        auto getCurrentFrameIndex() const -> int {
            assert(isFrameBegun && "Can't call getCurrentFrameIndex while frame is not in progress");
            return currentFrameIndex;
        };

    private:
        auto createCommandBuffers() -> void;
        auto freeCommandBuffers() -> void;
        auto recreateSwapChain() -> void;

        HeavenWindow& gameWindow;
        HeavenVkDevice& vulkanDevice;
        std::unique_ptr<HeavenVkSwapChain> vulkanSwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex{0};
        int currentFrameIndex{0};
        bool isFrameBegun{false};
    };
}
