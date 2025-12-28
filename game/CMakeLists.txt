target_sources(${PROJECT_NAME} PUBLIC
        main.cpp
        GameProgram.cpp
)
target_link_libraries(${PROJECT_NAME} PUBLIC ${ENGINE_NAME})