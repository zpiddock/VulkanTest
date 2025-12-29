add_subdirectory(input)

target_sources(${PROJECT_NAME} PUBLIC
        main.cpp
        GameProgram.cpp
        GameConfigInfo.cpp
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${ENGINE_NAME})