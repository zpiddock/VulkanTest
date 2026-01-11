add_subdirectory(integrations)
add_subdirectory(models)
add_subdirectory(shader)
add_subdirectory(system)

target_sources(${ENGINE_NAME} PUBLIC
        HeavenShaderPipeline.cpp
        HeavenVkDevice.cpp
        HeavenVkSwapChain.cpp
        HeavenRenderer.cpp
        HvnCamera.cpp
        HeavenVkBuffer.cpp
        HeavenDescriptors.cpp
)