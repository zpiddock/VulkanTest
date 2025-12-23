find_package(Vulkan REQUIRED)

function(add_shaders TARGET_NAME)

    message("CMAKE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}")

    set(SHADER_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/${ARGN}")
    set(SHADER_OUT_FOLDER "${CMAKE_BINARY_DIR}/assets/shaders")

    file(GLOB SHADERS "${SHADER_FOLDER}/*.vert" "${SHADER_FOLDER}/*.frag")

    file(MAKE_DIRECTORY ${SHADER_OUT_FOLDER})

    message("OUTPUT DIR ${SHADER_OUT_FOLDER}")

    foreach(SHADER ${SHADERS})
        get_filename_component(SHADER_NAME ${SHADER} NAME)
        set(SHADER_OUT_NAME "${SHADER_OUT_FOLDER}/${ARGN}/${SHADER_NAME}.spv")
        list(APPEND SHADER_OUT_NAMES ${SHADER_OUT_NAME})
        add_custom_command(
                MAIN_DEPENDENCY ${SHADER}
                OUTPUT ${SHADER_OUT_NAME}
                COMMAND Vulkan::glslc  ${SHADER} "-o" ${SHADER_OUT_NAME}
                VERBATIM)
    endforeach()

    add_custom_target(shaders DEPENDS ${SHADER_OUT_NAMES})

endfunction()