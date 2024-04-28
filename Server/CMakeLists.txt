cmake_minimum_required(VERSION 3.21.0)
project(Server VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
	src/Server.h
	src/Server.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC src/ ${CMAKE_SOURCE_DIR}/Networking/include)
target_link_libraries(${PROJECT_NAME} PUBLIC 
	YoYo 
	Networking
)

set_target_properties(
	${PROJECT_NAME} PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})