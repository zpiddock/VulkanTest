#pragma once
#include <glslang/Public/ShaderLang.h>
#include <string>
#include <vector>

#include "glm/glm.hpp"


namespace heaven_engine {
    struct GlslangInit {
        GlslangInit() { glslang::InitializeProcess(); }
        ~GlslangInit() { glslang::FinalizeProcess(); }
    };

   auto CompileGLSL(const std::string &source, EShLanguage stage,
                                             const char *debugName = nullptr) -> std::vector<uint32_t>;
}
