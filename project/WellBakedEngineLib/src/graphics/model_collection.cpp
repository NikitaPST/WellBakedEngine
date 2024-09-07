#include "graphics/model_collection.h"

#include <sstream>

#include "logger.h"

namespace WBEngine
{
	ModelCollection::ModelCollection()
	{
	}

	ModelCollection::~ModelCollection()
	{
	}

	void ModelCollection::Dispose()
	{
		Logger::Info(L"Model collection disposal started");

		for (auto it = m_dictModels.begin(); it != m_dictModels.end();)
		{
			std::wstringstream sMessage;
			sMessage << it->first << L" model is disposing";
			Logger::Info(sMessage.str());

			if (it->second)
			{
				it->second->Dispose();
				delete it->second;
			}

			it = m_dictModels.erase(it);
		}

		m_dictModels.clear();

		Logger::Info(L"Model collection disposal finished");
	}

	bool ModelCollection::CreateTestModel(ID3D11Device* pDevice)
	{
		if (m_dictModels.count(TEST_MODEL_NAME) > 0)
		{
			Logger::Warning(L"Test model already exists");
			return true;
		}

		Model* pModel = new Model();
		if (!pModel->LoadTestModel(pDevice))
		{
			delete pModel; pModel = nullptr;
			return false;
		}

		m_dictModels[TEST_MODEL_NAME] = pModel;

		Logger::Info(L"Test model created");
		return true;
	}
}