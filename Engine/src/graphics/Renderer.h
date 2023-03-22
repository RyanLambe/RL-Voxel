#pragma once

#include "Window.h"
#include "Graphics.h"
#include "Camera.h"

namespace Engine {
	class Renderer
	{
	public:

		bool Start(std::string windowName, int width, int height);
		bool Render();

		void SetActiveCamera(Camera* newCam);

	private:

		static void ResizeCallback(GLFWwindow* window, int width, int height);

		Window window;
		Graphics graphics;

		glm::vec2 windowSize = glm::vec2(0);

		static Camera* camera;
	};
}