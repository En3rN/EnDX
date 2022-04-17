#pragma once
#include "GfxResources.h"
#include "vec.h"
#include "Resource.h"
#include<vector>

namespace En3rN::DX
{	
	class RenderTarget : public GfxResources
	{
	public:
		struct Ptr { using Container = std::vector<ID3D11RenderTargetView*>; };
		using Container = std::vector<RenderTarget>;
		RenderTarget(const std::string& name ,uint32_t width, uint32_t height, bool makeAsMultiView = false);
		RenderTarget(const std::string& name, const Resource& resource);
		const std::string& GetName() const { return m_name; }
		ID3D11RenderTargetView* GetP() const		{ return m_rtv.Get(); }
		ID3D11RenderTargetView** GetPP()	{ return m_rtv.GetAddressOf(); }
		ID3D11RenderTargetView** operator & () { return &m_rtv; }

		bool Clear(Vec4f color = { 0,0,0,0 });
		Resource GetResource() const;

	private:
		std::string m_name;		
		ComPtr<ID3D11RenderTargetView> m_rtv;
	};
}

