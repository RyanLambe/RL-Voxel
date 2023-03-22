#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <iostream>

namespace Engine {
	class Window
	{
	public:

		~Window();

		bool Start(std::string name, int width, int height, GLFWframebuffersizefun ResizeCallback);
		bool Run();
		
		static GLFWwindow* GetWindow();
		
	private:

		static GLFWwindow* window;
	};
}

