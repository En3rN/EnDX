#include "Texture.h"
#include "..\DirectXTex\DirectXTex\DirectXTex.h"
#include "enexception.h"
#include "logger.h"
#include "BindableManager.h"
#include "Config.h"
#include "Resource.h"


namespace En3rN::DX
{
	Texture::Texture(std::filesystem::path file, UINT slot, Flag type) : m_slot(slot)
	{
		using namespace DirectX;
		std::filesystem::path path= Config::GetFolders().assets;
		if (file.has_parent_path())
			path = std::move(file);
		else {
			path /= Config::GetFolders().textures;
			path /= std::move(file);
		}

		DirectX::WIC_FLAGS wflags{};
		wflags |= WIC_FLAGS::WIC_FLAGS_FILTER_FANT;
		TEX_FILTER_FLAGS texFlags{};
		texFlags |= TEX_FILTER_FANT | TEX_FILTER_SEPARATE_ALPHA;
		DirectX::ScratchImage simg{};
		DirectX::TexMetadata meta{};
		meta.mipLevels = 0;
		
		if (!std::filesystem::exists(path)) throw EnExcept("File not found!"+ path.string(), std::source_location::current());

		errchk::hres(DirectX::LoadFromWICFile(path.wstring().c_str(), wflags, &meta, simg));

		if (simg.GetMetadata().format != DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM)
		{
			DirectX::ScratchImage converted{};
			DirectX::Convert(*simg.GetImages(),
				DXGI_FORMAT_R8G8B8A8_UNORM,
				texFlags,
				DirectX::TEX_THRESHOLD_DEFAULT,
				converted);
			simg = std::move(converted);
		};

		/*HRESULT CheckMultisampleQualityLevels(
			[in]  DXGI_FORMAT Format,
			[in]  UINT        SampleCount,
			[out] UINT * pNumQualityLevels
		);*/

		D3D11_TEXTURE2D_DESC tdesc{};
		tdesc.Width = simg.GetMetadata().width;
		tdesc.Height = simg.GetMetadata().height;
		tdesc.MipLevels = 0;
		tdesc.ArraySize = simg.GetMetadata().arraySize;
		tdesc.Format = simg.GetMetadata().format;
		tdesc.SampleDesc.Count = 1;
		tdesc.SampleDesc.Quality = 0;
		tdesc.Usage = D3D11_USAGE_DEFAULT;
		tdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		tdesc.CPUAccessFlags = 0;
		tdesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		errchk::hres(pDevice->CheckMultisampleQualityLevels(
			tdesc.Format,
			tdesc.SampleDesc.Count,
			&tdesc.SampleDesc.Quality));

		D3D11_SUBRESOURCE_DATA subres{};
		subres.pSysMem = simg.GetImage(0, 0, 0);
		subres.SysMemPitch = simg.GetImage(0, 0, 0)->rowPitch;
		subres.SysMemSlicePitch = simg.GetImage(0, 0, 0)->slicePitch;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvdesc{};

		switch (type)
		{
		case Texture::Flag::Default:
		{
			srvdesc.Format = tdesc.Format;
			srvdesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2DMS;
			srvdesc.Texture2D.MostDetailedMip = 0;
			srvdesc.Texture2D.MipLevels = -1;
			ScratchImage mips;
			GenerateMipMaps(*simg.GetImage(0, 0, 0), texFlags, (size_t)0, mips);
			simg = std::move(mips);
			break;
		}
		case Texture::Flag::CubeMap:
		{
			tdesc.Width;
			tdesc.Height; // /= 3
			tdesc.ArraySize = 6;
			tdesc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
			UINT sysMemPitch = subres.SysMemPitch = 12;
			srvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvdesc.TextureCube.MipLevels = -1;
			Rect rect;
			rect.w = tdesc.Width / 4;
			rect.h = tdesc.Height / 3;

			ScratchImage cube;
			cube.InitializeCube(simg.GetMetadata().format, rect.w, rect.h, 1, 1);

			std::vector<uint16_t> srcImgIndecies{ 6,4,1,9,5,7 };
			D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_POSITIVE_X; 
			D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_NEGATIVE_X;
			D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_POSITIVE_Y;
			D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_NEGATIVE_Y;
			D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_POSITIVE_Z;
			D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_NEGATIVE_Z;

			D3D11_SUBRESOURCE_DATA subres[6]{};
			for (auto i = 0; i < 6; i++)
			{
				auto& srcImgIndex = srcImgIndecies[i];
				rect.x = (srcImgIndex % 4) * rect.w;
				rect.y = (srcImgIndex / 4) * rect.w;
				DirectX::CopyRectangle(*simg.GetImages(), rect, *cube.GetImage(0, i, 0), texFlags, 0, 0);
				subres[i].SysMemPitch = sysMemPitch;
				subres[i].pSysMem = cube.GetImage(0, i, 0);
			}
			simg = std::move(cube);
			break;
		}

		}
		ComPtr<ID3D11Resource> pResource;
		errchk::hres(CreateTexture(pDevice, simg.GetImages(), simg.GetImageCount(), simg.GetMetadata(), &pResource));
		errchk::hres(CreateShaderResourceView(pDevice, simg.GetImages(), simg.GetImageCount(), simg.GetMetadata(), &m_srv));		
	}
	Texture::Texture(Resource resource, UINT slot, Flag flag, std::string_view tag ) : m_slot(slot)
	{
		auto dimention = resource.GetDimension();

		auto srvDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC();

		if( flag == Flag::NoCopy )
			resource = Resource( dimention, resource.GetDesc() );

		switch(dimention)
		{
		case D3D11_RESOURCE_DIMENSION_UNKNOWN:
			srvDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC(D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_UNKNOWN);
			break;
		case D3D11_RESOURCE_DIMENSION_BUFFER:
		{
			auto buf = resource.GetResourceType<ID3D11Buffer>();
			auto tDesc = std::get<0>(resource.GetDesc());
			srvDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC(buf.Get(), DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0, tDesc.ByteWidth / tDesc.StructureByteStride);
			if(tDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
				break;
			ComPtr<ID3D11Buffer> tex;
			tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			pDevice->CreateBuffer(&tDesc, nullptr, &tex);
			if(flag != Flag::NoCopy)
				pContext->CopyResource(tex.Get(), resource.GetP());
			resource = Resource(std::move(tex));
			break;
		}
		case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
		{
			auto tex1 = resource.GetResourceType<ID3D11Texture1D>();
			auto tDesc = std::get<1>(resource.GetDesc());
			srvDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC(tex1.Get(), D3D11_SRV_DIMENSION_TEXTURE1D, tDesc.Format);
			if(tDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
				break;
			
			ComPtr<ID3D11Texture1D> tex;
			tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			pDevice->CreateTexture1D(&tDesc, nullptr, &tex);
			if(flag != Flag::NoCopy)
				pContext->CopyResource(tex.Get(), resource.GetP());
			resource = Resource(std::move(tex));
			break;
		}
		case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
		{
			auto tex2 = resource.GetResourceType<ID3D11Texture2D>();
			auto tDesc = std::get<2>(resource.GetDesc());
			srvDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC(tex2.Get(), D3D11_SRV_DIMENSION_TEXTURE2D, tDesc.Format);
			if(tDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
				break;
			ComPtr<ID3D11Texture2D> tex;
			tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			pDevice->CreateTexture2D(&tDesc, nullptr, &tex);
			if(flag != Flag::NoCopy)
				pContext->CopyResource(tex.Get(), resource.GetP());
			resource = Resource(std::move(tex));
			break;
		}
		case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
		{
			auto tex3 = resource.GetResourceType<ID3D11Texture3D>();
			auto tDesc = std::get<3>(resource.GetDesc());
			srvDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC(tex3.Get(), tDesc.Format);
			if(tDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
				break;
			tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			ComPtr<ID3D11Texture3D> tex;
			pDevice->CreateTexture3D(&tDesc, nullptr, &tex);
			if(flag!=Flag::NoCopy) 
				pContext->CopyResource(tex.Get(), resource.GetP());
			resource = Resource(std::move(tex));
			break;
		}
		default:
			break;
		}

		errchk::hres(pDevice->CreateShaderResourceView(resource.GetP(), &srvDesc, &m_srv));
	}
	
	Resource Texture::GetResource()
	{
		ComPtr<ID3D11Resource> resource;
		m_srv->GetResource(&resource);
		return Resource(resource);
	}

	void Texture::SetBindslot(UINT slot)
	{
		m_slot = slot;
	}
	
	void Texture::Bind()
	{
		pContext->PSSetShaderResources(m_slot, 1, m_srv.GetAddressOf());
	}
}