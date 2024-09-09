#include "graphics/game_object.h"

namespace WBEngine
{
	GameObject::GameObject(Model* pModel)
	{
		m_pModel = pModel;

		m_mWorld = DirectX::XMMatrixIdentity();
	}

	GameObject::~GameObject()
	{
		m_pModel = nullptr;
	}
}