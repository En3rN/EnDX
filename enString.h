#pragma once
#include <string>
namespace En3rN
{
#define enNL "\n\r"
	class stringconverter 
	{
	public:
		
		static std::wstring wstr(std::string str)
		{
			std::wstring wstring(str.begin(), str.end());
			return wstring;
		}
		static std::string str(std::wstring str)
		{
			std::string string(str.begin(), str.end());
			return string;
		}
	};
}