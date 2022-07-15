#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <queue>

class Logger
{
public:
	static void Log(const std::string& message);
	static void Log(const std::wstring& message);
};

