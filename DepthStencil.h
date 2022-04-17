#pragma once
#include "GfxResources.h"
#include <vector>
namespace En3rN::DX
{
	class DepthStencil : public GfxResources
	{
	public:
		class State;
		using Container = std::vector<DepthStencil>;
		using Ptr = ID3D11DepthStencilView*;
		DepthStencil(UINT width, UINT height);
		Ptr GetP();		
		bool Clear(UINT flags = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, float depth = 1.f, UINT8 stencil = 0);
	private:
		ComPtr<ID3D11DepthStencilView> m_dsv;
	};
}

