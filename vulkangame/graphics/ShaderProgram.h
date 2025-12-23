//
// Created by Admin on 23/12/2025.
//

#pragma once
#include <string>
#include <vector>

namespace vulkangame {
    class ShaderProgram {
        public:
            ShaderProgram(const std::string& shaderName);

        private:
            static auto readShaderFile(const std::string& filePath) -> std::vector<char>;

            auto createShader(const std::string& shaderName) -> void;

            std::vector<std::string> supported_shader_types = {
                "vert",
                "frag"
            };
    };
}
