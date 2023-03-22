#include "graphics/Window.h"
#include "graphics/Renderer.h"

#include <string>

using namespace Engine;

std::string name = "Voxel Engine";
int width = 1600;
int height = 900;

int main()
{
    //create renderer
    Renderer renderer;
    if (!renderer.Start(name, width, height))
        return -1;

    //create camera
    Camera cam;
    renderer.SetActiveCamera(&cam);

    //run application
    while (renderer.Render()) {

        //close on escape
        if (glfwGetKey(Window::GetWindow(), GLFW_KEY_ESCAPE))
            break;

		//move the camera
		glm::vec3 move = glm::vec3(0, 0, 0);
		if (glfwGetKey(Window::GetWindow(), GLFW_KEY_W))
			move.z += 0.1f;
		if (glfwGetKey(Window::GetWindow(), GLFW_KEY_A))
			move.x -= 0.1f;
		if (glfwGetKey(Window::GetWindow(), GLFW_KEY_S))
			move.z -= 0.1f;
		if (glfwGetKey(Window::GetWindow(), GLFW_KEY_D))
			move.x += 0.1f;
		if (glfwGetKey(Window::GetWindow(), GLFW_KEY_Q))
			move.y -= 0.1f;
		if (glfwGetKey(Window::GetWindow(), GLFW_KEY_E))
			move.y += 0.1f;
		cam.Translate(move);

		//rotate the camera
		glm::vec3 rot = glm::vec3(0, 0, 0);
		if (glfwGetKey(Window::GetWindow(), GLFW_KEY_I))
			rot.x -= 0.01f;
		if (glfwGetKey(Window::GetWindow(), GLFW_KEY_J))
			rot.y -= 0.01f;
		if (glfwGetKey(Window::GetWindow(), GLFW_KEY_K))
			rot.x += 0.01f;
		if (glfwGetKey(Window::GetWindow(), GLFW_KEY_L))
			rot.y += 0.01f;
		if (glfwGetKey(Window::GetWindow(), GLFW_KEY_O))
			rot.z += 0.01f;
		if (glfwGetKey(Window::GetWindow(), GLFW_KEY_U))
			rot.z -= 0.01f;
		cam.Rotate(rot);
    }
    
    return 0;
}