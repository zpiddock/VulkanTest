//
// Created by Admin on 23/12/2025.
//
#pragma once

#include "VulkanDevice.h"

namespace vulkangame {

    struct PipelineConfigInfo {

        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class ShaderPipeline {

        public:
            ShaderPipeline(VulkanDevice& device, const std::string& shaderPipelinePath, const PipelineConfigInfo& configInfo);
            ~ShaderPipeline();

            ShaderPipeline(const ShaderPipeline&) = delete;
            void operator=(const ShaderPipeline&) = delete;

            auto bind(VkCommandBuffer buffer) -> void;
            static auto defaultPipelineConfigInfo(uint32_t width, uint32_t height) -> PipelineConfigInfo;

        private:
            auto readFile(const std::string& shaderFilepath, const std::string& shaderType) -> std::string;
            auto createGraphicsPipeline(const std::string& shaderFilepath, const PipelineConfigInfo& configInfo) -> void;

            auto compileShader(const std::string& shaderFilepath, const std::string& shaderType) -> std::vector<uint32_t>;
            auto createShaderModule(const std::vector<uint32_t>& shaderCode, VkShaderModule* shaderModule) -> void;

            VulkanDevice& vulkanDevice;
            VkPipeline pipeline;
            VkShaderModule vertModule;
            VkShaderModule fragModule;
    };
}
