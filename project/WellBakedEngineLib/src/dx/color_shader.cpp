#include "dx/color_shader.h"

#include <d3dcompiler.h>
#include <string>

#include "logger.h"

#pragma comment(lib, "D3DCompiler.lib")

namespace WBEngine
{
	struct MatrixBuffer
	{
		DirectX::XMMATRIX mWorld;
		DirectX::XMMATRIX mView;
		DirectX::XMMATRIX mProjection;
	};

	ColorShader::ColorShader()
	{
		m_pVertexShader = NULL;
		m_pPixelShader = NULL;
		m_pLayout = NULL;
		m_pMatrixBuffer = NULL;
	}

	ColorShader::~ColorShader()
	{
	}

	bool ColorShader::Initialize(ID3D11Device* pDevice)
	{
		Logger::Info(L"Color shader initialization started");

		std::wstring sVSFilename = L"../WellBakedEngineLib/hlsl/color.vs";
		std::wstring sPSFilename = L"../WellBakedEngineLib/hlsl/color.ps";

		ID3D10Blob* pErrorMessage = NULL;
		ID3D10Blob* pVertexShaderBuffer = NULL;
		HRESULT hr = D3DCompileFromFile(sVSFilename.c_str(), NULL, NULL, "ColorVertexShader", "vs_5_0",
			D3D10_SHADER_ENABLE_STRICTNESS, 0, &pVertexShaderBuffer, &pErrorMessage);
		if (FAILED(hr))
		{
			Logger::Error(L"Color vertex shader compilation failed");
			if (pErrorMessage)
			{
				OutputShaderErrorMessage(pErrorMessage);
			}
			return false;
		}
		Logger::Info(L"Color vertex shader compilation successful");

		ID3D10Blob* pPixelShaderBuffer = NULL;
		hr = D3DCompileFromFile(sPSFilename.c_str(), NULL, NULL, "ColorPixelShader", "ps_5_0",
			D3D10_SHADER_ENABLE_STRICTNESS, 0, &pPixelShaderBuffer, &pErrorMessage);
		if (FAILED(hr))
		{
			Logger::Error(L"Color pixel shader compilation failed");
			if (pErrorMessage)
			{
				OutputShaderErrorMessage(pErrorMessage);
			}
			pVertexShaderBuffer->Release(); pVertexShaderBuffer = NULL;
			return false;
		}
		Logger::Info(L"Color pixel shader compilation successful");

		hr = pDevice->CreateVertexShader(pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(),
			NULL, &m_pVertexShader);
		if (FAILED(hr))
		{
			Logger::Error(L"Color vertex shader creation failed");
			pPixelShaderBuffer->Release(); pPixelShaderBuffer = NULL;
			pVertexShaderBuffer->Release(); pVertexShaderBuffer = NULL;
			return false;
		}
		Logger::Info(L"Color vertex shader creation successful");

		hr = pDevice->CreatePixelShader(pPixelShaderBuffer->GetBufferPointer(), pPixelShaderBuffer->GetBufferSize(),
			NULL, &m_pPixelShader);
		if (FAILED(hr))
		{
			Logger::Error(L"Color pixel shader creation failed");
			pPixelShaderBuffer->Release(); pPixelShaderBuffer = NULL;
			pVertexShaderBuffer->Release(); pVertexShaderBuffer = NULL;
			return false;
		}
		Logger::Info(L"Color pixel shader creation successful");

		D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
		ZeroMemory(&polygonLayout, sizeof(D3D11_INPUT_ELEMENT_DESC[2]));

		polygonLayout[0].SemanticName = "POSITION";
		polygonLayout[0].SemanticIndex = 0;
		polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[0].InputSlot = 0;
		polygonLayout[0].AlignedByteOffset = 0;
		polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[0].InstanceDataStepRate = 0;

		polygonLayout[1].SemanticName = "COLOR";
		polygonLayout[1].SemanticIndex = 0;
		polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		polygonLayout[1].InputSlot = 0;
		polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[1].InstanceDataStepRate = 0;

		hr = pDevice->CreateInputLayout(polygonLayout, 2, pVertexShaderBuffer->GetBufferPointer(),
			pVertexShaderBuffer->GetBufferSize(), &m_pLayout);
		pPixelShaderBuffer->Release(); pPixelShaderBuffer = NULL;
		pVertexShaderBuffer->Release(); pVertexShaderBuffer = NULL;
		if (FAILED(hr))
		{
			Logger::Error(L"Color shader layout creation failed");
			return false;
		}
		Logger::Info(L"Color shader layout creation successful");

		D3D11_BUFFER_DESC matrixBufferDesc;
		ZeroMemory(&matrixBufferDesc, sizeof(D3D11_BUFFER_DESC));
		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
		matrixBufferDesc.StructureByteStride = 0;

		hr = pDevice->CreateBuffer(&matrixBufferDesc, NULL, &m_pMatrixBuffer);
		if (FAILED(hr))
		{
			Logger::Error(L"Color shader matrix buffer creation failed");
			return false;
		}
		Logger::Info(L"Color shader matrix buffer creation successful");

		Logger::Info(L"Color shader initialization finished");
		return true;
	}

	void ColorShader::Dispose()
	{
		Logger::Info(L"Color shader disposal started");

		if (m_pMatrixBuffer)
		{
			m_pMatrixBuffer->Release();
			m_pMatrixBuffer = NULL;
		}

		if (m_pLayout)
		{
			m_pLayout->Release();
			m_pLayout = NULL;
		}

		if (m_pPixelShader)
		{
			m_pPixelShader->Release();
			m_pPixelShader = NULL;
		}

		if (m_pVertexShader)
		{
			m_pVertexShader->Release();
			m_pVertexShader = NULL;
		}

		Logger::Info(L"Color shader disposal finished");
	}

	bool ColorShader::Render(ID3D11DeviceContext* pDeviceContext, UINT nIndexCount, DirectX::XMMATRIX mWorld,
		DirectX::XMMATRIX mView, DirectX::XMMATRIX mProjection)
	{
		mWorld = DirectX::XMMatrixTranspose(mWorld);
		mView = DirectX::XMMatrixTranspose(mView);
		mProjection = DirectX::XMMatrixTranspose(mProjection);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		sizeof(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		HRESULT hr = pDeviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr))
		{
			Logger::Error(L"Cannot set matrix buffer for color shader");
			return false;
		}

		MatrixBuffer* pData = (MatrixBuffer*)mappedResource.pData;
		pData->mWorld = mWorld;
		pData->mView = mView;
		pData->mProjection = mProjection;

		pDeviceContext->Unmap(m_pMatrixBuffer, 0);
		pDeviceContext->VSSetConstantBuffers(0, 1, &m_pMatrixBuffer);

		pDeviceContext->IASetInputLayout(m_pLayout);
		pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
		pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

		pDeviceContext->DrawIndexed(nIndexCount, 0, 0);

		return true;
	}

	void ColorShader::OutputShaderErrorMessage(ID3D10Blob* pErrorMessage)
	{
		int nErrorMessageSize = static_cast<int>(pErrorMessage->GetBufferSize());
		const char* pError = static_cast<const char*>(pErrorMessage->GetBufferPointer());

		int nErrorLength = MultiByteToWideChar(CP_UTF8, 0, pError, nErrorMessageSize, nullptr, 0);
		if (nErrorLength > 0)
		{
			std::wstring sError(nErrorLength, 0);
			MultiByteToWideChar(CP_UTF8, 0, pError, nErrorMessageSize, &sError[0], nErrorLength);
			Logger::Error(sError);
		}

		pErrorMessage->Release(); pErrorMessage = NULL;
	}
}