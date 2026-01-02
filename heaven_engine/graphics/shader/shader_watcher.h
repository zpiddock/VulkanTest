#pragma once
#include "shader_compiler.h"
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

namespace heaven_engine {
    struct ShaderWatcher {
        std::filesystem::path path;
        std::filesystem::file_time_type lastWrite;
        std::vector<uint32_t> spirv;
        EShLanguage stage;

        ShaderWatcher(const std::filesystem::path &p, EShLanguage s)
            : path(p), stage(s) {
            lastWrite = std::filesystem::last_write_time(path);
            spirv = CompileShader();
        }

        bool Update() {
            auto now = std::filesystem::last_write_time(path);
            if (now != lastWrite) {
                spirv = CompileShader();
                lastWrite = now;
                return true;
            }
            return false;
        }

    private:
        std::vector<uint32_t> CompileShader() const {
            std::ifstream file(path);
            if (!file)
                throw std::runtime_error("Failed to open shader file: " + path.string());

            const std::string source((std::istreambuf_iterator(file)),
                               std::istreambuf_iterator<char>());

            return CompileGLSL(source, stage, path.string().c_str());
        }
    };
}
