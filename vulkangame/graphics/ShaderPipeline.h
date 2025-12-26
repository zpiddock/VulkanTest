//
// Created by Admin on 23/12/2025.
//
#pragma once

#include "VulkanDevice.h"

namespace vulkangame {

    struct PipelineConfigInfo {

    };

    class ShaderPipeline {

        public:
            ShaderPipeline(VulkanDevice& device, const std::string& shaderPipelinePath, const PipelineConfigInfo& configInfo);
            ~ShaderPipeline() {};

            ShaderPipeline(const ShaderPipeline&) = delete;
            void operator=(const ShaderPipeline&) = delete;

            static auto defaultPipelineConfigInfo(uint32_t width, uint32_t height) -> PipelineConfigInfo;

        private:
            static auto readFile(const std::string& shaderFilepath, const std::string& shaderType) -> std::vector<char>;
            auto createGraphicsPipeline(const std::string& shaderFilepath, const PipelineConfigInfo& configInfo) -> void;

            auto createShaderModule(const std::vector<char>& shaderCode, VkShaderModule* shaderModule) -> void;

            VulkanDevice& device;
            VkPipeline pipeline;
            VkShaderModule vertModule;
            VkShaderModule fragModule;
    };
}
