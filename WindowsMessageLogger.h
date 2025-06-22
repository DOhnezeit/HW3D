#pragma once

#include "Win.h" 

#include <string>
#include <unordered_map>


// Using this class to get an idea of which Windows messages I need to handle
class WindowsMessageLogger
{
public:
	WindowsMessageLogger();
	std::wstring operator()(DWORD msg, LPARAM lParam, WPARAM wParam) const;
private:
	std::unordered_map<DWORD, std::string> map;
	static std::wstring SimpleToWstr(const std::string& str);
};

