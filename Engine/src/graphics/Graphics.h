#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#include <wrl.h>
#include <vector>

#include "../display/Debug.h"
#include "../math/Vec2.h"

namespace Engine {
	class Graphics
	{
	public:

		// Run Graphics
		bool Start(HWND hwnd, int width, int height);
		bool EndFrame();

		// Window Management
		static Math::Vec2 getDimensions();
		static bool setDimensions(Math::Vec2 dimensions);
		static bool setFullscreen(bool fullscreen);

		// Use DirectX functions
		ID3D11Device* getDevice();
		ID3D11DeviceContext* getContext();

	private:

		bool setupPipeline(HWND hwnd, int width, int height);
		bool setupShaders();

		// Instances
		static Graphics* mainGraphics;
		HWND hwnd;

		// Dimensions
		static Math::Vec2 dimensions;
		Math::Vec2 windowedDimensions;

		// DirectX
		Microsoft::WRL::ComPtr<IDXGISwapChain> swap = Microsoft::WRL::ComPtr<IDXGISwapChain>();
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target = Microsoft::WRL::ComPtr<ID3D11RenderTargetView>();

		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = Microsoft::WRL::ComPtr<ID3D11DeviceContext>();
		Microsoft::WRL::ComPtr<ID3D11Device> device = Microsoft::WRL::ComPtr<ID3D11Device>();
	};
}
