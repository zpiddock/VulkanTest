//
// Created by Admin on 23/12/2025.
//
#pragma once
#include <string>
#include <vector>

namespace vulkangame {

    class ShaderPipeline {

        public:
            ShaderPipeline(const std::string& shaderPipelinePath);

        private:
            static auto readFile(const std::string& shaderFilepath, const std::string& shaderType) -> std::vector<char>;
            auto createGraphicsPipeline(const std::string& shaderFilepath) -> void;
    };
}
