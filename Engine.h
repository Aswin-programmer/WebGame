#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include <SDL2/SDL.h>
#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Engine {
public:
    Engine();
    ~Engine();

    std::function<void()> MainLoop;

private:
    SDL_Window* window = nullptr;
    SDL_GLContext context = nullptr;

    GLuint shaderProgram, VAO, VBO, EBO;

    // Timing and Rotation
    double fpsTimer = 0.0;
    int frameCount = 0;
    Uint64 lastCounter = 0;
    float rotationAngle = 0.0f;

    void InitShaders();
    void InitBuffers();
};