#include "logger.h"

void Logger::Log(const std::string& message)
{
	std::wstring wstr(message.begin(), message.end());
	Log(wstr);
}

void Logger::Log(const std::wstring& message)
{
	std::wcout << message << std::endl;
}