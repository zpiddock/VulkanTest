cmake_minimum_required(VERSION 4.1)
project(VulkanTest)

set(CMAKE_CXX_STANDARD 26)

#Dependencies
# Set Versions
set(GLFW_VERSION 3.4)
set(GLM_VERSION 1.0.2)
# We need to have Vulkan
find_package(Vulkan REQUIRED)

if(MINGW)
    add_compile_options(-Wno-maybe-uninitialized)
endif()

#Fetch GLFW
include(FetchContent)
FetchContent_Declare(
        glfw
        URL https://github.com/glfw/glfw/releases/download/${GLFW_VERSION}/glfw-${GLFW_VERSION}.zip
        FIND_PACKAGE_ARGS ${GLFW_VERSION}
)
set(BUILD_EXAMPLES OFF CACHE INTERNAL "")
fetchcontent_makeavailable(glfw)

# GLM For Maths
FetchContent_Declare(
        glm
        URL https://github.com/g-truc/glm/releases/download/${GLM_VERSION}/glm-${GLM_VERSION}.zip
        FIND_PACKAGE_ARGS ${GLM_VERSION}
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
        spirv-headers
        GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Headers.git
        GIT_TAG vulkan-sdk-1.4.335.0
)
FetchContent_MakeAvailable(spirv-headers)

FetchContent_Declare(
        spirv-tools
        GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Tools.git
        GIT_TAG v2025.5
)
FetchContent_MakeAvailable(spirv-tools)

FetchContent_Declare(
        glslang
        GIT_REPOSITORY https://github.com/KhronosGroup/glslang.git
        GIT_TAG 16.1.0
)
FetchContent_MakeAvailable(glslang)

# ----------------------------
# FetchContent for Shaderc
# ----------------------------
# Shaderc options
set(SHADERC_SKIP_TESTS ON CACHE BOOL "" FORCE)
set(SHADERC_SKIP_EXAMPLES ON CACHE BOOL "" FORCE)

# Build Shaderc as shared library (DLL)
set(SHADERC_BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)

# Use shared CRT for MinGW if needed
set(SHADERC_ENABLE_SHARED_CRT OFF CACHE BOOL "" FORCE)
set(SHADERC_ENABLE_EXCEPTIONS ON CACHE BOOL "" FORCE)

# Force internal glslang build to avoid linker issues
set(SHADERC_FORCE_BUILD_GLSLANG ON CACHE BOOL "" FORCE)

# Enable SPIRV tools
set(SHADERC_ENABLE_SPIRV_TOOLS ON CACHE BOOL "" FORCE)
set(SPIRV_TOOLS_ENABLE_WERROR OFF CACHE BOOL "" FORCE)  # avoid MinGW warnings
set(SPIRV_TOOLS_BUILD_STATIC ON CACHE BOOL "" FORCE)
set(SPIRV_HEADERS_SKIP_INSTALL ON CACHE BOOL "" FORCE)
set(SPIRV_SKIP_EXECUTABLES ON CACHE BOOL "" FORCE)
FetchContent_Declare(
        shaderc
        GIT_REPOSITORY https://github.com/google/shaderc.git
        GIT_TAG v2025.5  # stable release
)
FetchContent_MakeAvailable(shaderc)

# Setup shader compilation
include(compile_shaders.cmake)

# --------------------
# Game library
# --------------------
add_library(vulkangame SHARED)
add_subdirectory(vulkangame) # optional if you have a submodule

target_include_directories(vulkangame PUBLIC
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/vulkangame
)

target_link_libraries(vulkangame PUBLIC
        glfw
        Vulkan::Vulkan
        glm::glm
        shaderc_shared
)

# --------------------
# Main executable
# --------------------
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC vulkangame)

# --------------------
# Runtime Shaderc DLL copy
# --------------------
add_custom_command(TARGET vulkangame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE_DIR:shaderc_shared>/$<TARGET_FILE_NAME:shaderc_shared>"
        $<TARGET_FILE_DIR:vulkangame>
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

add_shaders(shaders "simple_shader")
add_dependencies(vulkangame shaders)

# Optionally include shaderc DLL for runtime
install(FILES "$<TARGET_FILE:shaderc_shared>"
        DESTINATION bin)

install(TARGETS ${PROJECT_NAME} DESTINATION "./")
install(TARGETS vulkangame DESTINATION "./")
install(DIRECTORY "${PROJECT_SOURCE_DIR}/assets" DESTINATION "./assets")

set(CPACK_PACKAGE_NAME "VulkanGame")
set(CPACK_PACKAGE_VERSION "0.0.1")
set(CPACK_GENERATOR "ZIP")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_SOURCE_IGNORE_FILES "build")
include(CPack)