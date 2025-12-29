add_subdirectory(graphics)
add_subdirectory(utils)

target_sources(${ENGINE_NAME} PUBLIC
        HeavenWindow.cpp
        GameObject.cpp
        InputManager.cpp
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
        imguilib
        tinyobjloader
)

target_sources(imguilib PUBLIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imstb_truetype.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
)

target_link_libraries(imguilib PRIVATE glfw Vulkan::Vulkan)
target_include_directories(imguilib PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)

