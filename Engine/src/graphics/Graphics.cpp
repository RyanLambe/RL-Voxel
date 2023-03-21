#include "Graphics.h"

bool Engine::Graphics::Start()
{
    SetupScreenRect();
    if (!SetupShaders())
        return false;
    return true;
}

unsigned int Engine::Graphics::GetShaderProgram()
{
    return shaderProgram;
}

void Engine::Graphics::SetupScreenRect()
{
    //vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //vertex buffer
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //vertex atributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

bool Engine::Graphics::SetupShaders()
{
    //read shaders from file
    ReadShader("../Engine/shaders/VertexShader.glsl", &vertexShaderSource);
    ReadShader("../Engine/shaders/FragmentShader.glsl", &fragmentShaderSource);
    const char* vertexShaderCode = vertexShaderSource.c_str();
    const char* fragmentShaderCode = fragmentShaderSource.c_str();

    //vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);

    int  success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);

        std::cout << "Could not create vertex shader:\n";
        std::cout << infoLog << "\n";

        return false;
    }


    //fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);

        std::cout << "Could not create fragment shader:\n";
        std::cout << infoLog << "\n";

        return false;
    }


    //shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);

        std::cout << "Could not create shader program:\n";
        std::cout << infoLog << "\n";

        return false;
    }

    glUseProgram(shaderProgram);

    //cleanup
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

bool Engine::Graphics::ReadShader(std::string filename, std::string* shaderLocation)
{
    try {
        std::ifstream file;
        file.open(filename.c_str());

        std::stringstream stream;
        stream << file.rdbuf();
        file.close();

        *shaderLocation = stream.str();
    }
    catch (...) {
        std::cout << "Could not read shader: \"" << filename << "\"\n";
        return false;
    }
    return true;
}