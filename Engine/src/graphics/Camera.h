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

		Camera();
		~Camera();

		//transformation values
		glm::vec3 position = glm::vec3(0, 0, -5);
		glm::vec3 rotation = glm::vec3(0);

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
		void Enable(glm::vec2 size);

		void Resize(glm::vec2 size);
		void Render(unsigned int shaderProgram);
		void UpdateUniformVariables(unsigned int shaderProgram);

	private:

		//progressive rendering
		glm::vec3 prevPos;
		glm::vec3 prevRot;
		glm::vec2 screenScale = glm::vec2(0);
		unsigned int frameBuffer = 0;
		unsigned int prevFrame = 0;
		int frame = 1;

		//rotation data
		void SetRotationMatrix();
		glm::mat4 rotationMat = glm::mat4(1);

	};
}