#include "Renderer.h"

using namespace Engine;
using namespace Math;

bool Renderer::Start(HWND hwnd, int width, int height)
{
	graphics.Start(hwnd, width, height);

	return true;
}

void Renderer::Draw()
{
	updateCameraBuffer();
	updateLightBuffer();
	updateWorldBuffer();

	updateScreenPlane();
}

void Engine::Renderer::setActiveCamera(Camera* newCam)
{
	activeCamera = newCam;
}

bool Renderer::updateScreenPlane()
{
	if (!setScreenPlane) {
		
		//set screen plane data
		std::vector<Vec2> vertices;
		std::vector<unsigned int> indices;

		vertices.emplace_back(Vec2(-1, -1));
		vertices.emplace_back(Vec2(-1, 1));
		vertices.emplace_back(Vec2(1, -1));
		vertices.emplace_back(Vec2(1, 1));

		indices.emplace_back(0);
		indices.emplace_back(1);
		indices.emplace_back(2);
		indices.emplace_back(1);
		indices.emplace_back(3);
		indices.emplace_back(2);


		//create vertex and index buffers
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = (UINT)(sizeof(Vec2) * vertices.size());
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = vertices.data();
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		HRESULT hr = graphics.getDevice()->CreateBuffer(&bufferDesc, &initData, &vertexBuffer);
		logErrorCode(hr);
		if (FAILED(hr))
			return false;

		bufferDesc.ByteWidth = (UINT)(sizeof(unsigned int) * indices.size());
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		initData.pSysMem = indices.data();

		hr = graphics.getDevice()->CreateBuffer(&bufferDesc, &initData, &indexBuffer);
		logErrorCode(hr);
		if (FAILED(hr))
			return false;

		//set vertex and index buffers
		UINT offset = 0;
		UINT stride = sizeof(Vec2);
		graphics.getContext()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
		graphics.getContext()->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);


		//dont repeat
		setScreenPlane = true;
	}
	
	//draw to screen
	graphics.getContext()->DrawIndexed(6, 0, 0);
	return graphics.EndFrame();
}

bool Engine::Renderer::updateCameraBuffer()
{
	if (!cameraBufferCreated) {
		
		//set buffer data
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = sizeof(Camera::CameraBufferData);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = activeCamera->getBufferData();
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;


		//create buffer
		HRESULT hr = graphics.getDevice()->CreateBuffer(&bufferDesc, &initData, &cameraBuffer);
		logErrorCode(hr);
		if (FAILED(hr))
			return false;

		cameraBufferCreated = true;
	}
	else {

		//get location of buffer
		D3D11_MAPPED_SUBRESOURCE mappedSubResource;
		memset(&mappedSubResource, 0, sizeof(mappedSubResource));
		
		HRESULT hr = graphics.getContext()->Map(cameraBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
		logErrorCode(hr);
		if (FAILED(hr))
			return false;

		//copy new data
		memcpy(mappedSubResource.pData, activeCamera->getBufferData(), sizeof(Camera::CameraBufferData));


		//cleanup
		graphics.getContext()->Unmap(cameraBuffer.Get(), 0);
	}

	graphics.getContext()->PSSetConstantBuffers(0, 1, cameraBuffer.GetAddressOf());
	return true;
}

bool Engine::Renderer::updateLightBuffer()
{
	return false;
}

bool Engine::Renderer::updateWorldBuffer()
{
	return false;
}
