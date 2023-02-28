#pragma once

#include <Windows.h> 

#include "Input.h"
#include "Time.h"
#include "Debug.h"

//#pragma comment(lib, "user32.lib")

namespace Engine {
	class Window
	{
	public:

		~Window();
		
		// Start Window
		bool Start(HINSTANCE hInstance, std::wstring name, DWORD style, int width, int height, bool debugMode);

		//Run Window Frame
		struct ExitCode {
			bool windowClosed = false;
			int exitCode = 0;
		};
		ExitCode Run();

	private:

		// Reads messages and checks if window should close
		bool WindowClosed(int* quitMessage);

		// Windows Procedure
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:

		static Window* window;
		HWND hwnd = nullptr;

		// Local instances to be statically called on
		Input input;
		Time time;
		Debug debug;
	};
}