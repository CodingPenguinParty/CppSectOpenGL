cmake_minimum_required(VERSION 3.19)
project(CppSectOpenGL)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "-framework OpenGL")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

add_executable(${CMAKE_PROJECT_NAME}
        main.cpp
        Shader.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}
        OpenGL::GL
        glfw
        GLEW::glew)

FILE(COPY Shaders DESTINATION "${CMAKE_BINARY_DIR}")
