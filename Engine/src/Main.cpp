#include "../include/Window.h"

using namespace Engine;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	
	Window window = Window();

	window.Start(hInstance, L"Voxel Engine", WS_OVERLAPPEDWINDOW, 1280, 720, true);

	Window::ExitCode code;
	while (!(code = window.Run()).windowClosed);

	return code.exitCode;
}