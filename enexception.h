#pragma once
#include "enWin.h"
#include "enString.h"
#include "InfoManager.h"
#include <string>
#include <system_error>
#include <d3d11.h>

#define EnExParam  __FILE__, __LINE__

namespace En3rN
{
	
	class EnExcept : public std::exception
	{
	public:
		EnExcept() = default;
		EnExcept(std::string msg, std::string afile, int aline)
		{
			file += afile;
			line += std::to_string(aline);
			errMsg += msg;
			errMsg += enNL + file + enNL + line;
		}
		const char* what() { return errMsg.c_str(); }
		const char* GetType() { return type.c_str(); }
	protected:
		std::string type = "EnExcept";
		std::string file = "[File] ";
		std::string line = "[Line] ";
		std::string errMsg = "";
		std::string dxInfo = "[DXInfo] ";
		
	};

	class EnExHR : public EnExcept
	{
	public:
		EnExHR() = default;
		EnExHR(HRESULT hr, std::string afile, int aline)
		{
			if (!En3rN::DX::InfoManager::Empty()) {
				auto iq = En3rN::DX::InfoManager::GetInfo();
				for (auto& i : iq)
					dxInfo += i;
			}
			type = "EnExHR";
			file += afile;
			line += std::to_string(aline);
			errMsg += std::system_category().message(hr);
			
			errMsg += enNL + dxInfo + enNL + file + enNL + line;
		}
	};
	class EnExHRDXInfo : public EnExHR
	{
	public:
		EnExHRDXInfo(HRESULT hr, ID3D11InfoQueue* dxInfoQ, std::string afile, int aline)
		{
			type = "EnExDXInfo";
			file += afile;
			line += std::to_string(aline);
			errMsg += std::system_category().message(hr);
			
			for (auto i = 0; i < dxInfoQ->GetNumStoredMessages(); i++)
			{
				SIZE_T message_size = 0;
				dxInfoQ->GetMessageA(i, nullptr, &message_size); //,__FILE__,__LINE__); //get the size of the message

				D3D11_MESSAGE* message = (D3D11_MESSAGE*)malloc(message_size); //allocate enough space
				dxInfoQ->GetMessageA(i, message, &message_size);//, __FILE__, __LINE__); //get the actual message
				if (message)dxInfo += message->pDescription;
				dxInfo += enNL;
				free(message);
			}
			errMsg += enNL + dxInfo +enNL+ file + enNL + line;
		}
	};
	class errchk
	{
	public:

		static void hres(HRESULT hr, std::string file, int line)
		{
			if (hr == S_OK) return;

			throw EnExHR(hr, file, line);
		};
		inline static void test(HRESULT hr)
		{
			throw EnExHR(hr, __FILE__, __LINE__);
		};
		static void hres(HRESULT hr, ID3D11InfoQueue* dxInfoQ, std::string file, int line)
		{
			if (hr == S_OK) return;
			throw EnExHRDXInfo(hr, dxInfoQ, file, line);
		};
	};
}


