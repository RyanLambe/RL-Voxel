#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <iostream>

namespace Engine {
	class Window
	{
	public:

		~Window();

		bool Start(std::string name, int width, int height);
		bool Run();
		
		GLFWwindow* GetWindow();
		
	private:

		GLFWwindow* window = nullptr;
		

		//window callbacks
		static void ResizeCallback(GLFWwindow* window, int width, int height);
	};
}

