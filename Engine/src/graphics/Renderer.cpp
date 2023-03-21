#include "Renderer.h"

bool Engine::Renderer::Start(std::string windowName, int width, int height)
{
    screenScale = glm::vec2(width, height);
    if (!window.Start(windowName, width, height))
        return false;
    return graphics.Start();
}

bool Engine::Renderer::Render()
{
    camera->UpdateUniformVariables(graphics.GetShaderProgram(), screenScale);

    return window.Run();
}

void Engine::Renderer::SetActiveCamera(Camera* newCam)
{
    camera = newCam;
}

GLFWwindow* Engine::Renderer::GetWindow()
{
    return window.GetWindow();
}
