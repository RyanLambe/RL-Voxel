#include "Renderer.h"

using namespace Engine;

Camera* Renderer::camera = nullptr;

bool Engine::Renderer::Start(std::string windowName, int width, int height)
{
    if (!window.Start(windowName, width, height, ResizeCallback))
        return false;

    windowSize = glm::vec2(width, height);
    return graphics.Start();
}

bool Engine::Renderer::Render()
{
    camera->UpdateUniformVariables(graphics.GetShaderProgram());
    camera->Render(graphics.GetShaderProgram());

    return window.Run();
}

void Engine::Renderer::SetActiveCamera(Camera* newCam)
{
    camera = newCam;
    camera->Enable(windowSize);
}

void Engine::Renderer::ResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    if (camera)
        camera->Resize(glm::vec2(width, height));
}
