#include "engine.h"

#include <sstream>

#include "logger.h"
#include "configuration/iniconfiguration.h"

#pragma comment(lib, "Msimg32.lib")

namespace WBEngine
{
	Engine::Engine()
	{
		m_hBackgroundBrush = NULL;
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

		if (m_hBackgroundBrush)
		{
			DeleteObject(m_hBackgroundBrush);
			m_hBackgroundBrush = NULL;
		}

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
		static HBITMAP hLogo = NULL;

		switch (uMsg)
		{
			case WM_CLOSE:
				DestroyWindow(hWnd);
				return 0;
			case WM_CREATE:
				hLogo = (HBITMAP)LoadImage(NULL, L"..\\..\\assets\\icons\\Engine256.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
				return 0;
			case WM_DESTROY:
				if (hLogo)
				{
					DeleteObject(hLogo);
				}
				PostQuitMessage(0);
				return 0;
			case WM_KEYDOWN:
				m_pInput->KeyDown((UINT)wParam);
				return 0;
			case WM_KEYUP:
				m_pInput->KeyUp((UINT)wParam);
				return 0;
			case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);

				SetTextColor(hdc, RGB(255, 77, 0));
				SetBkMode(hdc, TRANSPARENT);

				RECT rect;
				GetClientRect(hWnd, &rect);
				DrawLogo(hWnd, hdc, hLogo, rect);

				EndPaint(hWnd, &ps);
				return 0;
			}
			default:
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}

	bool Engine::InitializeWindow(int& nWidth, int& nHeight)
	{
		Logger::Info(L"Windows initialization started");

		m_hInstance = GetModuleHandle(NULL);

		m_hBackgroundBrush = CreateSolidBrush(RGB(30, 30, 30));

		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEX));

		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_hInstance;
		wc.hIcon = (HICON)LoadImage(NULL, L"..\\..\\assets\\icons\\Engine32.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		wc.hIconSm = (HICON)LoadImage(NULL, L"..\\..\\assets\\icons\\Engine16.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = m_hBackgroundBrush;
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
		if (m_pInput->IsKeyUp(VK_ESCAPE))
		{
			Logger::Info(L"User pressed ESC");
			PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		}

		m_pInput->UpdateFrame();

		return true;
	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return g_pEngineInstance->MessageHandler(hWnd, uMsg, wParam, lParam);
	}

	void Engine::DrawLogo(HWND hWnd, HDC hdc, HBITMAP hLogo, RECT rc)
	{
		if (hLogo == NULL)
			return;

		HDC hdcMem = CreateCompatibleDC(hdc);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hLogo);

		BITMAP bm;
		GetObject(hLogo, sizeof(BITMAP), &bm);

		int nImgX = (rc.right - bm.bmWidth) / 2;
		int nImgY = (rc.bottom - bm.bmHeight) / 2;

		TransparentBlt(hdc, nImgX, nImgY, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0,
			bm.bmWidth, bm.bmHeight, RGB(255, 255, 255));

		SelectObject(hdcMem, hOldBitmap);
		DeleteDC(hdcMem);
	}
}