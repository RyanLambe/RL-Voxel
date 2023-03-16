#pragma once

#include <directxmath.h>

#include "Graphics.h"

#define PI 3.14159f

namespace Engine {
	class Camera
	{
	public:

		//transform values
		Math::Vec3 position = Math::Vec3(0, 0, -5);
		Math::Vec3 rotation = Math::Vec3(0, 0, 0);

		//directions
		Math::Vec3 foreward();
		Math::Vec3 right();
		Math::Vec3 up();

		//offset values
		void Translate(Math::Vec3 offset);
		void Translate(float x, float y, float z);

		void Rotate(Math::Vec3 offset);
		void Rotate(float x, float y, float z);

		//properties
		float fov = 60;

		//constant buffer data
		struct CameraBufferData {
			DirectX::XMVECTOR position;
			DirectX::XMMATRIX rotation;

			DirectX::XMVECTOR screenData;
		};
		CameraBufferData* getBufferData();

	private:
		CameraBufferData lastBuffer;

	};
}


