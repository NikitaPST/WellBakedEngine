#ifndef _MODEL_H
#define _MODEL_H

#include <d3d11.h>

namespace WBEngine
{
	class Model
	{
	public:
		Model();
		~Model();

		bool LoadTestModel(ID3D11Device* pDevice);
		void Dispose();
		void Render(ID3D11DeviceContext* pDeviceContext);

		unsigned int GetIndexCount();

	private:
		unsigned int m_nVertexCount;
		unsigned int m_nIndexCount;

		ID3D11Buffer* m_pVertexBuffer;
		ID3D11Buffer* m_pIndexBuffer;
	};
}

#endif