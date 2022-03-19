#pragma once
#include "enWin.h"
#include "enString.h"
#include "InfoManager.h"
#include <source_location>
#include <string>
#include <system_error>
#include <d3d11.h>

namespace En3rN
{
	
	class EnExcept : public std::exception
	{
	public:
		EnExcept() = default;
		EnExcept(std::string msg, const std::source_location location= std::source_location::current())
		{
			file += location.file_name();
			func += location.function_name();
			line += std::to_string(location.line());
			errMsg += msg;
			errMsg += enNL + file + enNL + func +enNL + line;
		}
		const char* what() { return errMsg.c_str(); }
		const char* GetType() { return type.c_str(); }
	protected:
		std::string type = "EnExcept";
		std::string file = "[File] ";
		std::string func = "[Function] ";
		std::string line = "[Line] ";
		std::string errMsg = "[Error] ";
		std::string dxInfo = "[DXInfo] ";
		
	};

	class EnExHR : public EnExcept
	{
	public:
		EnExHR() = default;
		EnExHR(HRESULT hr, const std::source_location location = std::source_location::current())
		{
			if (!En3rN::DX::InfoManager::Empty()) {
				auto iq = En3rN::DX::InfoManager::GetInfo();
				for (auto& i : iq)
					dxInfo += i;
			}
			type = "EnExHR";
			file += location.file_name();
			func += location.function_name();
			line += std::to_string(location.line());
			errMsg += std::system_category().message(hr);
			
			errMsg += enNL + dxInfo + enNL + file + enNL + line;
		}
	};
	class EnExDXInfo : public EnExcept
	{
	public:
		EnExDXInfo(const std::source_location location =
			std::source_location::current())
		{
			if (!En3rN::DX::InfoManager::Empty()) {
				auto iq = En3rN::DX::InfoManager::GetInfo();
				for (auto& i : iq)
					dxInfo += i;
			}
			type = "EnExDXInfo";
			file += location.file_name();
			func += location.function_name();
			line += std::to_string(location.line());

			errMsg += enNL + dxInfo +enNL+ file + enNL + line;
		}
	};
	class errchk
	{
	public:

		static void hres(HRESULT hr, const std::source_location location =
			std::source_location::current())
		{
			if (hr == S_OK) return;

			throw EnExHR(hr, location);
		};
		static void hres(HRESULT hr, std::string msg, const std::source_location location =
			std::source_location::current())
		{
			if(hr == S_OK)
				return;
			throw EnExcept(msg, location);
		};
	};
}


