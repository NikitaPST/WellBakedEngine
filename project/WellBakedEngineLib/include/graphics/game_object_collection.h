#ifndef _GAME_OBJECT_COLLECTION_H
#define _GAME_OBJECT_COLLECTION_H

#include <map>
#include <string>

#include "graphics/game_object.h"
#include "graphics/model_collection.h"

namespace WBEngine
{
	class GameObjectCollection
	{
	public:
		GameObjectCollection(ModelCollection* pModelCollection);
		~GameObjectCollection();

		void Dispose();
		GameObject* CreateObject(std::wstring sObjectName, std::wstring sModelName);

	private:
		std::map<std::wstring, GameObject*> m_dictObjects;

		ModelCollection* m_pModelCollection;
	};
}

#endif