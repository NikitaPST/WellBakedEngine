#include "configuration/iniconfiguration.h"

#include <Windows.h>

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600
#define INI_FILE_PATH L".\\config.ini"

namespace WBEngine
{
	IniConfiguration::IniConfiguration()
	{
		m_sIniFilePath = INI_FILE_PATH;
	}

	int IniConfiguration::GetWidth() const
	{
		return GetIntValue(L"Graphics", L"Width", DEFAULT_WIDTH);
	}

	int IniConfiguration::GetHeight() const
	{
		return GetIntValue(L"Graphics", L"Height", DEFAULT_HEIGHT);
	}

	int IniConfiguration::GetIntValue(const std::wstring& sSection, const std::wstring& sKey, int nDefaultValue) const
	{
		return GetPrivateProfileInt(sSection.c_str(), sKey.c_str(), nDefaultValue, m_sIniFilePath.c_str());
	}
}