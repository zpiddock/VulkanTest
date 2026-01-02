#pragma once

// std
#include <memory>
#include <unordered_map>
#include <vulkan/vulkan_core.h>

#include "HeavenVkDevice.h"

namespace heaven_engine {
    class HvnDescriptorSetLayout {
    public:
        class Builder {
        public:
            Builder(HeavenVkDevice &device) : vkDevice{device} {
            }

            Builder &addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);

            std::unique_ptr<HvnDescriptorSetLayout> build() const;

        private:
            HeavenVkDevice &vkDevice;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        HvnDescriptorSetLayout(
            HeavenVkDevice &HeavenVkDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);

        ~HvnDescriptorSetLayout();

        HvnDescriptorSetLayout(const HvnDescriptorSetLayout &) = delete;

        HvnDescriptorSetLayout &operator=(const HvnDescriptorSetLayout &) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        HeavenVkDevice &vkDevice;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class HvnDescriptorWriter;
    };

    class HvnDescriptorPool {
    public:
        class Builder {
        public:
            Builder(HeavenVkDevice &vkDevice) : vkDevice{vkDevice} {}

            Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder &setMaxSets(uint32_t count);
            [[nodiscard]] std::unique_ptr<HvnDescriptorPool> build() const;

        private:
            HeavenVkDevice &vkDevice;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        HvnDescriptorPool(
            HeavenVkDevice &vkDevice,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize> &poolSizes);
        ~HvnDescriptorPool();
        HvnDescriptorPool(const HvnDescriptorPool &) = delete;
        HvnDescriptorPool &operator=(const HvnDescriptorPool &) = delete;

        bool allocateDescriptorSet(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

        void resetPool();

        VkDescriptorPool getDescriptorPool() const { return descriptorPool; }

    private:
        HeavenVkDevice &vkDevice;
        VkDescriptorPool descriptorPool;

        friend class HvnDescriptorWriter;
    };

    class HvnDescriptorWriter {
    public:
        HvnDescriptorWriter(HvnDescriptorSetLayout &setLayout, HvnDescriptorPool &pool);

        HvnDescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
        HvnDescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

        bool build(VkDescriptorSet &set);
        void overwrite(VkDescriptorSet &set);

    private:
        HvnDescriptorSetLayout &setLayout;
        HvnDescriptorPool &pool;
        std::vector<VkWriteDescriptorSet> writes;
    };
}
