add_subdirectory(models)

target_sources(vulkangame PUBLIC
        ShaderPipeline.cpp
        VulkanDevice.cpp
        VulkanSwapChain.cpp
)