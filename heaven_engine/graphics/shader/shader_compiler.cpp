#include "shader_compiler.h"
#include <stdexcept>

#include "SPIRV/GlslangToSpv.h"

namespace heaven_engine {
    /// Default Vulkan resource limits (can trim as needed)
    static const TBuiltInResource &DefaultResources() {
        static TBuiltInResource r = [] {
            TBuiltInResource res{};
            res.maxLights = 32;
            res.maxClipPlanes = 6;
            res.maxTextureUnits = 32;
            res.maxTextureCoords = 32;
            res.maxVertexAttribs = 16;
            res.maxVertexUniformComponents = 4096;
            res.maxFragmentUniformComponents = 4096;
            res.maxComputeWorkGroupSizeX = 1024;
            res.maxComputeWorkGroupSizeY = 1024;
            res.maxComputeWorkGroupSizeZ = 64;
            res.maxDrawBuffers = 8;
            res.maxVertexOutputComponents = 64;
            res.maxFragmentInputComponents = 64;
            res.limits.generalUniformIndexing = true;
            res.limits.generalAttributeMatrixVectorIndexing = true;
            res.limits.generalVaryingIndexing = true;
            res.limits.generalSamplerIndexing = true;
            res.limits.generalVariableIndexing = true;
            res.limits.generalConstantMatrixVectorIndexing = true;
            return res;
        }();
        return r;
    }

    /// Helper for Vulkan SPIR-V + rules flags
    inline EShMessages VulkanSpvRules() {
        return static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules);
    }

    std::vector<uint32_t> CompileGLSL(const std::string &source, EShLanguage stage, const char *debugName) {
        const char *strings[] = {source.c_str()};

        glslang::TShader shader(stage);
        shader.setStrings(strings, 1);
        shader.setEntryPoint("main");

        // Input / target environment
        shader.setEnvInput(glslang::EShSourceGlsl, stage, glslang::EShClientVulkan, 100);
        shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_3);
        shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_3);

        // Parse with Vulkan + SPIR-V rules
        EShMessages msgs = VulkanSpvRules();

        if (!shader.parse(&DefaultResources(), 100, false, msgs)) {
            std::string log = shader.getInfoLog();
            if (debugName) log = std::string(debugName) + ":\n" + log;
            throw std::runtime_error("GLSL parsing failed:\n" + log);
        }

        // Link program
        glslang::TProgram program;
        program.addShader(&shader);
        if (!program.link(msgs)) {
            std::string log = program.getInfoLog();
            if (debugName) log = std::string(debugName) + ":\n" + log;
            throw std::runtime_error("GLSL linking failed:\n" + log);
        }

        // Convert to SPIR-V
        std::vector<uint32_t> spirv;
        glslang::GlslangToSpv(*program.getIntermediate(stage), spirv);
        return spirv;
    }
}
