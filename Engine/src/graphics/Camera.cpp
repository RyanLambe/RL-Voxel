#include "Camera.h"

using namespace Engine;
using namespace Math;

Camera::CameraBufferData* Camera::getBufferData()
{
	//position
	lastBuffer.position = DirectX::XMVectorSet(position.x, position.y, position.z, 0.0f);

	//rotation
	lastBuffer.rotation = DirectX::XMMatrixRotationRollPitchYaw(rotation.x * PI / 180.0f, rotation.y * PI / 180.0f, rotation.z * PI / 180.0f);

	//screen data
	Vec2 scale = Graphics::getDimensions();
	lastBuffer.screenData = DirectX::XMVectorSet(scale.x, scale.y, fov, 0.0f);

	return &lastBuffer;
}

Vec3 Camera::foreward()
{
	DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYaw(rotation.x * PI / 180.0f, rotation.y * PI / 180.0f, rotation.z * PI / 180.0f);

	DirectX::XMVECTOR out = DirectX::XMVector3Normalize(DirectX::XMVector3Transform(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotMat));

	return Vec3(DirectX::XMVectorGetX(out), DirectX::XMVectorGetY(out), DirectX::XMVectorGetZ(out));
}

Vec3 Camera::right()
{
	DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYaw(rotation.x * PI / 180.0f, rotation.y * PI / 180.0f, rotation.z * PI / 180.0f);

	DirectX::XMVECTOR out = DirectX::XMVector3Normalize(DirectX::XMVector3Transform(DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), rotMat));

	return Vec3(DirectX::XMVectorGetX(out), DirectX::XMVectorGetY(out), DirectX::XMVectorGetZ(out));
}

Vec3 Camera::up()
{
	DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYaw(rotation.x * PI / 180.0f, rotation.y * PI / 180.0f, rotation.z * PI / 180.0f);

	DirectX::XMVECTOR out = DirectX::XMVector3Normalize(DirectX::XMVector3Transform(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotMat));
	
	return Vec3(DirectX::XMVectorGetX(out), DirectX::XMVectorGetY(out), DirectX::XMVectorGetZ(out));
}

void Engine::Camera::Translate(Math::Vec3 offset)
{
	this->position += right() * offset.x + up() * offset.y + foreward() * offset.z;
}

void Engine::Camera::Translate(float x, float y, float z)
{
	this->position += right() * x + up() * y + foreward() * z;
}

void Engine::Camera::Rotate(Math::Vec3 offset)
{
	this->rotation += offset;
}

void Engine::Camera::Rotate(float x, float y, float z)
{
	this->rotation.x += x;
	this->rotation.y += y;
	this->rotation.z += z;
}
