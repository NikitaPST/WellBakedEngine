#ifndef _INPUT_H
#define _INPUT_H

namespace WBEngine
{
	class Input
	{
	public:
		Input();
		~Input();

		void UpdateFrame();
		void KeyDown(unsigned int uKey);
		void KeyUp(unsigned int uKey);

		bool IsKeyUp(unsigned int uKey);

	private:
		bool m_arrKeys[256];
		bool m_arrOldKeys[256];
	};
}

#endif