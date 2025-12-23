//
// Created by Admin on 23/12/2025.
//

#include "ShaderProgram.h"

#include <fstream>
#include <filesystem>
#include <iostream>

namespace vulkangame {

    ShaderProgram::ShaderProgram(const std::string &shaderName) {

        createShader(shaderName);
    }

    auto ShaderProgram::readShaderFile(const std::string &filePath) -> std::vector<char> {

        std::ifstream file(filePath, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open shader file: " + filePath);
        }

        size_t fileSize = file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        return buffer;
    }

    auto ShaderProgram::createShader(const std::string &shaderName) -> void {

        for (const auto& shaderType : supported_shader_types) {

            std::string shaderFileName = shaderName + "." + shaderType + ".spv";
            std::string filePath = std::format("{}/assets/shaders/{}/{}", std::filesystem::current_path().string(), shaderName, shaderFileName);
            std::cout << filePath << std::endl;

            if (std::filesystem::exists(filePath)) {

                auto shaderData = readShaderFile(filePath);

                std::cout << shaderData.size() << std::endl;
            } else {

                std::cout << std::format("Shader {} doesn't exit, this may be normal if you don't have vert/frag for this shader!", shaderName) << std::endl;
            }
        }
    }
}
