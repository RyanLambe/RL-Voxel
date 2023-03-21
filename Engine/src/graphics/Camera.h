#pragma once

#define PI 3.14159265389f

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <iostream>
#include <string>

namespace Engine {
	class Camera
	{
	public:

		//transformation values
		
		glm::vec3 position = glm::vec3(0, 0, -5);
		glm::vec3 rotation = glm::vec3(0, 0, 0);

		//directions
		glm::vec3 foreward();
		glm::vec3 right();
		glm::vec3 up();

		//offset values
		void Translate(glm::vec3 offset);
		void Translate(float x, float y, float z);

		void Rotate(glm::vec3 offset);
		void Rotate(float x, float y, float z);

		//properties
		float fov = 60;

		//open gl
		void UpdateUniformVariables(unsigned int shaderProgram, glm::vec2 screenScale);

	private:

		void SetRotationMatrix();
		glm::mat4 rotationMat;

	};
}