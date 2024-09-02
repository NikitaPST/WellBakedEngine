#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "dx/direct3d.h"

namespace WBEngine
{
	class Graphics
	{
	public:
		Graphics();
		~Graphics();

		bool Initialize(INT nScreenWidth, INT nScreenHeight, HWND hWnd);
		void Shutdown();

	private:
		Direct3D* m_pDirect3D;
	};
}

#endif