#pragma once

#include "Graphics.h"

namespace Engine {
	class Renderer
	{
	public:
		struct CamPos {
			float x = 0;
			float y = 0;
			float z = -4;
			float w = 0;
		} camPos;

		bool Start(HWND hwnd, int width, int height);

		void Draw();

	private:
		Engine::Graphics graphics;
	};
}

