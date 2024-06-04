cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)

project(
    Tetris
    VERSION 0.0.2.2
    DESCRIPTION "A simple tetris game written with C++ and OpenGL.")

# Enable warnings and pedantic flags for various compilers
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall        # Enable most warning messages
        -Wextra      # Enable extra warning messages
        -pedantic    # Enable pedantic mode
    )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(
        /W4        # Enable most warnings
    )
endif()

# Setup dependency paths and add subdirectories
set(GLFW ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glfw)
add_subdirectory(${GLFW})
set(GLAD ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad)
set(GLM ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glm)
set(IMGUI ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui)
set(STB ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/stb)

add_subdirectory(src)