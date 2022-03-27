#pragma once
#include "GfxResources.h"
namespace En3rN::DX
{
	class DepthStencil : public GfxResources
	{
	public:
		DepthStencil(UINT width, UINT height);
		ID3D11DepthStencilView* Get();
		operator ID3D11DepthStencilView* () { return m_dsv.Get(); };
		auto Clear(UINT flags = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, float depth = 1.f, UINT8 stencil = 0);
	private:
		ComPtr<ID3D11DepthStencilView> m_dsv;
	};
}

