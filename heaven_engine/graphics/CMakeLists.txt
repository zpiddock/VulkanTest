add_subdirectory(models)

target_sources(${ENGINE_NAME} PUBLIC
        HeavenShaderPipeline.cpp
        HeavenVkDevice.cpp
        HeavenVkSwapChain.cpp
        HeavenRenderer.cpp
        SimpleRenderSystem.cpp
)