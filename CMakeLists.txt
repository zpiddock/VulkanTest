cmake_minimum_required(VERSION 4.1)
project(VulkanTest)

set(CMAKE_CXX_STANDARD 26)

#Dependencies
# Set Versions
set(GLFW_VERSION 3.4)
set(GLM_VERSION 1.0.2)
# We need to have Vulkan
find_package(Vulkan REQUIRED)

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

add_library(vulkangame STATIC)

add_subdirectory(vulkangame)

add_executable(${PROJECT_NAME}
        main.cpp
)

target_include_directories(vulkangame PUBLIC ${PROJECT_SOURCE_DIR}/ ${PROJECT_SOURCE_DIR}/vulkangame)
target_link_libraries(vulkangame PUBLIC glfw ${GLFW_LIBRARIES} Vulkan::Vulkan glm::glm)

target_link_libraries(${PROJECT_NAME} PUBLIC vulkangame)
