#include "engine.h"

#include <sstream>

#include "logger.h"
#include "configuration/iniconfiguration.h"

namespace WBEngine
{
	Engine::Engine()
	{
		m_hInstance = NULL;
		m_hWnd = NULL;

		m_pGraphics = new Graphics();
		m_pInput = new Input();

		g_pEngineInstance = this;
	}

	Engine::~Engine()
	{
		if (m_pInput)
		{
			delete m_pInput;
			m_pInput = nullptr;
		}

		if (m_pGraphics)
		{
			delete m_pGraphics;
			m_pGraphics = nullptr;
		}

		g_pEngineInstance = nullptr;
	}

	bool Engine::Initialize()
	{
		Logger::Info(L"Engine initialization started");

		// Configuration
		IniConfiguration config;
		int nWidth = config.GetWidth();
		int nHeight = config.GetHeight();

		std::wstringstream sLogMessage;
		sLogMessage << L"Engine Configuration:" << std::endl;
		sLogMessage << L"\tWidth: " << nWidth << std::endl;
		sLogMessage << L"\tHeight: " << nHeight;
		Logger::Info(sLogMessage.str());

		if (!InitializeWindow(nWidth, nHeight))
		{
			Logger::Error(L"Windows initialization failed");
			return false;
		}

		if (!m_pGraphics->Initialize())
		{
			Logger::Error(L"Graphics initialization failed");
			return false;
		}

		Logger::Info(L"Engine initialization finished");
		return true;
	}

	void Engine::Shutdown()
	{
		Logger::Info(L"Engine shutdown started");

		if (m_pGraphics)
		{
			m_pGraphics->Shutdown();
		}

		if (m_hWnd)
		{
			DestroyWindow(m_hWnd);
			m_hWnd = NULL;
		}
		UnregisterClass(L"Well-Baked Engine", m_hInstance);

		Logger::Info(L"Engine shutdown finished");
	}

	void Engine::Run()
	{
		Logger::Info(L"Engine is running");

		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		bool bDone = false;
		while (!bDone)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (msg.message == WM_QUIT)
			{
				bDone = true;
			}
			else
			{
				if (!FrameProcessing())
				{
					Logger::Error(L"Frame processing failed");
					bDone = true;
				}
			}
		}
	}

	LRESULT CALLBACK Engine::MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			case WM_CLOSE:
				DestroyWindow(hWnd);
				return 0;
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
			default:
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}

	bool Engine::InitializeWindow(int& nWidth, int& nHeight)
	{
		Logger::Info(L"Windows initialization started");

		m_hInstance = GetModuleHandle(NULL);

		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEX));

		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hIcon = wc.hIcon;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = L"Well-Baked Engine";
		wc.cbSize = sizeof(WNDCLASSEX);

		if (RegisterClassEx(&wc) == 0)
		{
			Logger::Error(L"Failed to register windows class");
			return false;
		}

		int nPosX = 100, nPosY = 50;

		m_hWnd = CreateWindowEx(
			WS_EX_APPWINDOW,
			L"Well-Baked Engine",
			L"Well-Baked Engine",
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_BORDER,
			nPosX,
			nPosY,
			nWidth,
			nHeight,
			NULL,
			NULL,
			m_hInstance,
			NULL
		);
		
		if (m_hWnd == NULL)
		{
			Logger::Error(L"Failed to create window");
			return false;
		}

		ShowWindow(m_hWnd, SW_SHOW);
		SetForegroundWindow(m_hWnd);
		SetFocus(m_hWnd);

		Logger::Info(L"Windows initialization finished");
		return true;
	}

	bool Engine::FrameProcessing()
	{
		return true;
	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return g_pEngineInstance->MessageHandler(hWnd, uMsg, wParam, lParam);
	}
}