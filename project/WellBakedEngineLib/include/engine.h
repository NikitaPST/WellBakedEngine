#ifndef _ENGINE_H
#define _ENGINE_H

#include "input.h"
#include "graphics.h"

namespace WBEngine
{
	class Engine
	{
	public:
		Engine();
		~Engine();

		bool Initialize();
		void Shutdown();
		void Run();

	private:
		Graphics* m_pGraphics;
		Input* m_pInput;
	};
}

#endif