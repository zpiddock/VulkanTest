//
// Created by Admin on 23/12/2025.
//

#include "ShaderPipeline.h"

#include <string>
#include <fstream>
#include <filesystem>
#include <format>
#include <iostream>
#include <shaderc/shaderc.hpp>

#include "models/BasicModel.h"

namespace vulkangame {

    ShaderPipeline::ShaderPipeline(VulkanDevice& device,
        const std::string& shaderPipelinePath,
        const PipelineConfigInfo& configInfo) : vulkanDevice(device) {

        // Eventually we will runtime compile, for now we use the existing compiled shaders
        createGraphicsPipeline(shaderPipelinePath, configInfo);
    }

    ShaderPipeline::~ShaderPipeline() {

        ::vkDestroyShaderModule(vulkanDevice.device(), vertModule, nullptr);
        ::vkDestroyShaderModule(vulkanDevice.device(), fragModule, nullptr);
        ::vkDestroyPipeline(vulkanDevice.device(), pipeline, nullptr);
    }

    auto ShaderPipeline::compileShader(
        const std::string &shaderFilepath,
        const std::string &shaderType) -> std::vector<uint32_t> {

        std::string source = readFile(shaderFilepath, shaderType);

        shaderc_shader_kind kind = {};

        if (shaderType == "vert") kind = shaderc_vertex_shader;
        if (shaderType == "frag") kind = shaderc_fragment_shader;

        shaderc::Compiler compiler;
        shaderc::CompileOptions options;

        options.SetOptimizationLevel(shaderc_optimization_level_performance);

        shaderc::SpvCompilationResult module =
            compiler.CompileGlslToSpv(source, kind, shaderFilepath.c_str(), options);

        if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
            throw std::runtime_error("Shader compilation failed (" + shaderFilepath + "): " +
                                     module.GetErrorMessage());
        }

        std::cout << std::format("Compiled shader {} of type {}", shaderFilepath, shaderType) << "\n";

        return std::vector(module.cbegin(), module.cend());
    }

    auto ShaderPipeline::readFile(const std::string& shaderName, const std::string& shaderType) -> std::string {

        auto assetsDirectory = std::format("{}/assets/shaders", std::filesystem::current_path().string());

        std::ifstream file(std::format("{}/{}/{}.{}", assetsDirectory, shaderName, shaderName, shaderType),
            std::ios::in);

        if (!file.is_open()) {

            throw std::runtime_error("Failed to open shader file: " + shaderName);
        }

        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    auto ShaderPipeline::createGraphicsPipeline(const std::string &shaderFilepath, const PipelineConfigInfo &configInfo) -> void {

        // auto vertexCode = readFile(shaderFilepath, "vert");
        // auto fragmentCode = readFile(shaderFilepath,"frag");

        // TODO: Maybe dont compile shaders on every window resize....
        auto vertexCode = compileShader(shaderFilepath, "vert");
        auto fragmentCode = compileShader(shaderFilepath, "frag");

        createShaderModule(vertexCode, &vertModule);
        createShaderModule(fragmentCode, &fragModule);

        VkPipelineShaderStageCreateInfo shaderStages[2];
        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].module = vertModule;
        shaderStages[0].pName = "main";
        shaderStages[0].flags = 0;
        shaderStages[0].pNext = nullptr;
        shaderStages[0].pSpecializationInfo = nullptr;
        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].module = fragModule;
        shaderStages[1].pName = "main";
        shaderStages[1].flags = 0;
        shaderStages[1].pNext = nullptr;
        shaderStages[1].pSpecializationInfo = nullptr;

        auto bindingDescriptions = BasicModel::Vertex::getBindingDescriptions();
        auto attributeDescriptions = BasicModel::Vertex::getAttributeDescriptions();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
        vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

        VkGraphicsPipelineCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        createInfo.stageCount = 2;
        createInfo.pStages = shaderStages;
        createInfo.pVertexInputState = &vertexInputInfo;
        createInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
        createInfo.pViewportState = &configInfo.viewportInfo;
        createInfo.pRasterizationState = &configInfo.rasterizationInfo;
        createInfo.pMultisampleState = &configInfo.multisampleInfo;
        createInfo.pColorBlendState = &configInfo.colorBlendInfo;
        createInfo.pDepthStencilState = &configInfo.depthStencilInfo;
        createInfo.pDynamicState = &configInfo.dynamicStateInfo;

        createInfo.layout = configInfo.pipelineLayout;
        createInfo.renderPass = configInfo.renderPass;
        createInfo.subpass = configInfo.subpass;

        createInfo.basePipelineIndex = -1;
        createInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (::vkCreateGraphicsPipelines(
            vulkanDevice.device(),
            VK_NULL_HANDLE,
            1,
            &createInfo,
            nullptr,
            &pipeline) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline");
        }
    }

    auto ShaderPipeline::bind(VkCommandBuffer buffer) -> void {

        ::vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    }

    auto ShaderPipeline::defaultPipelineConfigInfo(PipelineConfigInfo& configInfo) -> void {

        configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

        configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        configInfo.viewportInfo.viewportCount = 1;
        configInfo.viewportInfo.pViewports = nullptr;
        configInfo.viewportInfo.scissorCount = 1;
        configInfo.viewportInfo.pScissors = nullptr;

        configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
        configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
        configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
        configInfo.rasterizationInfo.lineWidth = 1.0f;
        configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
        configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
        configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
        configInfo.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
        configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

        configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
        configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        configInfo.multisampleInfo.minSampleShading = 1.0f;           // Optional
        configInfo.multisampleInfo.pSampleMask = nullptr;             // Optional
        configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
        configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

        configInfo.colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
                VK_COLOR_COMPONENT_A_BIT;
        configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
        configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
        configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
        configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
        configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
        configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
        configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

        configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
        configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
        configInfo.colorBlendInfo.attachmentCount = 1;
        configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
        configInfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
        configInfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
        configInfo.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
        configInfo.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

        configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
        configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
        configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
        configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
        configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
        configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
        configInfo.depthStencilInfo.front = {};  // Optional
        configInfo.depthStencilInfo.back = {};   // Optional

        configInfo.dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
        configInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        configInfo.dynamicStateInfo.pDynamicStates = configInfo.dynamicStateEnables.data();
        configInfo.dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(configInfo.dynamicStateEnables.size());
        configInfo.dynamicStateInfo.flags = 0;
    }

    auto ShaderPipeline::createShaderModule(const std::vector<uint32_t> &shaderCode, VkShaderModule* shaderModule) -> void {

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = shaderCode.size() * sizeof(uint32_t);
        createInfo.pCode = shaderCode.data();

        if (::vkCreateShaderModule(vulkanDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {

            throw std::runtime_error("Failed to create shader module");
        }
    }
}
