#include "engine.h"

#include <sstream>
#include <Windows.h>

#include "logger.h"
#include "configuration/iniconfiguration.h"

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

		if (!m_pGraphics->Initialize())
		{
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

		Logger::Info(L"Engine shutdown finished");
	}

	void Engine::Run()
	{
		Logger::Info(L"Engine is running");

		MessageBox(NULL, L"Well-Baked Engine is running", L"Well-Baked Engine", MB_ICONINFORMATION | MB_OK);
	}
}