cmake_minimum_required(VERSION 4.1)
project(TestGame)

set(CMAKE_CXX_STANDARD 26)

# Engine / Game Names
set(ENGINE_NAME heaven_engine)

#Dependencies
# Set Versions
set(GLFW_VERSION 3.4)
set(GLM_VERSION 1.0.2)
set(SPIRV_HEADERS_VERSION vulkan-sdk-1.4.335.0)
set(SPIRV_TOOLS_VERSION v2025.5)
set(GLSLANG_VERSION 16.1.0)
set(SHADERC_VERSION v2025.5)

if(MINGW)
    add_compile_options(-Wno-maybe-uninitialized)
    set(CMAKE_SH "CMAKE_SH-NOTFOUND") # avoid Unix shell issues
endif()

# List all in cmake folder, allows us to include by filename
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
# Standard Dependencies
include(dependencies)
# Shaderc gets it's own file
#include(shaderc_fetch)

# --------------------
# IMGUI Library
# --------------------
add_library(imguilib SHARED)

# --------------------
# Engine library
# --------------------
add_library(${ENGINE_NAME} SHARED)
add_subdirectory(heaven_engine)

# --------------------
# Game executable
# --------------------
add_executable(${PROJECT_NAME})
add_subdirectory(game)

# To remove the 'lib' prefix from all targets defined in the project
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_PREFIX "")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

# --------------------
# Runtime Copy Assets Dir
# --------------------
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

set(CPACK_INSTALL_CMAKE_PROJECTS
        "${CMAKE_BINARY_DIR};${PROJECT_NAME};ALL;/"
)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION .
)

install(TARGETS ${ENGINE_NAME}
        RUNTIME DESTINATION .
        LIBRARY DESTINATION .
)

install(TARGETS imguilib
        RUNTIME DESTINATION .
        LIBRARY DESTINATION .
)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/assets" DESTINATION .)

set(CPACK_PACKAGE_NAME "heaven_engine")
set(CPACK_PACKAGE_VERSION "0.0.1")
set(CPACK_GENERATOR "ZIP")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_SOURCE_IGNORE_FILES "build")
include(CPack)