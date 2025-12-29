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
include(shaderc_fetch)

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

# --------------------
# Runtime Shaderc DLL copy
# --------------------
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE_DIR:shaderc_shared>/$<TARGET_FILE_NAME:shaderc_shared>"
        $<TARGET_FILE_DIR:${ENGINE_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

# Optionally include shaderc DLL for runtime
install(FILES "$<TARGET_FILE:shaderc_shared>"
        DESTINATION bin)

install(TARGETS ${PROJECT_NAME} DESTINATION "./")
install(TARGETS ${ENGINE_NAME} DESTINATION "./")
install(DIRECTORY "${PROJECT_SOURCE_DIR}/assets" DESTINATION "./assets")

set(CPACK_PACKAGE_NAME "heaven_engine")
set(CPACK_PACKAGE_VERSION "0.0.1")
set(CPACK_GENERATOR "ZIP")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_SOURCE_IGNORE_FILES "build")
include(CPack)