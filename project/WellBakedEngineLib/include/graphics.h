#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "dx/color_shader.h"
#include "dx/direct3d.h"
#include "graphics/camera.h"

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

	private:
		Direct3D* m_pDirect3D;
		ColorShader* m_pColorShader;
		Camera* m_pCamera;
	};
}

#endif