#ifndef _MODEL_COLLECTION_H
#define _MODEL_COLLECTION_H

#include <d3d11.h>
#include <map>
#include <string>

#include "graphics/model.h"

namespace WBEngine
{
	class ModelCollection
	{
	public:
		ModelCollection();
		~ModelCollection();

		void Dispose();
		bool CreateTestModel(ID3D11Device* pDevice);

	private:
		std::map<std::wstring, Model*> m_dictModels;
	};

	const std::wstring TEST_MODEL_NAME = L"_TEST_MODEL_";
}

#endif