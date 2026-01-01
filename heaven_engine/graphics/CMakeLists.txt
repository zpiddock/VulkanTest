add_subdirectory(models)
add_subdirectory(integrations)

target_sources(${ENGINE_NAME} PUBLIC
        HeavenShaderPipeline.cpp
        HeavenVkDevice.cpp
        HeavenVkSwapChain.cpp
        HeavenRenderer.cpp
        SimpleRenderSystem.cpp
        HvnCamera.cpp
        HeavenVkBuffer.cpp
)