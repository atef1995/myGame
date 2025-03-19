#include <glad/glad.h>
#include <SDL3/SDL.h>
//#include <SDL3/SDL_opengl.h>
#include <iostream>
#include <fstream>
#include <sstream>

std::string loadShaderSource(const std::string& filepath) {
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint compileShader(const char* shaderSource, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR:SHADER::COMPILATION_FAILED\n" << infoLog
            << std::endl;

    }
    
    return shader;
}

int main(int argc, char* argv[])
{
    const int WIDTH = 640;
    const int HEIGHT = 480;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer;
    SDL_Surface* screen;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        std::cout << "Error: " << SDL_GetError();
    }

    else
    {
        std::cout << "SDL video ready";
    }


    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE
    );
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow(
        "Hello SDL", WIDTH, HEIGHT, SDL_WINDOW_OPENGL);

    if (window == NULL) {
        // In the case that the window could not be made...
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }


    screen = SDL_GetWindowSurface(window);

    SDL_Surface* image;
    image = SDL_LoadBMP("images/blender-essential-pack.bmp");
    SDL_BlitSurface(image, NULL, screen, NULL);
    SDL_DestroySurface(image);
    SDL_UpdateWindowSurface(window);

     SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    
    if (!glcontext)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create context");
        return 1;
    }
    
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "Failed to init glad";
        return 1;
    }

    std::cout << "OpnGL version" << glGetString(GL_VERSION) <<
        std::endl;

    std::string vertexShaderSource = loadShaderSource("vertexShader.glsl");
    std::string fragmentShaderSource = loadShaderSource("fragmentShader.glsl");
    
    GLuint vertexShader = compileShader(vertexShaderSource.c_str(), GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentShaderSource.c_str(), GL_FRAGMENT_SHADER);
    
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Error::program::linking:failed\n" << infoLog
            << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,   // Bottom left
         0.5f, -0.5f, 0.0f,   // Bottom right
         0.0f,  0.5f, 0.0f    // Top
    };

    // Create a Vertex Array Object (VAO)
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create a Vertex Buffer Object (VBO) and copy the vertices to it
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set the vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    SDL_GL_SetSwapInterval(1);
     
    
    bool gameIsRunning = true;
    while (gameIsRunning) {
        SDL_Event event;
        
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                gameIsRunning = false;
            }

            if (event.type == SDL_EVENT_MOUSE_MOTION)
            {
                std::cout << "Mouse has been moved";
            }

            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                std::cout << "Key pressed" << std::endl;
                if (event.key.key == 0x0000001bu)
                {
                    std::cout << "esc pressed";
                    gameIsRunning = false;
                }

                if (event.key.key == SDLK_UP)
                {
                    std::cout << "up key pressed";
                }
            }
        }

        
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap the buffers
        SDL_GL_SwapWindow(window);
    }

    
    SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    SDL_GL_DestroyContext(glcontext);

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}