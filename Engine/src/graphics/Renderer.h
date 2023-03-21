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

		GLFWwindow* GetWindow();

	private:

		Window window;
		Graphics graphics;

		glm::vec2 screenScale;


		Camera* camera;

	};
}