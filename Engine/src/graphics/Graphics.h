#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

namespace Engine {
	class Graphics
	{
	public:

		bool Start();

		unsigned int GetShaderProgram();

	private:

		void SetupScreenRect();
		bool SetupShaders();

		// Screen Rect
		float vertices[12] = {
			-1, -1,
			 1, -1,
			-1,  1,

			 1,  1,
			-1,  1,
			 1, -1
		};

		// Shaders
		unsigned int shaderProgram;
		std::string vertexShaderSource;
		std::string fragmentShaderSource;

		bool ReadShader(std::string filename, std::string* shaderLocation);
	};
}
