cmake_minimum_required(VERSION 3.18.1 FATAL_ERROR)

set(PROJECT_NAME OpenGLNetwork)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} 
    src/main.cpp
	src/Circle.h src/Circle.cpp
	src/Engine.h src/Engine.cpp
	src/Line.h src/Line.cpp
	src/ResourceManager.h src/ResourceManager.cpp
	src/ShaderProgram.h src/ShaderProgram.cpp
	src/Network.h src/Network.cpp
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw-3.3.6)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

include_directories(external/glm-master)

include_directories(external/rapidjson-master/include)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/MACHINE:X64")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
	COMMAND ${CMAKE_COMMAND} -E copy_directory 
	${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
