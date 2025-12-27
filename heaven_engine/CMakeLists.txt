add_subdirectory(graphics)

target_sources(${ENGINE_NAME} PUBLIC
        GameWindow.cpp
        GameProgram.cpp
        GameObject.cpp
)
