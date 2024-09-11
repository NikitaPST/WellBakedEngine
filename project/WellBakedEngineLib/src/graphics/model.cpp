#include "graphics/model.h"

#include <DirectXMath.h>

#include "logger.h"

namespace WBEngine
{
	struct ColorVertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	Model::Model()
	{
		m_nVertexCount = 0;
		m_nIndexCount = 0;

		m_pVertexBuffer = NULL;
		m_pIndexBuffer = NULL;
	}

	Model::~Model()
	{
	}

	bool Model::LoadTestModel(ID3D11Device* pDevice)
	{
		Logger::Info(L"Test model load started");

		m_nVertexCount = 8;
		m_nIndexCount = 36;

		ColorVertexType* arrVertices = new ColorVertexType[m_nVertexCount];

		arrVertices[0].position = DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f);
		arrVertices[1].position = DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f);
		arrVertices[2].position = DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f);
		arrVertices[3].position = DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f);
		arrVertices[4].position = DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f);
		arrVertices[5].position = DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f);
		arrVertices[6].position = DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f);
		arrVertices[7].position = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

		arrVertices[0].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		arrVertices[1].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		arrVertices[2].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		arrVertices[3].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		arrVertices[4].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		arrVertices[5].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		arrVertices[6].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		arrVertices[7].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(ColorVertexType) * m_nVertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;
		
		D3D11_SUBRESOURCE_DATA vertexData;
		ZeroMemory(&vertexData, sizeof(D3D11_SUBRESOURCE_DATA));

		vertexData.pSysMem = arrVertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		HRESULT hr = pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);
		delete[] arrVertices; arrVertices = nullptr;
		if (FAILED(hr))
		{
			Logger::Error(L"Model vertex buffer creation failed");
			return false;
		}
		Logger::Info(L"Model vertex buffer creation successful");

		unsigned long* arrIndices = new unsigned long[m_nIndexCount];

		// front face
		arrIndices[0] = 2; arrIndices[1] = 4; arrIndices[2] = 6;
		arrIndices[3] = 6; arrIndices[4] = 4; arrIndices[5] = 7;

		// back face
		arrIndices[6] = 0; arrIndices[7] = 3; arrIndices[8] = 1;
		arrIndices[9] = 1; arrIndices[10] = 3; arrIndices[11] = 5;

		// left face
		arrIndices[12] = 0; arrIndices[13] = 2; arrIndices[14] = 3;
		arrIndices[15] = 3; arrIndices[16] = 2; arrIndices[17] = 6;

		// right face
		arrIndices[18] = 1; arrIndices[19] = 5; arrIndices[20] = 4;
		arrIndices[21] = 4; arrIndices[22] = 5; arrIndices[23] = 7;

		// top face
		arrIndices[24] = 2; arrIndices[25] = 6; arrIndices[26] = 4;
		arrIndices[27] = 4; arrIndices[28] = 6; arrIndices[29] = 7;

		// bottom face
		arrIndices[30] = 0; arrIndices[31] = 1; arrIndices[32] = 3;
		arrIndices[33] = 1; arrIndices[34] = 5; arrIndices[35] = 3;

		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));

		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_nIndexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexData;
		ZeroMemory(&indexData, sizeof(D3D11_SUBRESOURCE_DATA));

		indexData.pSysMem = arrIndices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		hr = pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);
		delete[] arrIndices; arrIndices = nullptr;
		if (FAILED(hr))
		{
			Logger::Error(L"Model index buffer creation failed");
			return false;
		}
		Logger::Info(L"Model index buffer creation successful");

		Logger::Info(L"Test model load finished");
		return true;
	}

	void Model::Dispose()
	{
		if (m_pIndexBuffer)
		{
			m_pIndexBuffer->Release();
			m_pIndexBuffer = NULL;
		}

		if (m_pVertexBuffer)
		{
			m_pVertexBuffer->Release();
			m_pVertexBuffer = NULL;
		}
	}

	void Model::Render(ID3D11DeviceContext* pDeviceContext)
	{
		UINT uStride = sizeof(ColorVertexType);
		UINT uOffset = 0;

		pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &uStride, &uOffset);
		pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	unsigned int Model::GetIndexCount()
	{
		return m_nIndexCount;
	}
}