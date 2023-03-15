#include "Renderer.h"

using namespace Engine;

bool Renderer::Start(HWND hwnd, int width, int height)
{
	graphics.Start(hwnd, width, height);
	return true;
}

void Renderer::Draw()
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> psBuffer;

	D3D11_BUFFER_DESC bufferDesc1;
	bufferDesc1.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc1.ByteWidth = 16;
	bufferDesc1.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc1.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc1.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData1;
	initData1.pSysMem = &camPos;
	initData1.SysMemPitch = 0;
	initData1.SysMemSlicePitch = 0;

	//create buffer
	logErrorCode(graphics.getDevice()->CreateBuffer(&bufferDesc1, &initData1, &psBuffer));
	graphics.getContext()->PSSetConstantBuffers(0, 1, psBuffer.GetAddressOf());


	struct Vertex {
		float x;
		float y;

		Vertex(float x, float y) {
			this->x = x;
			this->y = y;
		}
	};
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	vertices.emplace_back(Vertex(-1, -1));
	vertices.emplace_back(Vertex(-1, 1));
	vertices.emplace_back(Vertex(1, -1));
	vertices.emplace_back(Vertex(1, 1));
	vertices.emplace_back(Vertex(1, 1));

	indices.emplace_back(0);
	indices.emplace_back(1);
	indices.emplace_back(2);
	indices.emplace_back(1);
	indices.emplace_back(3);
	indices.emplace_back(2);

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = (UINT)(sizeof(Vertex) * vertices.size());
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = vertices.data();
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	logErrorCode(graphics.getDevice()->CreateBuffer(&bufferDesc, &initData, &vertexBuffer));

	bufferDesc.ByteWidth = (UINT)(sizeof(unsigned int) * indices.size());
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	initData.pSysMem = indices.data();

	logErrorCode(graphics.getDevice()->CreateBuffer(&bufferDesc, &initData, &indexBuffer));

	UINT offset = 0;
	UINT stride = sizeof(Vertex);
	graphics.getContext()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	graphics.getContext()->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	graphics.getContext()->DrawIndexed(6, 0, 0);
	graphics.EndFrame();
}
