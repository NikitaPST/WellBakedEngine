#include <Windows.h>

#include "engine.h"
#include "logger.h"

bool LoadScene(WBEngine::Engine* pEngine);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	WBEngine::Logger::SetFilePath(L"logfile.log");
	WBEngine::Engine* pEngine = new WBEngine::Engine();

	if (pEngine->Initialize() && LoadScene(pEngine))
	{
		pEngine->Run();
	}
	else
	{
		MessageBox(NULL, L"Well-Baked Engine initialization failed", L"Well-Baked Engine", MB_ICONERROR | MB_OK);
	}

	pEngine->Shutdown();
	delete pEngine; pEngine = NULL;

	return 0;
}

bool LoadScene(WBEngine::Engine* pEngine)
{
	if (!pEngine->GetGraphics()->CreateTestModel())
		return false;

	return true;
}