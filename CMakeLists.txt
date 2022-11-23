cmake_minimum_required(VERSION 3.16)

project(frutibandas)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules ${CMAKE_MODULE_PATH})
set(CMAKE_RUNTIME_OUTPUT ${CMAKE_BINARY_DIR})
set(APP_ICON_RESOURCE_WINDOWS "src/resources.rc")
cmake_policy(SET CMP0072 NEW)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	set(CONAN_DISABLE_CHECK_COMPILER TRUE)
	include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	conan_basic_setup()
else()
	message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

include_directories(include)

set(SRCS
	src/stb_image.cpp
	src/main.cpp
	src/window.cpp
	src/shader_light.cpp
	src/camera.cpp
	src/mesh.cpp
	src/object.cpp
	src/grid_axis.cpp
	src/skybox.cpp
	src/scene.cpp
	src/framebuffer.cpp
	src/game.cpp
	src/graphics.cpp
	src/animatedObject.cpp
	src/joint.cpp
	src/worldPhysics.cpp
	src/character.cpp
	src/IBL.cpp
	src/particle.cpp
	src/audio.cpp
	src/vehicle.cpp
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
	include/shader_light.hpp
	include/camera.hpp
	include/mesh.hpp
	include/object.hpp
	include/grid_axis.hpp
	include/skybox.hpp
	include/scene.hpp
	include/framebuffer.hpp
	include/game.hpp
	include/graphics.hpp
	include/animatedObject.hpp
	include/joint.hpp
	include/worldPhysics.hpp
	include/character.hpp
	include/IBL.hpp
	include/particle.hpp
	include/audio.hpp
	include/vehicle.hpp
    include/renderTexture.hpp
    include/user_interface.hpp
    include/editorUI.hpp
	include/rapidxml.hpp
	include/lightning.hpp
	include/helpers.hpp
	include/allocation.hpp
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

find_package(OpenMP COMPONENTS CXX)
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)
else()
    find_package(OPENMP)
    if(OPENMP_FOUND)
        target_link_libraries(${PROJECT_NAME} ${OpenMP_LD_FLAGS})
    endif()
endif()

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
	target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})
	target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY})
else()
	message(FATAL_ERROR "OpenGL not found.")
endif()

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
