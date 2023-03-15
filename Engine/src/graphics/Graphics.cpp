#include "Graphics.h"

using namespace Engine;
using namespace Math;

Vec2 Graphics::dimensions = Vec2(0);
Graphics* Graphics::mainGraphics = nullptr;

bool Graphics::Start(HWND hwnd, int width, int height)
{
	if (mainGraphics != nullptr) {
		logError("Graphics have already been started.");
		return false;
	}
	mainGraphics = this;
	this->hwnd = hwnd;

	return setupPipeline(hwnd, width, height);
}

bool Graphics::EndFrame()
{
	HRESULT hr;
	if (FAILED(hr = swap->Present(1, 0))) {

		if (hr == DXGI_ERROR_DEVICE_REMOVED) {
			logErrorCode(device->GetDeviceRemovedReason());
			return false;
		}
		else {
			logErrorCode(hr);
			return false;
		}
	}

	float bgColour[4] = { 1, 155.0f / 255.0f, 0, 1 };
	context->ClearRenderTargetView(target.Get(), bgColour);

	return true;
}

Vec2 Graphics::getDimensions()
{
	return dimensions;
}

bool Graphics::setDimensions(Vec2 dimensions)
{
	//save new dimensions
	mainGraphics->dimensions = dimensions;

	//resize target
	DXGI_MODE_DESC dxgiDesc = {};
	dxgiDesc.Width = (UINT)dimensions.x;
	dxgiDesc.Height = (UINT)dimensions.y;
	dxgiDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	dxgiDesc.RefreshRate.Numerator = 1000;
	dxgiDesc.RefreshRate.Denominator = 1;
	dxgiDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	dxgiDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	HRESULT hr = mainGraphics->swap->ResizeTarget(&dxgiDesc);
	logErrorCode(hr);
	if (FAILED(hr))
		return false;

	//set target to new scaled target
	mainGraphics->context->OMSetRenderTargets(1, mainGraphics->target.GetAddressOf(), nullptr);
	return true;
}

bool Graphics::setFullscreen(bool fullscreen)
{
	if (fullscreen) {
		mainGraphics->windowedDimensions = getDimensions();

		HMONITOR hMonitor = MonitorFromWindow(mainGraphics->hwnd, MONITOR_DEFAULTTONEAREST);
		MONITORINFO monitorInfo;
		monitorInfo.cbSize = sizeof(MONITORINFO);
		if (!GetMonitorInfo(hMonitor, &monitorInfo))
			return false;

		HRESULT hr = mainGraphics->swap->SetFullscreenState(true, nullptr);
		logErrorCode(hr);
		if (FAILED(hr))
			return false;

		return setDimensions(Vec2((float)(monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left), (float)(monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top)));
	}
	else {
		HRESULT hr = mainGraphics->swap->SetFullscreenState(false, nullptr);
		logErrorCode(hr);
		if (FAILED(hr))
			return false;

		return setDimensions(mainGraphics->windowedDimensions);
	}
}

bool Engine::Graphics::setupPipeline(HWND hwnd, int width, int height)
{
	//setup data for swap chain
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 1000;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hwnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	UINT swapCreateFlags = 0u;
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;


	//create device, context and swapchain
	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, swapCreateFlags, nullptr, 0, D3D11_SDK_VERSION, &sd, &swap, &device, nullptr, &context);
	logErrorCode(hr);
	if (FAILED(hr))
		return false;


	// get frame buffer to create render target
	Microsoft::WRL::ComPtr<ID3D11Texture2D> frameBuffer;
	hr = swap->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&frameBuffer);
	logErrorCode(hr);
	if (FAILED(hr))
		return false;


	//create render target
	hr = device->CreateRenderTargetView(frameBuffer.Get(), 0, &target);
	logErrorCode(hr);
	if (FAILED(hr))
		return false;


	//set size of window
	windowedDimensions = Vec2((float)width, (float)height);
	setDimensions(windowedDimensions);


	//set viewport size
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	context->RSSetViewports(1, &vp);


	//set topology
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//setup shader then return
	return setupShaders();
}

bool Engine::Graphics::setupShaders()
{
	//setup com pointers
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	Microsoft::WRL::ComPtr<ID3DBlob> blob;

	HRESULT hr;


	//pixel shader
	hr = D3DReadFileToBlob(L"../bin/Debug/PixelShader.cso", &blob);
	logErrorCode(hr);
	if (FAILED(hr))
		return false;

	hr = device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);
	logErrorCode(hr);
	if (FAILED(hr))
		return false;

	context->PSSetShader(pixelShader.Get(), 0, 0);



	//vertex shader
	hr = D3DReadFileToBlob(L"../bin/Debug/VertexShader.cso", &blob);
	logErrorCode(hr);
	if (FAILED(hr))
		return false;

	hr = device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);
	logErrorCode(hr);
	if (FAILED(hr))
		return false;

	context->VSSetShader(vertexShader.Get(), 0, 0);



	//input Layout
	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = device->CreateInputLayout(ied, sizeof(ied) / sizeof(D3D11_INPUT_ELEMENT_DESC), blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout);
	logErrorCode(hr);
	if (FAILED(hr))
		return false;

	context->IASetInputLayout(inputLayout.Get());

	return true;
}

ID3D11Device* Engine::Graphics::getDevice()
{
	return device.Get();
}

ID3D11DeviceContext* Engine::Graphics::getContext()
{
	return context.Get();
}