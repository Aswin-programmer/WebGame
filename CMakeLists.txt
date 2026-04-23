cmake_minimum_required(VERSION 3.20)
project(WebGame CXX)

set(CMAKE_CXX_STANDARD 20)

# This ensures the output is always a runnable HTML page
set(CMAKE_EXECUTABLE_SUFFIX ".html")

add_subdirectory(DEPENDENCIES/GLM)

add_executable(WebGame main.cpp
        Engine.cpp
        Engine.h)

# CRITICAL: These flags link the JS-to-WebGL bridge
target_link_options(WebGame PUBLIC
        "--bind"
        "-sUSE_SDL=2"
        "-sFULL_ES3=1"          # Links OpenGL ES 3.0 headers
        "-sUSE_WEBGL2=1"       # Forces WebGL 2.0 context
        "-sMIN_WEBGL_VERSION=2"
        "-sMAX_WEBGL_VERSION=2"
        "-sALLOW_MEMORY_GROWTH=1"
        "-o index.html"          # Force the output name to index.html
)

target_link_libraries(WebGame PUBLIC glm::glm-header-only)