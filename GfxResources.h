#pragma once
#include "end3d11.h"

namespace En3rN::DX
{
	
	class Graphics;
	class GfxResources
	{
	public:		
	protected:
		static Graphics& GetGfx();
		static ID3D11DeviceContext* GetContext();
		static ID3D11Device* GetDevice();
		static IDXGISwapChain* GetSwapChain();		 
	};	
}

