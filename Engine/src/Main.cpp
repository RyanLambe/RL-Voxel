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

	//create camera
	Engine::Camera cam;
	renderer.setActiveCamera(&cam);

	//run code
	ExitCode code;
	while (!(code = window.WindowClosed()).windowClosed) {
		
		//move the camera
		Math::Vec3 move;
		if (Input::getKey('W'))
			move.z += 0.1f;
		if (Input::getKey('A'))
			move.x -= 0.1f;
		if (Input::getKey('S'))
			move.z -= 0.1f;
		if (Input::getKey('D'))
			move.x += 0.1f;
		if (Input::getKey('Q'))
			move.y -= 0.1f;
		if (Input::getKey('E'))
			move.y += 0.1f;

		cam.Translate(move);

		//rotate the camera
		Math::Vec3 rot;
		if (Input::getKey('I'))
			rot.x -= 0.5f;
		if (Input::getKey('J'))
			rot.y -= 0.5f;
		if (Input::getKey('K'))
			rot.x += 0.5f;
		if (Input::getKey('L'))
			rot.y += 0.5f;
		if (Input::getKey('O'))
			rot.z += 0.5f;
		if (Input::getKey('U'))
			rot.z -= 0.5f;
		cam.Rotate(rot);

		renderer.Draw();
	}


	//exit
	return code.exitCode;
}