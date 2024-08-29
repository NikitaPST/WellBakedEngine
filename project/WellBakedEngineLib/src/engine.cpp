#include "engine.h"

#include <Windows.h>

namespace WBEngine
{
	Engine::Engine()
	{
		m_pGraphics = new Graphics();
		m_pInput = new Input();
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
	}

	bool Engine::Initialize()
	{
		if (!m_pGraphics->Initialize())
		{
			return false;
		}

		return true;
	}

	void Engine::Shutdown()
	{
		if (m_pGraphics)
		{
			m_pGraphics->Shutdown();
		}
	}

	void Engine::Run()
	{
		MessageBox(NULL, L"Well-Baked Engine is running", L"Well-Baked Engine", MB_ICONINFORMATION | MB_OK);
	}
}