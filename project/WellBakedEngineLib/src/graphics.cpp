#include "graphics.h"

#include "logger.h"

namespace WBEngine
{
	Graphics::Graphics()
	{
	}

	Graphics::~Graphics()
	{
	}

	bool Graphics::Initialize()
	{
		Logger::Info(L"Graphics initialization started");

		Logger::Info(L"Graphics initialization finished");
		return true;
	}

	void Graphics::Shutdown()
	{
		Logger::Info(L"Graphics shutdown started");

		Logger::Info(L"Graphics shutdown finished");
	}
}