#ifndef _COLOR_SHADER_H
#define _COLOR_SHADER_H

#include <d3d11.h>
#include <DirectXMath.h>

namespace WBEngine
{
	class ColorShader
	{
	public:
		ColorShader();
		~ColorShader();

		bool Initialize(ID3D11Device* pDevice);
		void Dispose();
		bool Render(ID3D11DeviceContext* pDeviceContext, UINT nIndexCount, DirectX::XMMATRIX mWorld,
			DirectX::XMMATRIX mView, DirectX::XMMATRIX mProjection);

	private:
		ID3D11VertexShader* m_pVertexShader;
		ID3D11PixelShader* m_pPixelShader;
		ID3D11InputLayout* m_pLayout;
		ID3D11Buffer* m_pMatrixBuffer;

		void OutputShaderErrorMessage(ID3D10Blob* pErrorMessage);
	};
}

#endif