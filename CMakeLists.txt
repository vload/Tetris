cmake_minimum_required(VERSION 3.5)


project(
    GL3
    VERSION 0.0.1.1
    DESCRIPTION "The GL trials continue onwards")
    
set(CMAKE_CXX_STANDARD 20)

# Set the path to dependencies folders
set(GLFW ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glfw)
set(GLAD ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad)
set(GLM ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glm)
set(IMGUI ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui)


# Add the include directory
include_directories(include)

# Add the executable with all sources from src
file(GLOB SOURCES "src/*.cpp")
add_executable(main ${SOURCES})

# Add GLFW
add_subdirectory(${GLFW})
include_directories(${GLFW}/include)
target_link_libraries(main glfw)

# Add the include directory for GLAD
include_directories(${GLAD}/include)
# Add the glad.c file to the sources
target_sources(main PRIVATE ${GLAD}/src/glad.c)

# Add the include directory for GLM
include_directories(${GLM})

# Add the sources for ImGui
file(GLOB IMGUI_SOURCES ${IMGUI}/*.cpp ${IMGUI}/backends/imgui_impl_glfw.cpp ${IMGUI}/backends/imgui_impl_opengl3.cpp)
file(GLOB IMGUI_HEADERS ${IMGUI}/*.h ${IMGUI}/backends/imgui_impl_glfw.h ${IMGUI}/backends/imgui_impl_opengl3.h)
target_sources(main PUBLIC ${IMGUI_HEADERS})
target_sources(main PUBLIC ${IMGUI_SOURCES})
include_directories(${IMGUI})

# Copy shaders to the place where the executable is (cmake wizardry)
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:main>/shaders)