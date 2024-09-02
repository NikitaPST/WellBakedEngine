#ifndef _DIRECT3D_H
#define _DIRECT3D_H

#include <d3d11.h>
#include <DirectXMath.h>

namespace WBEngine
{
	class Direct3D
	{
	public:
		Direct3D();
		~Direct3D();

		bool Initialize(INT nScreenWidth, INT nScreenHeight, HWND hWnd);
		void Shutdown();

		void BeginScene();
		void EndScene();

	private:
		IDXGISwapChain* m_pSwapChain;
		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pDeviceContext;
		ID3D11RenderTargetView* m_pRenderTargetView;
		ID3D11Texture2D* m_pDepthStencilBuffer;
		ID3D11DepthStencilState* m_pDepthStencilState;
		ID3D11DepthStencilView* m_pDepthStencilView;
		ID3D11RasterizerState* m_pRasterState;

		D3D11_VIEWPORT m_viewport;
		DirectX::XMMATRIX m_mProjectionMatrix;
		DirectX::XMFLOAT4 m_clrBackgroundColor;
	};

	const float SCREEN_NEAR = 0.3f;
	const float SCREEN_DEPTH = 1000.0f;
}

#endif