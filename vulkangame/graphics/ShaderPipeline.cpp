//
// Created by Admin on 23/12/2025.
//

#include "ShaderPipeline.h"

#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>

namespace vulkangame {

    ShaderPipeline::ShaderPipeline(const std::string &shaderPipelinePath) {

        // Eventually we will runtime compile, for now we use the existing compiled shaders
        createGraphicsPipeline(shaderPipelinePath);
    }

    auto ShaderPipeline::readFile(const std::string& shaderName, const std::string& shaderType) -> std::vector<char> {

        auto assetsDirectory = std::format("{}/assets/shaders", std::filesystem::current_path().string());

        std::ifstream file(std::format("{}/{}/{}.{}.spv", assetsDirectory, shaderName, shaderName, shaderType),
            std::ios::ate | std::ios::binary);

        if (!file.is_open()) {

            throw std::runtime_error(std::format("Failed to open shader file {}", shaderName));
        }

        auto fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }

    auto ShaderPipeline::createGraphicsPipeline(const std::string &shaderFilepath) -> void {

        auto vertexCode = readFile(shaderFilepath, "vert");
        auto fragmentCode = readFile(shaderFilepath,"frag");

        std::cout << vertexCode.size() << std::endl;
        std::cout << fragmentCode.size() << std::endl;
    }
}
