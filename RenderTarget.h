#pragma once
#include "GfxResources.h"
#include "vec.h"
#include "DepthStencil.h"
#include<vector>

namespace En3rN::DX
{
	class RenderTarget : public GfxResources
	{
	public:
		RenderTarget(uint32_t width, uint32_t height);
		ID3D11RenderTargetView** Get() { return m_rtv.GetAddressOf(); }
		ID3D11RenderTargetView** operator & () { return &m_rtv; }
		bool Set(DepthStencil& depthStencil);
		bool Clear(Vec4f color = { 0,0,0,0 });
		auto GetResource();

	private:
		ComPtr<ID3D11RenderTargetView> m_rtv;
	};
}

