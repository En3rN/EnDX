#include "RenderTarget.h"
#include "enexception.h"
#include <algorithm>
namespace En3rN::DX
{
	RenderTarget::RenderTarget(uint32_t width, uint32_t height)
	{
		D3D11_TEXTURE2D_DESC texDesc{};
		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		
		ComPtr<ID3D11Texture2D> tex;
		errchk::hres(pDevice->CreateTexture2D(&texDesc, nullptr, &tex));

		D3D11_RENDER_TARGET_VIEW_DESC desc{};
		desc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension = D3D11_RTV_DIMENSION::D3D11_RTV_DIMENSION_TEXTURE2D;
		errchk::hres(pDevice->CreateRenderTargetView(tex.Get(), &desc, &m_rtv));		
	}
	bool RenderTarget::Set(DepthStencil& depthStencil)
	{
		pContext->OMSetRenderTargets(1, m_rtv.GetAddressOf(), depthStencil.Get());
		return true;
	}
	bool RenderTarget::Clear(Vec4f color)
	{
		pContext->ClearRenderTargetView(m_rtv.Get(), &color.x);
		return true;
	}
	auto RenderTarget::GetResource()
	{
		ComPtr<ID3D11Resource> resource;
		ComPtr<ID3D11Texture2D> tex;
		D3D11_RESOURCE_DIMENSION dimension{};
		m_rtv->GetResource(&resource);
		resource->GetType(&dimension);
		resource.As(&tex);
		return tex;
	}
}
