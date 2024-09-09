#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <DirectXMath.h>

#include "graphics/model.h"

namespace WBEngine
{
	class GameObject
	{
	public:
		GameObject(Model* pModel);
		~GameObject();

	private:
		Model* m_pModel;
		DirectX::XMMATRIX m_mWorld;
	};
}

#endif