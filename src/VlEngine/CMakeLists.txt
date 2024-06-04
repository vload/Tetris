file(GLOB VLENGINE_SOURCES "*.cpp")

add_library(VlEngine ${VLENGINE_SOURCES})

# Add GLFW
target_include_directories(VlEngine PUBLIC ${GLFW}/include)
target_link_libraries(VlEngine PUBLIC glfw)

# Add the include directory for GLAD
target_include_directories(VlEngine PUBLIC ${GLAD}/include)
# Add the glad.c file to the sources
target_sources(VlEngine PUBLIC ${GLAD}/src/glad.c)

# Add the include directory for GLM
target_include_directories(VlEngine PUBLIC ${GLM})

# Add the sources for ImGui
file(GLOB IMGUI_SOURCES ${IMGUI}/*.cpp ${IMGUI}/backends/imgui_impl_glfw.cpp ${IMGUI}/backends/imgui_impl_opengl3.cpp)
file(GLOB IMGUI_HEADERS ${IMGUI}/*.h ${IMGUI}/backends/imgui_impl_glfw.h ${IMGUI}/backends/imgui_impl_opengl3.h)
target_sources(VlEngine PUBLIC ${IMGUI_HEADERS})
target_sources(VlEngine PUBLIC ${IMGUI_SOURCES})
target_include_directories(VlEngine PUBLIC ${IMGUI})

# Add the include directory for STB
target_include_directories(VlEngine PUBLIC ${STB})