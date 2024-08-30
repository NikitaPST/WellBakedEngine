#ifndef _INI_CONFIGURATION_H
#define _INI_CONFIGURATION_H

#include <string>

namespace WBEngine
{
	class IniConfiguration
	{
	public:
		IniConfiguration();

		// Graphics
		int GetWidth() const;
		int GetHeight() const;

	private:
		std::wstring m_sIniFilePath;

		int GetIntValue(const std::wstring& sSection, const std::wstring& sKey, int nDefaultValue) const;
	};
}

#endif