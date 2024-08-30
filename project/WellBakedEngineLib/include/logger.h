#ifndef _LOGGER_H
#define _LOGGER_H

#include <string>

namespace WBEngine
{
	class Logger
	{
	public:
		static void SetFilePath(const std::wstring& sPath);
		static void Info(const std::wstring& sMessage);
		static void Warning(const std::wstring& sMessage);
		static void Error(const std::wstring& sMessage);

	private:
		static std::wstring m_sFilePath;

		static void Log(const std::wstring& sSeverity, const std::wstring& sMessage);
		static std::wstring GetCurrentDateTime();
	};
}

#endif