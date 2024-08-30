#include "logger.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace WBEngine
{
	std::wstring Logger::m_sFilePath = L"logfile.log";

	void Logger::SetFilePath(const std::wstring& sPath)
	{
		m_sFilePath = sPath;

		if (std::filesystem::exists(sPath))
		{
			std::wofstream fLogFile(m_sFilePath, std::ios::trunc);
			if (fLogFile.is_open())
			{
				fLogFile.close();
			}
			else
			{
				std::wcerr << L"Unable to clear log file: " << m_sFilePath << std::endl;
			}
		}
	}

	void Logger::Info(const std::wstring& sMessage)
	{
		Log(L"INFO", sMessage);
	}

	void Logger::Warning(const std::wstring& sMessage)
	{
		Log(L"WARNING", sMessage);
	}

	void Logger::Error(const std::wstring& sMessage)
	{
		Log(L"ERROR", sMessage);
	}

	void Logger::Log(const std::wstring& sSeverity, const std::wstring& sMessage)
	{
		std::wofstream fLogFile(m_sFilePath, std::ios_base::app);
		if (fLogFile.is_open())
		{
			fLogFile << L"[" << GetCurrentDateTime() << L"]" << sSeverity << L": " << sMessage << std::endl;
			fLogFile.close();
		}
		else
		{
			std::wcerr << L"Unable to open log file: " << m_sFilePath.c_str() << std::endl;
		}
	}

	std::wstring Logger::GetCurrentDateTime()
	{
		auto now = std::chrono::system_clock::now();
		std::time_t tNowTime = std::chrono::system_clock::to_time_t(now);
		std::tm tm;
		localtime_s(&tm, &tNowTime);

		std::wostringstream oss;
		oss << std::put_time(&tm, L"%Y-%m-%d %H:%M:%S");
		
		return oss.str();
	}
}