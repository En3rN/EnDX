#include "DepthStencil.h"
#include "enexception.h"
#include "Resource.h"

namespace En3rN::DX
{
    DepthStencil::DepthStencil(UINT width, UINT height, DXGI_FORMAT format)
    {
		// DepthStencil View
		D3D11_DEPTH_STENCIL_VIEW_DESC dsVDesc{};
		dsVDesc.Format = format;
		dsVDesc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
		
		ComPtr<ID3D11Texture2D> pDepthBuf;
		D3D11_TEXTURE2D_DESC depthTexDesc{};
		depthTexDesc.Width = width;
		depthTexDesc.Height = height;
		depthTexDesc.Format = format;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.SampleDesc.Count = 1;
		depthTexDesc.SampleDesc.Quality = 0;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;
		errchk::hres(GetDevice()->CreateTexture2D(&depthTexDesc, nullptr, &pDepthBuf));
		errchk::hres(GetDevice()->CreateDepthStencilView(pDepthBuf.Get(), &dsVDesc, &m_dsv));
    }

	DepthStencil::DepthStencil( Resource& resource, DXGI_FORMAT format ) {
		auto tDesc = std::get<2>( resource.GetDesc() );
		D3D11_DEPTH_STENCIL_VIEW_DESC dsVDesc{};
		dsVDesc.Format = format;
		dsVDesc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
		errchk::hres(GetDevice()->CreateDepthStencilView(resource.GetP(), &dsVDesc, &m_dsv));
	}

    ID3D11DepthStencilView* DepthStencil::GetP()
    {
        return m_dsv.Get();
    }
	bool DepthStencil::Clear(UINT flags, float depth, UINT8 stencil)
	{
		GetContext()->ClearDepthStencilView(m_dsv.Get(), flags, depth, stencil);
		return true;
	}
}
