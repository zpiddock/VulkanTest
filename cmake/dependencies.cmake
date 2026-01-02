
# We need to have Vulkan
find_package(Vulkan REQUIRED)

# Set options
set(ENABLE_SPVREMAPPER OFF CACHE BOOL "" FORCE)
set(ENABLE_GLSLANG_BINARIES OFF CACHE BOOL "" FORCE)
set(ENABLE_HLSL OFF CACHE BOOL "" FORCE)
set(ENABLE_OPT OFF CACHE BOOL "" FORCE)

# Disable Glslang tests
set(SKIP_GLSLANG_INSTALL ON CACHE BOOL "" FORCE)
set(ENABLE_GLSLANG_BINARIES OFF CACHE BOOL "" FORCE)

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

# SPIRV-Headers
FetchContent_Declare(
        spirv-headers
        GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Headers.git
        GIT_TAG ${SPIRV_HEADERS_VERSION}
)
FetchContent_MakeAvailable(spirv-headers)

# GLSLang
FetchContent_Declare(
        glslang
        GIT_REPOSITORY https://github.com/KhronosGroup/glslang.git
        GIT_TAG ${GLSLANG_VERSION}
)
FetchContent_MakeAvailable(glslang)

# IMGUI
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG docking
)
FetchContent_MakeAvailable(imgui)

# TinyObjLoader
FetchContent_Declare(
        tinyobjloader
        GIT_REPOSITORY http://github.com/tinyobjloader/tinyobjloader.git
        GIT_TAG v2.0.0rc13
)
FetchContent_MakeAvailable(tinyobjloader)