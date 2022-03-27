#pragma once
#include "GfxResources.h"

namespace En3rN::DX
{
	class Resource : public GfxResources
	{
	public:
		D3D11_RESOURCE_DIMENSION GetDimension();
		ID3D11Resource* GetP() { return m_res.Get(); };
		ID3D11Resource** GetPP() { return m_res.GetAddressOf(); }
		ID3D11Resource** ReleaseAndGetPP () { return m_res.ReleaseAndGetAddressOf(); }
		
	private:
		ComPtr<ID3D11Resource> m_res;
	};
}

