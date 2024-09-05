#include "dx/direct3d.h"

#include "logger.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

namespace WBEngine
{
	Direct3D::Direct3D()
	{
		m_pSwapChain = NULL;
		m_pDevice = NULL;
		m_pDeviceContext = NULL;
		m_pRenderTargetView = NULL;
		m_pDepthStencilBuffer = NULL;
		m_pDepthStencilState = NULL;
		m_pDepthStencilView = NULL;
		m_pRasterState = NULL;

		ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
		m_mProjectionMatrix = DirectX::XMMatrixIdentity();
		m_clrBackgroundColor = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	}

	Direct3D::~Direct3D()
	{
	}

	bool Direct3D::Initialize(INT nScreenWidth, INT nScreenHeight, HWND hWnd)
	{
		Logger::Info(L"Direct3D initialization started");

		IDXGIFactory* pFactory = NULL;
		HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
		if (FAILED(hr))
		{
			Logger::Error(L"Failed to create DXGI Factory");
			return false;
		}
		Logger::Info(L"DXGI Factory created successfully");

		IDXGIAdapter* pAdapter = NULL;
		hr = pFactory->EnumAdapters(0, &pAdapter);
		if (FAILED(hr))
		{
			Logger::Error(L"Failed to get video card interface");
			pFactory->Release(); pFactory = NULL;
			return false;
		}
		Logger::Info(L"Access to video card interface established successfully");

		IDXGIOutput* pAdapterOutput = NULL;
		hr = pAdapter->EnumOutputs(0, &pAdapterOutput);
		if (FAILED(hr))
		{
			Logger::Error(L"Failed to get monitor interface");
			pAdapter->Release(); pAdapter = NULL;
			pFactory->Release(); pFactory = NULL;
			return false;
		}
		Logger::Info(L"Access to monitor interface established successfully");

		UINT uNumModes = 0;
		hr = pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &uNumModes, NULL);
		if (FAILED(hr))
		{
			Logger::Error(L"Failed to get access to display modes list");
			pAdapterOutput->Release(); pAdapterOutput = NULL;
			pAdapter->Release(); pAdapter = NULL;
			pFactory->Release(); pFactory = NULL;
			return false;
		}

		DXGI_MODE_DESC* arrDisplayModeList = new DXGI_MODE_DESC[uNumModes];
		hr = pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
			&uNumModes, arrDisplayModeList);
		if (FAILED(hr))
		{
			Logger::Error(L"Failed to get access to display modes list");
			delete[] arrDisplayModeList; arrDisplayModeList = NULL;
			pAdapterOutput->Release(); pAdapterOutput = NULL;
			pAdapter->Release(); pAdapter = NULL;
			pFactory->Release(); pFactory = NULL;
			return false;
		}
		Logger::Info(L"Display modes list retreived successfully");

		DXGI_RATIONAL refreshRate;
		for (unsigned int i = 0; i < uNumModes; i++)
		{
			if (arrDisplayModeList[i].Width == nScreenWidth && arrDisplayModeList[i].Height == nScreenHeight)
			{
				refreshRate = arrDisplayModeList[i].RefreshRate;
			}
		}

		delete[] arrDisplayModeList; arrDisplayModeList = NULL;
		pAdapterOutput->Release(); pAdapterOutput = NULL;
		pAdapter->Release(); pAdapter = NULL;
		pFactory->Release(); pFactory = NULL;

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = nScreenWidth;
		swapChainDesc.BufferDesc.Height = nScreenHeight;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = 0;

		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
		hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
			D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext);
		if (FAILED(hr))
		{
			Logger::Error(L"Direct3D Device creation failed");
			return false;
		}
		Logger::Info(L"Direct3D Device creation successful");

		ID3D11Texture2D* pBackBuffer = NULL;
		hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (VOID**) & pBackBuffer);
		if (FAILED(hr))
		{
			Logger::Error(L"Failed to access back buffer");
			return false;
		}

		hr = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
		pBackBuffer->Release(); pBackBuffer = NULL;
		if (FAILED(hr))
		{
			Logger::Error(L"Failed to create render target view");
			return false;
		}
		Logger::Info(L"Render target view creation successful");

		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
		depthBufferDesc.Width = nScreenWidth;
		depthBufferDesc.Height = nScreenWidth;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		hr = m_pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthStencilBuffer);
		if (FAILED(hr))
		{
			Logger::Error(L"Depth stencil buffer creation failed");
			return false;
		}
		Logger::Info(L"Depth stencil buffer creation successful");

		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.StencilEnable = TRUE;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		hr = m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
		if (FAILED(hr))
		{
			Logger::Error(L"Depth stencil state creation failed");
			return false;
		}
		m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
		Logger::Info(L"Depth stencil state creation successful");

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView);
		if (FAILED(hr))
		{
			Logger::Error(L"Depth stencil view creation failed");
			return false;
		}
		m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
		Logger::Info(L"Depth stencil view creation successful");

		D3D11_RASTERIZER_DESC rasterDesc;
		ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
		rasterDesc.AntialiasedLineEnable = FALSE;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = TRUE;
		rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
		rasterDesc.FrontCounterClockwise = FALSE;
		rasterDesc.MultisampleEnable = FALSE;
		rasterDesc.ScissorEnable = FALSE;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		hr = m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState);
		if (FAILED(hr))
		{
			Logger::Error(L"Rasterizer state creation failed");
			return false;
		}
		m_pDeviceContext->RSSetState(m_pRasterState);
		Logger::Info(L"Rasterizer state creation successful");

		m_viewport.Width = (FLOAT)nScreenWidth;
		m_viewport.Height = (FLOAT)nScreenHeight;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;
		
		m_pDeviceContext->RSSetViewports(1, &m_viewport);

		float fFieldOfView = DirectX::XM_PIDIV4;
		float fScreenAspect = (float)nScreenWidth / (float)nScreenHeight;
		m_mProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fFieldOfView, fScreenAspect, SCREEN_NEAR, SCREEN_DEPTH);

		Logger::Info(L"Direct3D initialization finished");
		return true;
	}

	void Direct3D::Shutdown()
	{
		Logger::Info(L"Direct3D shutdown started");

		if (m_pRasterState)
		{
			m_pRasterState->Release();
			m_pRasterState = NULL;
		}

		if (m_pDepthStencilView)
		{
			m_pDepthStencilView->Release();
			m_pDepthStencilView = NULL;
		}

		if (m_pDepthStencilState)
		{
			m_pDepthStencilState->Release();
			m_pDepthStencilState = NULL;
		}

		if (m_pDepthStencilBuffer)
		{
			m_pDepthStencilBuffer->Release();
			m_pDepthStencilBuffer = NULL;
		}

		if (m_pRenderTargetView)
		{
			m_pRenderTargetView->Release();
			m_pRenderTargetView = NULL;
		}

		if (m_pDeviceContext)
		{
			m_pDeviceContext->Release();
			m_pDeviceContext = NULL;
		}

		if (m_pDevice)
		{
			m_pDevice->Release();
			m_pDevice = NULL;
		}

		if (m_pSwapChain)
		{
			m_pSwapChain->Release();
			m_pSwapChain = NULL;
		}

		Logger::Info(L"Direct3D shutdown finished");
	}

	void Direct3D::BeginScene()
	{
		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, reinterpret_cast<const FLOAT*>(&m_clrBackgroundColor));
		m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void Direct3D::EndScene()
	{
		m_pSwapChain->Present(0, 0);
	}

	ID3D11Device* Direct3D::GetDevice()
	{
		return m_pDevice;
	}
}