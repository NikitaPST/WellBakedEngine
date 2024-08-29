#ifndef _GRAPHICS_H
#define _GRAPHICS_H

namespace WBEngine
{
	class Graphics
	{
	public:
		Graphics();
		~Graphics();

		bool Initialize();
		void Shutdown();
	};
}

#endif