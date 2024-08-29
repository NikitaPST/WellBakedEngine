#include <Windows.h>

#include "engine.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	WBEngine::Engine* pEngine = new WBEngine::Engine();

	if (pEngine->Initialize())
	{
		pEngine->Run();
	}

	delete pEngine; pEngine = NULL;

	return 0;
}