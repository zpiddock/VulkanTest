include(FetchContent)

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
        GIT_TAG ${SHADERC_VERSION}  # stable release
)
FetchContent_MakeAvailable(shaderc)