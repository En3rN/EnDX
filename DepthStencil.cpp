#include "DepthStencil.h"
#include "enexception.h"

namespace En3rN::DX
{
    DepthStencil::DepthStencil(UINT width, UINT height)
    {
		// DepthStencil View
		D3D11_DEPTH_STENCIL_VIEW_DESC dsVDesc{};
		dsVDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsVDesc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
		
		ComPtr<ID3D11Texture2D> pDepthBuf;
		D3D11_TEXTURE2D_DESC depthTexDesc{};
		depthTexDesc.Width = width;
		depthTexDesc.Height = height;
		depthTexDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.SampleDesc.Count = 1;
		depthTexDesc.SampleDesc.Quality = 0;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;
		errchk::hres(pDevice->CreateTexture2D(&depthTexDesc, nullptr, &pDepthBuf));
		errchk::hres(pDevice->CreateDepthStencilView(pDepthBuf.Get(), &dsVDesc, m_dsv.GetAddressOf()));
    }

    ID3D11DepthStencilView* DepthStencil::Get()
    {
        return m_dsv.Get();
    }
	auto DepthStencil::Clear(UINT flags, float depth, UINT8 stencil)
	{
		pContext->ClearDepthStencilView(m_dsv.Get(), flags, depth, stencil);
	}
}
