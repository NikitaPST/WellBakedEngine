#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <DirectXMath.h>

#include "graphics/model.h"

namespace WBEngine
{
	class Graphics;

	class GameObject
	{
	public:
		GameObject(Model* pModel);
		~GameObject();

		bool Render(Graphics* pGraphics, DirectX::XMMATRIX mView, DirectX::XMMATRIX mProjection);

	private:
		Model* m_pModel;
		DirectX::XMMATRIX m_mWorld;
	};
}

#endif