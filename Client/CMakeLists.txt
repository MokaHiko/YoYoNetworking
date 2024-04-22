cmake_minimum_required(VERSION 3.21.0)
project(Client VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
	src/GameConsole.cpp
	src/GameConsole.h

	src/Sandbox.h
	src/Sandbox.cpp
)

# if(CMAKE_BUILD_TYPE AND CMAKE_BUILD_TYPE STREQUAL "Debug" OR TRUE)
# endif()

target_include_directories(${PROJECT_NAME} PUBLIC src/ ${CMAKE_SOURCE_DIR}/Networking/include)
target_link_libraries(${PROJECT_NAME} PUBLIC 
	YoYo 
	Networking
)



# add_custom_target(copy_assets ALL
# 	COMMAND ${CMAKE_COMMAND} -E copy_directory
# 	${PROJECT_SOURCE_DIR}/assets
# 	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
# 	COMMENT "Copying resources into bin and binary folder."
# 	COMMAND cmd /c ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/shaders/compile_shaders.bat
# 	COMMENT "Compiling shaders.")

# add_dependencies(${PROJECT_NAME} copy_assets)

set_target_properties(
	${PROJECT_NAME} PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})