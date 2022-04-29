#include "Resource.h"
#include "Texture.h"

namespace En3rN::DX
{
	Resource::Resource(UINT width, UINT height, UINT arr, UINT bindFlags, UINT miscFlags)
	{		
		D3D11_TEXTURE2D_DESC texDesc{};
		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = bindFlags;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = arr;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = miscFlags;

		ComPtr<ID3D11Texture2D> tex;
		GetDevice()->CreateTexture2D( &texDesc, nullptr, &tex );
		m_res = tex;
	}

	Resource::Resource( Dimention dimention, Desc_t desc) 
	{
		using std::get;
		switch( dimention ) {
		case D3D11_RESOURCE_DIMENSION_UNKNOWN:
			break;
		case D3D11_RESOURCE_DIMENSION_BUFFER:
		{
			auto tDesc = get<0>( desc );
			ComPtr<ID3D11Buffer> tex;
			GetDevice()->CreateBuffer( &tDesc, nullptr, &tex );
			m_res = tex;
			break;
		}
			
		case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
		{
			auto tDesc = get<1>( desc );
			ComPtr<ID3D11Texture1D> tex;
			GetDevice()->CreateTexture1D( &tDesc, nullptr, &tex );
			m_res = tex;
			break;
		}
			
		case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
		{
			auto tDesc = get<2>( desc );
			ComPtr<ID3D11Texture2D> tex;
			GetDevice()->CreateTexture2D( &tDesc, nullptr, &tex );
			m_res = tex;
			break;
		}
		case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
		{
			auto tDesc = get<3>( desc );
			ComPtr<ID3D11Texture3D> tex;
			GetDevice()->CreateTexture3D( &tDesc, nullptr, &tex );
			m_res = tex;
			break;
		}
		default:
			break;
		}
		
	}

	D3D11_RESOURCE_DIMENSION Resource::GetDimension() const
	{
		D3D11_RESOURCE_DIMENSION d{};
		m_res->GetType(&d);
		return d;
	}
	Resource::Desc_t Resource::GetDesc()
	{
		Desc_t result;


		D3D11_RESOURCE_DIMENSION resourceDimention = GetDimension();
		switch(resourceDimention)
		{
		case D3D11_RESOURCE_DIMENSION_UNKNOWN:
			break;
		case D3D11_RESOURCE_DIMENSION_BUFFER:
		{
			D3D11_BUFFER_DESC desc{};
			ID3D11Buffer* r = static_cast<ID3D11Buffer*>(GetP());
			r->GetDesc(&desc);
			result = desc;
		}
		break;
		case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
		{
			D3D11_TEXTURE1D_DESC desc{};
			ID3D11Texture1D* r = static_cast<ID3D11Texture1D*>(GetP());
			r->GetDesc(&desc);
			result = desc;
		}
		break;
		case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
		{
			D3D11_TEXTURE2D_DESC desc{};
			ID3D11Texture2D* r = static_cast<ID3D11Texture2D*>(GetP());
			r->GetDesc(&desc);
			result = desc;
		}
		break;
		case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
		{
			D3D11_TEXTURE3D_DESC desc{};
			ID3D11Texture3D* r = static_cast<ID3D11Texture3D*>(GetP());
			r->GetDesc(&desc);
			result = desc;
		}
		break;
		default:
			break;
		}
		return result;
	}

	DirectX::ScratchImage Resource::GetAsScratch()
	{
		if(!m_res) throw EnExcept("Resource is NULL");
		ComPtr<ID3D11Device> pDevice;
		ComPtr<ID3D11DeviceContext> pContext;
		m_res->GetDevice(&pDevice);
		GetDevice()->GetImmediateContext(&pContext);
		DirectX::ScratchImage simg{};
		DirectX::CaptureTexture(pDevice.Get(), pContext.Get(), GetP(), simg);
		return simg;
	}
	
}
