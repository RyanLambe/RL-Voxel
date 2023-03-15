#include "display/Window.h"
#include "graphics/Renderer.h"
#include <locale>
#include <codecvt>

using namespace Display;

std::wstring name = L"Voxel Engine";
int width = 1280;
int height = 720;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	
	//create window
	Window window = Window();
	HWND hwnd = window.Start(hInstance, name, WS_OVERLAPPEDWINDOW, width, height, true);

	if (hwnd == nullptr)
		return -1;


	//setup graphics
	Engine::Renderer renderer;
	if (!renderer.Start(hwnd, width, height))
		return -1;


	//run code
	ExitCode code;
	while (!(code = window.WindowClosed()).windowClosed) {
		if (Input::getKey('W'))
			renderer.camPos.z += 0.1f;
		if (Input::getKey('A'))
			renderer.camPos.x -= 0.1f;
		if (Input::getKey('S'))
			renderer.camPos.z -= 0.1f;
		if (Input::getKey('D'))
			renderer.camPos.x += 0.1f;
		if (Input::getKey('Q'))
			renderer.camPos.y += 0.1f;
		if (Input::getKey('E'))
			renderer.camPos.y -= 0.1f;


		renderer.Draw();
	}


	//exit
	return code.exitCode;
}