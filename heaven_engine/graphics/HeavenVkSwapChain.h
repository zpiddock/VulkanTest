#pragma once

// vulkan headers
#include <vulkan/vulkan.h>

// std lib headers
#include <memory>
#include <string>
#include <vector>

#include "HeavenVkDevice.h"

namespace heaven_engine {
    class HeavenVkSwapChain {
    public:
        static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

        HeavenVkSwapChain(HeavenVkDevice &deviceRef, VkExtent2D windowExtent);

        HeavenVkSwapChain(HeavenVkDevice &deviceRef, VkExtent2D windowExtent,
                        const std::shared_ptr<HeavenVkSwapChain> &previousSwapChain);

        ~HeavenVkSwapChain();

        HeavenVkSwapChain(const HeavenVkSwapChain &) = delete;

        HeavenVkSwapChain &operator=(const HeavenVkSwapChain &) = delete;

        VkFramebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
        VkRenderPass getRenderPass() { return renderPass; }
        VkImageView getImageView(int index) { return swapChainImageViews[index]; }
        size_t imageCount() { return swapChainImages.size(); }
        VkFormat getSwapChainImageFormat() { return swapChainImageFormat; }
        VkExtent2D getSwapChainExtent() { return currentSwapChainExtent; }
        uint32_t width() { return currentSwapChainExtent.width; }
        uint32_t height() { return currentSwapChainExtent.height; }

        float extentAspectRatio() {
            return static_cast<float>(currentSwapChainExtent.width) / static_cast<float>(currentSwapChainExtent.height);
        }

        VkFormat findDepthFormat();

        VkResult acquireNextImage(uint32_t *imageIndex);

        VkResult submitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);

        auto compareSwapFormats(const HeavenVkSwapChain& swapChain) const -> bool {
            return swapChainImageFormat == swapChain.swapChainImageFormat &&
                swapChainDepthFormat == swapChain.swapChainDepthFormat;
        };

    private:
        auto init() -> void;

        void createSwapChain();

        void createImageViews();

        void createDepthResources();

        void createRenderPass();

        void createFramebuffers();

        void createSyncObjects();

        // Helper functions
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(
            const std::vector<VkSurfaceFormatKHR> &availableFormats);

        VkPresentModeKHR chooseSwapPresentMode(
            const std::vector<VkPresentModeKHR> &availablePresentModes);

        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        VkFormat swapChainImageFormat;
        VkFormat swapChainDepthFormat;
        VkExtent2D currentSwapChainExtent;

        std::vector<VkFramebuffer> swapChainFramebuffers;
        VkRenderPass renderPass;

        std::vector<VkImage> depthImages;
        std::vector<VkDeviceMemory> depthImageMemorys;
        std::vector<VkImageView> depthImageViews;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;

        HeavenVkDevice &device;
        VkExtent2D windowExtent;

        VkSwapchainKHR swapChain;
        std::shared_ptr<HeavenVkSwapChain> oldSwapChain;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;
        size_t currentFrame = 0;
    };
} // namespace lve
