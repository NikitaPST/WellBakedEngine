#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <string>

#include "dx/color_shader.h"
#include "dx/direct3d.h"
#include "graphics/camera.h"
#include "graphics/game_object_collection.h"
#include "graphics/model_collection.h"

namespace WBEngine
{
	class Graphics
	{
	public:
		Graphics();
		~Graphics();

		bool Initialize(INT nScreenWidth, INT nScreenHeight, HWND hWnd);
		void Shutdown();
		bool RenderFrame();

		bool CreateTestModel();
		GameObject* CreateGameObject(std::wstring sObjectName, std::wstring sModelName);

	private:
		Direct3D* m_pDirect3D;
		ColorShader* m_pColorShader;
		Camera* m_pCamera;
		ModelCollection* m_pModelCollection;
		GameObjectCollection* m_pGameObjectCollection;
	};
}

#endif