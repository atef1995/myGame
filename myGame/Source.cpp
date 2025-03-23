#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Utility function to load shader source code from file
std::string loadShaderSource(const std::string& filepath) {
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Compile shader source (vertex or fragment)
GLuint compileShader(const char* shaderSource, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR: SHADER COMPILATION FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

//int main(int argc, char* argv[])
//{
//    const int WIDTH = 800;
//    const int HEIGHT = 600;
//
//    // Initialize SDL with video subsystem
//    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//        std::cout << "Error: " << SDL_GetError();
//        return -1;
//    }
//    else {
//        std::cout << "SDL initialized successfully\n";
//    }
//
//    // Set OpenGL version (4.1 Core Profile)
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
//
//    // Create SDL Window with OpenGL flag
//    SDL_Window* window = SDL_CreateWindow("SDL3 + OpenGL Window", WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
//    if (window == NULL) {
//        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
//        return 1;
//    }
//
//    // Create OpenGL context
//    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
//    if (!glcontext) {
//        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create OpenGL context");
//        return 1;
//    }
//
//    // Load OpenGL functions using glad
//    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
//        std::cerr << "Failed to initialize GLAD" << std::endl;
//        return 1;
//    }
//
//    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
//
//    // Load and compile shaders
//    std::string vertexShaderSource = loadShaderSource("vertexShader.glsl");
//    std::string fragmentShaderSource = loadShaderSource("fragmentShader.glsl");
//
//    GLuint vertexShader = compileShader(vertexShaderSource.c_str(), GL_VERTEX_SHADER);
//    GLuint fragmentShader = compileShader(fragmentShaderSource.c_str(), GL_FRAGMENT_SHADER);
//
//    // Link shaders into a program
//    GLuint shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    // Check linking errors
//    GLint success;
//    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//    if (!success) {
//        char infoLog[512];
//        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//        std::cout << "ERROR: PROGRAM LINKING FAILED\n" << infoLog << std::endl;
//    }
//
//    // Cleanup individual shaders
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    // Triangle vertices (3D positions)
//    float vertices[] = {
//        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // Bottom left red
//         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // Bottom right green
//         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // Top center blue
//    };
//
//    // Create VAO (Vertex Array Object)
//    GLuint VAO;
//    glGenVertexArrays(1, &VAO);
//    glBindVertexArray(VAO);
//
//    // Create VBO (Vertex Buffer Object)
//    GLuint VBO;
//    glGenBuffers(1, &VBO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    // Set vertex Position attribute pointers
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    // Color attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//    // Unbind VBO and VAO (optional but clean)
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//
//    // Enable V-Sync
//    SDL_GL_SetSwapInterval(1);
//
//    // Main game loop
//    bool gameIsRunning = true;
//    while (gameIsRunning) {
//        SDL_Event event;
//
//        // Event polling
//        while (SDL_PollEvent(&event)) {
//            if (event.type == SDL_EVENT_QUIT) {
//                gameIsRunning = false;
//            }
//
//            if (event.type == SDL_EVENT_KEY_DOWN) {
//                if (event.key.key == SDLK_ESCAPE) {
//                    std::cout << "ESC pressed, exiting...\n";
//                    gameIsRunning = false;
//                }
//            }
//        }
//
//        // Rendering:
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Clear color
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        // Use shader & draw triangle
//        glUseProgram(shaderProgram);
//        glBindVertexArray(VAO);
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//
//        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::rotate(model, (float)SDL_GetTicks() * 0.001f, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate over time
//
//        GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//
//
//        // Swap buffers
//        SDL_GL_SwapWindow(window);
//    }
//
//    //// Cleanup OpenGL resources
//    //glDeleteVertexArrays(1, &VAO);
//    //glDeleteBuffers(1, &VBO);
//    //SDL_GL_DestroyContext(glcontext);
//    //SDL_DestroyWindow(window);
//    //SDL_Quit();
//
//    return 0;
//}
