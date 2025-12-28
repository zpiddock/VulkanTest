add_subdirectory(models)

target_sources(${ENGINE_NAME} PUBLIC
        ShaderPipeline.cpp
        VulkanDevice.cpp
        VulkanSwapChain.cpp
        HeavenRenderer.cpp
        SimpleRenderSystem.cpp
)