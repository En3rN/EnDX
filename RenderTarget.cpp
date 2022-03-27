#include "RenderTarget.h"
#include "enexception.h"
#include <variant>
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
	RenderTarget::RenderTarget(Resource& resource)
	{
		D3D11_RESOURCE_DIMENSION resourceDimention = resource.GetDimension();
		D3D11_RENDER_TARGET_VIEW_DESC RTVdesc{};
		switch(resourceDimention)
		{
		case D3D11_RESOURCE_DIMENSION_UNKNOWN:			
			break;
		case D3D11_RESOURCE_DIMENSION_BUFFER:
		{
			D3D11_BUFFER_DESC desc{};
			ID3D11Buffer* r = static_cast<ID3D11Buffer*>(resource.GetP());
			r->GetDesc(&desc);
		}
			break;
		case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
		{
			D3D11_TEXTURE1D_DESC desc{};
			ID3D11Texture1D* r = static_cast<ID3D11Texture1D*>(resource.GetP());
			r->GetDesc(&desc);
			RTVdesc.Format = desc.Format;
			RTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1D;
			RTVdesc.Texture1D.MipSlice = 0;
		}
			break;
		case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
		{
			D3D11_TEXTURE2D_DESC desc{};
			ID3D11Texture2D* r = static_cast<ID3D11Texture2D*>(resource.GetP());
			r->GetDesc(&desc);
			RTVdesc.Format = desc.Format;
			RTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			RTVdesc.Texture2D.MipSlice = 0;
		}
			break;
		case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
		{
			D3D11_TEXTURE3D_DESC desc{};
			ID3D11Texture3D* r = static_cast<ID3D11Texture3D*>(resource.GetP());
			r->GetDesc(&desc);
			RTVdesc.Format = desc.Format;
			RTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
			RTVdesc.Texture3D.MipSlice = 0;
			RTVdesc.Texture3D.FirstWSlice = 0;
			RTVdesc.Texture3D.WSize = -1;
		}
			break;
		default:
			break;
		}
		errchk::hres(pDevice->CreateRenderTargetView(resource.GetP(), &RTVdesc, &m_rtv));

		

	}
	bool RenderTarget::Set(DepthStencil* depthStencil = nullptr)
	{
		pContext->OMSetRenderTargets(1, m_rtv.GetAddressOf(), depthStencil->Get());
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
		m_rtv->GetResource(&resource);
		return resource;
	}
}
