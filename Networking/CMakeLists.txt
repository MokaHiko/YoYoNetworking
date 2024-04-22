cmake_minimum_required(VERSION 3.21.0)
project(Networking VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} STATIC
	src/SocketAddress.h
	src/SocketAddress.cpp

	src/UDPSocket.h
	src/UDPSocket.cpp

	src/TCPSocket.h
	src/TCPSocket.cpp
	
	src/SocketUtil.h
	src/SocketUtil.cpp

	include/FumblyNet.h
	include/FumblyNet.cpp
)
target_precompile_headers(${PROJECT_NAME} PUBLIC src/epch.h)
target_include_directories(${PROJECT_NAME} PRIVATE src/)
target_link_libraries(${PROJECT_NAME} ws2_32)
