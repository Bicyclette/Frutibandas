cmake_minimum_required(VERSION 3.2)

project(frutibandas)

set(CMAKE_TOOLCHAIN_FILE "C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules ${CMAKE_MODULE_PATH})
set(CMAKE_RUNTIME_OUTPUT ${CMAKE_BINARY_DIR})
set(APP_ICON_RESOURCE_WINDOWS "src/resources.rc")
cmake_policy(SET CMP0072 NEW)

include_directories(include)

set(SRCS
	src/stb_image.cpp
	src/main.cpp
	src/mesh.cpp
	src/window.cpp
	src/shader_light.cpp
	src/camera.cpp
	src/scene.cpp
	src/framebuffer.cpp
	src/game.cpp
	src/graphics.cpp
	src/audio.cpp
    src/renderTexture.cpp
    src/user_interface.cpp
	src/network_client.cpp
	src/helpers.cpp
	src/mouse.cpp
	src/writer.cpp
	src/bandas.cpp
	src/logic.cpp
	src/gl_debug.cpp)

set(HEADERS
	include/stb_image.h
	include/color.hpp
	include/window.hpp
	include/mesh.hpp
	include/shader_light.hpp
	include/camera.hpp
	include/scene.hpp
	include/framebuffer.hpp
	include/game.hpp
	include/graphics.hpp
	include/audio.hpp
    include/renderTexture.hpp
    include/user_interface.hpp
	include/helpers.hpp
	include/network_client.hpp
	include/mouse.hpp
	include/avatar.hpp
	include/writer.hpp
	include/bandas.hpp
	include/communication.hpp
	include/logic.hpp
	include/music.hpp
	include/tinyutf8/tinyutf8.h
	include/card.hpp
	include/gl_debug.hpp)

add_executable(${PROJECT_NAME} ${SRCS} ${HEADERS} ${APP_ICON_RESOURCE_WINDOWS})

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
	target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})
	target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENGL_LIBRARY})
else()
	message(FATAL_ERROR "OpenGL not found.")
endif()

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main SDL2::SDL2)

find_package(Freetype CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Freetype::Freetype)

find_package(OpenAL CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenAL::OpenAL)

find_package(SndFile CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SndFile::sndfile)

find_package(GLEW CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    find_package(LLVM_OPENMP CONFIG REQUIRED)
	target_link_libraries(${PROJECT_NAME} PRIVATE LLVM_OPENMP::LLVM_OPENMP)
else()
	find_package(OpenMP COMPONENTS CXX)
	if(OpenMP_CXX_FOUND)
		target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)
	else()
		find_package(OPENMP)
		if(OPENMP_FOUND)
			target_link_libraries(${PROJECT_NAME} ${OpenMP_LD_FLAGS})
		endif()
	endif()
endif()

find_package(unofficial-enet CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::enet::enet)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
