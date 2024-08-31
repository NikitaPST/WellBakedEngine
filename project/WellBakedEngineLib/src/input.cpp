#include "input.h"

#include <cstring>

namespace WBEngine
{
	Input::Input()
	{
		memset(m_arrKeys, 0, sizeof(bool[256]));
		memset(m_arrOldKeys, 0, sizeof(bool[256]));
	}

	Input::~Input()
	{
	}

	void Input::UpdateFrame()
	{
		memcpy(m_arrOldKeys, m_arrKeys, sizeof(bool[256]));
	}

	void Input::KeyDown(unsigned int uKey)
	{
		m_arrKeys[uKey] = true;
	}

	void Input::KeyUp(unsigned int uKey)
	{
		m_arrKeys[uKey] = false;
	}

	bool Input::IsKeyUp(unsigned int uKey)
	{
		return (!m_arrKeys[uKey] && m_arrOldKeys[uKey]);
	}
}