#include "graphics.h"

#include "logger.h"

namespace WBEngine
{
	Graphics::Graphics()
	{
		m_pDirect3D = new Direct3D();
	}

	Graphics::~Graphics()
	{
		if (m_pDirect3D)
		{
			delete m_pDirect3D;
			m_pDirect3D = nullptr;
		}
	}

	bool Graphics::Initialize(INT nScreenWidth, INT nScreenHeight, HWND hWnd)
	{
		Logger::Info(L"Graphics initialization started");

		if (!m_pDirect3D->Initialize(nScreenWidth, nScreenHeight, hWnd))
		{
			Logger::Error(L"Direct3D initialization failed");
			return false;
		}

		Logger::Info(L"Graphics initialization finished");
		return true;
	}

	void Graphics::Shutdown()
	{
		Logger::Info(L"Graphics shutdown started");

		if (m_pDirect3D)
		{
			m_pDirect3D->Shutdown();
		}

		Logger::Info(L"Graphics shutdown finished");
	}

	bool Graphics::RenderFrame()
	{
		m_pDirect3D->BeginScene();

		m_pDirect3D->EndScene();
		return true;
	}
}