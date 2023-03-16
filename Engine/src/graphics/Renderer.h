#pragma once

#include "Graphics.h"
#include "Camera.h"

namespace Engine {
	class Renderer
	{
	public:
		bool Start(HWND hwnd, int width, int height);
		void Draw();

		void setActiveCamera(Camera* newCam);

	private:

		//draws plane covering the screen allowing pixel shader to raytrace each pixel
		bool updateScreenPlane();

		//update constant buffers
		bool updateCameraBuffer();
		bool updateLightBuffer();
		bool updateWorldBuffer();


		//graphics
		Engine::Graphics graphics;


		//buffer data
		Camera* activeCamera = nullptr;
		//LightManager* activeLightManager = nullptr;
		//World* activeWorld = nullptr;


		//buffers
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
		bool setScreenPlane = false;

		Microsoft::WRL::ComPtr<ID3D11Buffer> cameraBuffer;
		bool cameraBufferCreated = false;

		Microsoft::WRL::ComPtr<ID3D11Buffer> lightBuffer;
		bool lightBufferCreated = false;

		Microsoft::WRL::ComPtr<ID3D11Buffer> worldBuffer;
		bool worldBufferCreated = false;
	};
}

