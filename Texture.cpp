#include "Texture.h"
#include "..\DirectXTex\DirectXTex\DirectXTex.h"
#include "enexception.h"
#include "logger.h"
#include "BindableManager.h"
#include "Config.h"


namespace En3rN::DX
{
	Texture::Texture(std::filesystem::path file, UINT slot, Type type) : m_slot(slot)
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
		DirectX::ScratchImage simg{};
		DirectX::TexMetadata meta{};
		meta.mipLevels = 0;
		

		//std::wstring p = path.make_preferred().wstring();
		if (!std::filesystem::exists(path)) throw EnExcept("File not found!"+ path.string(), std::source_location::current());

		errchk::hres(DirectX::LoadFromWICFile(path.wstring().c_str(), wflags, &meta, simg));

		if (simg.GetMetadata().format != DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM)
		{
			DirectX::ScratchImage converted{};
			DirectX::Convert(*simg.GetImages(),
				DXGI_FORMAT_R8G8B8A8_UNORM,
				TEX_FILTER_FANT,
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

		pDevice->CheckMultisampleQualityLevels(tdesc.Format, tdesc.SampleDesc.Count, &tdesc.SampleDesc.Quality);

		D3D11_SUBRESOURCE_DATA subres{};
		subres.pSysMem = simg.GetImage(0, 0, 0);
		subres.SysMemPitch = simg.GetImage(0, 0, 0)->rowPitch;
		subres.SysMemSlicePitch = simg.GetImage(0, 0, 0)->slicePitch;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvdesc{};

		switch (type)
		{
		case Texture::Type::Default:
		{
			srvdesc.Format = tdesc.Format;
			srvdesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2DMS;
			srvdesc.Texture2D.MostDetailedMip = 0;
			srvdesc.Texture2D.MipLevels = -1;
			ScratchImage mips;
			GenerateMipMaps(*simg.GetImage(0, 0, 0), TEX_FILTER_FLAGS::TEX_FILTER_SEPARATE_ALPHA, (size_t)0, mips);
			simg = std::move(mips);
			break;
		}
		case Texture::Type::CubeMap:
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
				DirectX::CopyRectangle(*simg.GetImages(), rect, *cube.GetImage(0, i, 0), TEX_FILTER_DEFAULT, 0, 0);
				subres[i].SysMemPitch = sysMemPitch;
				subres[i].pSysMem = cube.GetImage(0, i, 0);
			}
			simg = std::move(cube);
			break;
		}

		}
		ComPtr<ID3D11Resource> pTexture;
		errchk::hres(CreateTexture(pDevice, simg.GetImages(), simg.GetImageCount(), simg.GetMetadata(), &pTexture));
		errchk::hres(CreateShaderResourceView(pDevice, simg.GetImages(), simg.GetImageCount(), simg.GetMetadata(), &pTextureView));		
	}	
	void Texture::Bind()
	{
		pContext->PSSetShaderResources(m_slot, 1, pTextureView.GetAddressOf());
	}
}