#ifndef _ENGINE_H
#define _ENGINE_H

#include <Windows.h>

#include "input.h"
#include "graphics.h"

namespace WBEngine
{
	class Engine
	{
	public:
		Engine();
		~Engine();

		bool Initialize();
		void Shutdown();
		void Run();

		LRESULT CALLBACK MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		Graphics* m_pGraphics;
		Input* m_pInput;

		HINSTANCE m_hInstance;
		HWND m_hWnd;

		bool InitializeWindow(int& nWidth, int& nHeight);
		bool FrameProcessing();
	};

	// Global functions
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// Global handle
	static Engine* g_pEngineInstance = nullptr;
}

#endif