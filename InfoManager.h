#pragma once
#include "end3d11.h"
#include "iHandle.h"
#include <deque>
#include <string>

namespace En3rN::DX
{
	class InfoManager :  unPtr(InfoManager)
	{
	public:
		using InfoQ = std::deque<std::string>;
		InfoManager(ID3D11Device& device);
		~InfoManager() = default;
		static void Update();
		static bool Empty();
		static InfoQ GetInfo();
	private:
		static InfoManager* infoManager;
		ComPtr<ID3D11InfoQueue> pInfoQ;
		InfoQ infoQ;
	};
}

