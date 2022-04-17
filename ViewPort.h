#pragma once
#include "end3d11.h"
#include <vector>

namespace En3rN::DX
{
	struct ViewPort
	{
		using Container = std::vector<D3D11_VIEWPORT>;
		D3D11_VIEWPORT ViewPort;
	};
}

