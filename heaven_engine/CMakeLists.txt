add_subdirectory(graphics)

target_sources(${ENGINE_NAME} PUBLIC
        HeavenWindow.cpp
        GameObject.cpp
)

target_include_directories(${ENGINE_NAME} PUBLIC
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/heaven_engine
)

target_link_libraries(${ENGINE_NAME} PUBLIC
        glfw
        Vulkan::Vulkan
        glm::glm
        shaderc_shared
)
