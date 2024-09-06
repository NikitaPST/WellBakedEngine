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

	private:
		int m_nVertexCount;
		int m_nIndexCount;

		ID3D11Buffer* m_pVertexBuffer;
		ID3D11Buffer* m_pIndexBuffer;
	};
}

#endif