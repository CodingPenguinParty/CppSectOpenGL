cmake_minimum_required(VERSION 3.19)
project(CppSectOpenGL)

set(CMAKE_CXX_STANDARD 17)
if (APPLE)
    set(CMAKE_CXX_FLAGS "-framework Cocoa -framework OpenGL")
endif()

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

find_package(glm REQUIRED)
if (NOT ${GLM_FOUND})
    message(FATAL_ERROR "Library \"glm\" should be installed!")
endif()

include_directories(
        ${OPENGL_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIR})

### SOIL2

# Check for premake4
execute_process(COMMAND premake4 --version
                RESULT_VARIABLE PREMAKE_CHECK_RESULT)
if (NOT ${PREMAKE_CHECK_RESULT} EQUAL "1")
    message(FATAL_ERROR "Package \"premake4\" should be installed!")
endif()

# Generate Makefile
execute_process(COMMAND premake4 gmake
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/SOIL2)

# Generate static library
string(TOLOWER ${CMAKE_SYSTEM_NAME} PLATFORM_NAME)
if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(PLATFORM_NAME "macosx")
endif()
message("Current platform name: ${PLATFORM_NAME}")
execute_process(COMMAND make config=release
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/SOIL2/make/${PLATFORM_NAME})

# Set path to sources and library
set(SOIL2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SOIL2/src)
set(SOIL2_LIBRARY     ${CMAKE_CURRENT_SOURCE_DIR}/SOIL2/lib/${PLATFORM_NAME}/libsoil2.a)

# Include sources into project
include_directories(${SOIL2_INCLUDE_DIR})

### !SOIL2

add_executable(${CMAKE_PROJECT_NAME}
        main.cpp
        Shader.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}
        OpenGL::GL
        glfw
        GLEW::glew
        ${GLM_LIBRARY}
        ${SOIL2_LIBRARY})

# Copy resources into CMake binary directory
FILE(COPY Shaders DESTINATION "${CMAKE_BINARY_DIR}")
FILE(COPY Images DESTINATION "${CMAKE_BINARY_DIR}")
