#include "Engine.h"

// Shaders for WebGL 2.0 (GLES 3.0)
const char* vertexShaderSource = "#version 300 es\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 vColor;\n"
    "uniform mat4 u_MVP;\n"
    "void main() {\n"
    "   gl_Position = u_MVP * vec4(aPos, 1.0);\n"
    "   vColor = aColor;\n"
    "}\0";

const char* fragmentShaderSource = "#version 300 es\n"
    "precision highp float;\n"
    "in vec3 vColor;\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "   FragColor = vec4(vColor, 1.0);\n"
    "}\n\0";

Engine::Engine() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

    window = SDL_CreateWindow("Aswin2D Web - Spinning Cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);

    InitShaders();
    InitBuffers();

    glEnable(GL_DEPTH_TEST);
    lastCounter = SDL_GetPerformanceCounter();

    MainLoop = [this]() {
        // Delta Time Calculation
        Uint64 currentCounter = SDL_GetPerformanceCounter();
        double deltaTime = (double)(currentCounter - lastCounter) / SDL_GetPerformanceFrequency();
        lastCounter = currentCounter;

        // FPS Counter logic
        fpsTimer += deltaTime;
        frameCount++;
        if (fpsTimer >= 1.0) {
            std::cout << "FPS: " << (int)(frameCount / fpsTimer) << std::endl;
            frameCount = 0; fpsTimer = 0.0;
        }

        // Update Rotation
        rotationAngle += 60.0f * (float)deltaTime;

        // Render
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Matrix Math (MVP)
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), glm::vec3(0.5f, 1.0f, 0.0f));
        glm::mat4 view  = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 proj  = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 mvp   = proj * view * model;

        GLint mvpLoc = glGetUniformLocation(shaderProgram, "u_MVP");
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
    };
}

void Engine::InitShaders() {
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSource, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSource, NULL);
    glCompileShader(fs);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);
}

void Engine::InitBuffers() {
    float vertices[] = {
        // Position           // Color (RGB)
        -0.5f, -0.5f,  0.5f,  1.0f, 0.5f, 0.2f, // Front
         0.5f, -0.5f,  0.5f,  1.0f, 0.5f, 0.2f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.5f, 0.2f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.5f, 0.2f,
        -0.5f, -0.5f, -0.5f,  0.2f, 0.5f, 1.0f, // Back
         0.5f, -0.5f, -0.5f,  0.2f, 0.5f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.2f, 0.5f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.2f, 0.5f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1, 7, 6, 5, 5, 4, 7,
        4, 0, 3, 3, 7, 4, 3, 2, 6, 6, 7, 3, 4, 5, 1, 1, 0, 4
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position (Location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color (Location 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

Engine::~Engine() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}