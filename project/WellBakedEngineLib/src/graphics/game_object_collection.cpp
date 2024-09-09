#include "graphics/game_object_collection.h"

#include <sstream>

#include "logger.h"

namespace WBEngine
{
	GameObjectCollection::GameObjectCollection(ModelCollection* pModelCollection)
	{
		m_pModelCollection = pModelCollection;
	}

	GameObjectCollection::~GameObjectCollection()
	{
		m_pModelCollection = nullptr;
	}

	void GameObjectCollection::Dispose()
	{
		Logger::Info(L"Game object collection disposal started");

		for (auto it = m_dictObjects.begin(); it != m_dictObjects.end();)
		{
			std::wstringstream sMessage;
			sMessage << it->first << L" game object is disposing";
			Logger::Info(sMessage.str());

			if (it->second)
			{
				delete it->second;
			}

			it = m_dictObjects.erase(it);
		}

		m_dictObjects.clear();

		Logger::Info(L"Game object collection disposal finished");
	}

	GameObject* GameObjectCollection::CreateObject(std::wstring sObjectName, std::wstring sModelName)
	{
		auto it = m_dictObjects.find(sObjectName);
		if (it!= m_dictObjects.end())
		{
			std::wstringstream sMessage;
			sMessage << sObjectName << L" game object already exists";
			Logger::Warning(sMessage.str());
			return it->second;
		}

		Model* pModel = m_pModelCollection->GetModel(sModelName);
		if (!pModel)
		{
			std::wstringstream sMessage;
			sMessage << sModelName << L" model can't be find.";
			Logger::Error(sMessage.str());
			return nullptr;
		}

		GameObject* pObject = new GameObject(pModel);
		m_dictObjects[sObjectName] = pObject;

		std::wstringstream sMessage;
		sMessage << sObjectName << " game object created";
		Logger::Info(sMessage.str());

		return pObject;
	}
}