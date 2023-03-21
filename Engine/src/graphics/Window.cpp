#include "Window.h"

using namespace Engine;

Engine::Window::~Window() {
    glfwTerminate();
}

bool Engine::Window::Start(std::string name, int width, int height)
{
    //start glfw
    if (!glfwInit()) {
        std::cout << "Could not innitialize GLFW.\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //create window
    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    if (!window)
    {
        std::cout << "Could not create GLFW Window.\n";
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    //start glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Could not innitialize GLAD.\n";
        return false;
    }

    //set viewport
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, ResizeCallback);

    return true;
}

bool Engine::Window::Run()
{
    if (window == nullptr)
        return false;

    if (!glfwWindowShouldClose(window))
    {
        //render
        glClearColor(1, 0.6f, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        //next frame
        glfwSwapBuffers(window);
        glfwPollEvents();

        return true;
    }
    
	return false;
}

GLFWwindow* Engine::Window::GetWindow() {
    return window;
}

//callback functions
void Engine::Window::ResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}