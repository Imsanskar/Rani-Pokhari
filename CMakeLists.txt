cmake_minimum_required(VERSION 3.1)

# set compiler to clang
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_DEBUG_INIT -Wall)

project(RaniPokhari VERSION 1.1 LANGUAGES C CXX)

file(GLOB INCLUDES 
	includes/*.h 
	includes/*.hpp
	${CMAKE_SOURCE_DIR}/Dependencies/includes/*
)
file(GLOB SRC src/*.cpp src/*.c)

include_directories(
	${CMAKE_SOURCE_DIR}/includes
	${CMAKE_SOURCE_DIR}/Dependencies/includes
)

add_executable(RaniPokhari src/main.cpp ${SRC} ${INCLUDES})


if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	message("You are on windows")
	link_directories(${CMAKE_SOURCE_DIR}/Dependencies)
	link_directories(${CMAKE_SOURCE_DIR}/Dependencies/Release)
	target_link_libraries(RaniPokhari glfw3 opengl32 assimp-vc142-mt.lib assimp.lib zlibstatic.lib zlib.lib)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message("You are on linux")
	target_link_libraries(RaniPokhari glfw GL dl)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
