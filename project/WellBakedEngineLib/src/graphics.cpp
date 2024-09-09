#include "graphics.h"

#include "logger.h"

namespace WBEngine
{
	Graphics::Graphics()
	{
		m_pDirect3D = new Direct3D();
		m_pColorShader = new ColorShader();
		m_pCamera = new Camera();
		m_pModelCollection = new ModelCollection();
		m_pGameObjectCollection = new GameObjectCollection(m_pModelCollection);
	}

	Graphics::~Graphics()
	{
		if (m_pGameObjectCollection)
		{
			delete m_pGameObjectCollection;
			m_pGameObjectCollection = nullptr;
		}

		if (m_pModelCollection)
		{
			delete m_pModelCollection;
			m_pModelCollection = nullptr;
		}

		if (m_pCamera)
		{
			delete m_pCamera;
			m_pCamera = nullptr;
		}

		if (m_pColorShader)
		{
			delete m_pColorShader;
			m_pColorShader = nullptr;
		}

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

		if (!m_pColorShader->Initialize(m_pDirect3D->GetDevice()))
		{
			Logger::Error(L"Color shader initialization failed");
			return false;
		}

		m_pCamera->SetPosition(0.0f, 0.0f, -5.0f);

		Logger::Info(L"Graphics initialization finished");
		return true;
	}

	void Graphics::Shutdown()
	{
		Logger::Info(L"Graphics shutdown started");

		if (m_pGameObjectCollection)
		{
			m_pGameObjectCollection->Dispose();
		}

		if (m_pModelCollection)
		{
			m_pModelCollection->Dispose();
		}

		if (m_pColorShader)
		{
			m_pColorShader->Dispose();
		}

		if (m_pDirect3D)
		{
			m_pDirect3D->Shutdown();
		}

		Logger::Info(L"Graphics shutdown finished");
	}

	bool Graphics::RenderFrame()
	{
		m_pDirect3D->BeginScene();

		m_pCamera->Render();

		m_pDirect3D->EndScene();
		return true;
	}

	bool Graphics::CreateTestModel()
	{
		return m_pModelCollection->CreateTestModel(m_pDirect3D->GetDevice());
	}

	GameObject* Graphics::CreateGameObject(std::wstring sObjectName, std::wstring sModelName)
	{
		return m_pGameObjectCollection->CreateObject(sObjectName, sModelName);
	}
}