#include "Window.h"

using namespace Engine;

GLFWwindow* Window::window = nullptr;

Engine::Window::~Window() {
    glfwTerminate();
}

bool Engine::Window::Start(std::string name, int width, int height, GLFWframebuffersizefun ResizeCallback)
{
    if (window) {
        std::cout << "A window has already been created.\n";
        return false;
    }

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

    //unlock framerate
    //glfwSwapInterval(0);

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
        glfwSwapBuffers(window);
        glfwPollEvents();

        return true;
    }
    
	return false;
}

GLFWwindow* Engine::Window::GetWindow() {
    return window;
}