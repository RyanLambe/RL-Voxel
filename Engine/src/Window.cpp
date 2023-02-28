#include "../include/Window.h"

using namespace Engine;

Window* Window::window = nullptr;


bool Window::Start(HINSTANCE hInstance, std::wstring name, DWORD style, int width, int height, bool debugMode) {
	
	if (hwnd != nullptr) {
		Debug::logError("A Window has already been created.");
		return false;
	}
	window = this;

	// Create class for window
	WNDCLASS wClass = {};

	wClass.hInstance = hInstance;
	wClass.lpfnWndProc = WindowProc;
	wClass.lpszClassName = name.c_str();
	
	RegisterClass(&wClass);

	// Create window
	hwnd = CreateWindow(name.c_str(), name.c_str(), style, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, this);

	//check for error
	if (hwnd == NULL) {
		throw std::exception(Debug::TranslateHResult(GetLastError()).c_str());
	}

	ShowWindow(hwnd, SW_SHOW);

	//setup input, debug, and graphics
	input.start(hwnd);
	debug.start(debugMode);
	//graphics.Start(hwnd, width, height);

	Debug::log("test25");
}

Window::~Window() {
	if (hwnd != nullptr)
		DestroyWindow(hwnd);
}

Window::ExitCode Window::Run()
{
	ExitCode out;
	try {
		if (!WindowClosed(&out.exitCode)) {

			time.update();
			input.update();
			//draw frame

			out.windowClosed = false;
		}
		else {
			out.windowClosed = true;
		}

		return out;
	}
	catch (std::exception& e) {
		MessageBoxA(nullptr, e.what(), "Error", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...) {
		MessageBoxA(nullptr, "Check log file for more info.", "Error", MB_OK | MB_ICONEXCLAMATION);
	}

	//error
	out.windowClosed = true;
	return out;
}

bool Window::WindowClosed(int* quitMessage)
{
	// for all messages in queue
	MSG msg = {};
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		// if message is quit, set quitMessage and return true
		if (msg.message == WM_QUIT) {
			*quitMessage = (int)msg.wParam;
			return true;
		}

		//deal with messages
		TranslateMessage(&msg);
		Debug::logErrorCode(DispatchMessage(&msg));
	}

	// if not quitting, return false
	return false;
}

LRESULT Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{

	//keyboard
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		window->input.updateKey(wParam, true);
		break;

	case WM_SYSKEYUP:
	case WM_KEYUP:
		window->input.updateKey(wParam, false);
		break;

	//mouse buttons
	case WM_LBUTTONDOWN:
		window->input.updateMouse(0, true);
		break;
	case WM_LBUTTONUP:
		window->input.updateMouse(0, false);
		break;

	case WM_RBUTTONDOWN:
		window->input.updateMouse(1, true);
		break;
	case WM_RBUTTONUP:
		window->input.updateMouse(1, false);
		break;

	case WM_MBUTTONDOWN:
		window->input.updateMouse(2, true);
		break;
	case WM_MBUTTONUP:
		window->input.updateMouse(2, false);
		break;

	//mouse pos
	case WM_MOUSEWHEEL:
		if(GET_WHEEL_DELTA_WPARAM(wParam) < 0)
			window->input.updateMouseWheel(-1);
		else
			window->input.updateMouseWheel(1);
		break;

	case WM_INPUT:
		window->input.updateMousePos(lParam);
		break;
	
	//close window
	case WM_DESTROY:
		PostQuitMessage(25);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
