#include "Texture.h"
#include "..\DirectXTex\DirectXTex\DirectXTex.h"
#include "enexception.h"
#include "logger.h"

namespace En3rN::DX
{
	Texture::Texture(std::wstring file , Type type, DirectX::TEX_FILTER_FLAGS flags)
	{
		
		using namespace DirectX;
		std::wstring folder = L"Assets/Textures/";
		std::wstring fullpath = folder + file;

		DirectX::WIC_FLAGS wflags{};
		DirectX::ScratchImage simg{};
		DirectX::TexMetadata meta{};
		DirectX::LoadFromWICFile(fullpath.c_str(), wflags, &meta, simg);

		if (simg.GetMetadata().format != DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM || flags != TEX_FILTER_DEFAULT)
		{
			DirectX::ScratchImage converted{};
			DirectX::Convert(*simg.GetImages(),
				DXGI_FORMAT_R8G8B8A8_UNORM,
				flags,
				DirectX::TEX_THRESHOLD_DEFAULT,
				converted);
			simg = std::move(converted);
		};
		
		
		D3D11_TEXTURE2D_DESC tdesc{};
		tdesc.Width = simg.GetMetadata().width;
		tdesc.Height = simg.GetMetadata().height;
		tdesc.MipLevels = simg.GetMetadata().mipLevels;
		tdesc.ArraySize = simg.GetMetadata().arraySize;
		tdesc.Format = simg.GetMetadata().format;
		tdesc.SampleDesc.Count = 1;
		tdesc.SampleDesc.Quality = 0;
		tdesc.Usage = D3D11_USAGE_DEFAULT;
		tdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		tdesc.CPUAccessFlags = 0;
		tdesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		D3D11_SUBRESOURCE_DATA subres{};
		subres.pSysMem = simg.GetImage(0,0,0);
		subres.SysMemPitch = simg.GetImage(0, 0, 0)->rowPitch;
		subres.SysMemSlicePitch = simg.GetImage(0, 0, 0)->slicePitch;
		
		D3D11_SHADER_RESOURCE_VIEW_DESC srvdesc{};		
		srvdesc.Format = tdesc.Format;
		srvdesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
		srvdesc.Texture2D.MostDetailedMip = 0;
		srvdesc.Texture2D.MipLevels = 1;

		switch (type)
		{
		case En3rN::DX::Texture::Type::Default:
			break;
		case En3rN::DX::Texture::Type::Cube:
		{
			tdesc.Width;
			tdesc.Height; // /= 3
			tdesc.ArraySize = 6;
			tdesc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
			UINT sysMemPitch = subres.SysMemPitch = 12;
			srvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			Rect rect;
			rect.w = tdesc.Width / 4;
			rect.h = tdesc.Height / 3;

			ScratchImage cube;
			cube.InitializeCube(simg.GetMetadata().format, rect.w, rect.h, 1, 1);


			std::vector<uint16_t> srcImgIndexes{ 6,4,1,9,7,5 };
			D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_POSITIVE_X;
			D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_NEGATIVE_X;
			D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_POSITIVE_Y;
			D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_NEGATIVE_Y;
			D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_POSITIVE_Z;
			D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_NEGATIVE_Z;

			D3D11_SUBRESOURCE_DATA subres[6]{};
			for (auto i = 0; i < 6; i++)
			{
				auto& srcImgIndex = srcImgIndexes[i];
				rect.x = (srcImgIndex % 4) * rect.w;
				rect.y = (srcImgIndex / 4) * rect.w;
				subres[i].SysMemPitch = sysMemPitch;
				DirectX::CopyRectangle(*simg.GetImages(), rect, *cube.GetImage(0, i, 0), TEX_FILTER_DEFAULT, 0, 0);
			}
			
			ScratchImage rotated[6];
			int i = 0;
			FlipRotate(*cube.GetImage(0, i, 0), TEX_FR_FLIP_HORIZONTAL, rotated[i]);i++;
			FlipRotate(*cube.GetImage(0, i, 0), TEX_FR_FLIP_HORIZONTAL, rotated[i]);i++;
			FlipRotate(*cube.GetImage(0, i, 0), TEX_FR_FLIP_VERTICAL, rotated[i]); i++;
			FlipRotate(*cube.GetImage(0, i, 0), TEX_FR_FLIP_VERTICAL, rotated[i]);i++;
			FlipRotate(*cube.GetImage(0, i, 0), TEX_FR_FLIP_HORIZONTAL, rotated[i]); i++;
			FlipRotate(*cube.GetImage(0, i, 0), TEX_FR_FLIP_HORIZONTAL, rotated[i]);i++;
			rect.x = 0; rect.y = 0;
			simg.InitializeCube(cube.GetMetadata().format, rect.w, rect.h, 1, 1);
			for (auto i = 0; i < 6; i++)
				CopyRectangle(*rotated[i].GetImages(), rect, *simg.GetImage(0, i, 0), TEX_FILTER_DEFAULT, 0, 0);
			
			break;
		}
		case En3rN::DX::Texture::Type::SkyBox:
		{
			tdesc.Width;
			tdesc.Height; // /= 3
			tdesc.ArraySize = 6;
			tdesc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
			UINT sysMemPitch = subres.SysMemPitch = 12;
			srvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			Rect rect;
			rect.w = tdesc.Width / 4;
			rect.h = tdesc.Height / 3;

			ScratchImage cube;
			cube.InitializeCube(simg.GetMetadata().format, rect.w, rect.h, 1, 1);

			std::vector<uint16_t> srcImgIndexes{ 6,4,1,9,5,7 };
			D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_POSITIVE_X;
			D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_NEGATIVE_X;
			D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_POSITIVE_Y;
			D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_NEGATIVE_Y;
			D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_POSITIVE_Z;
			D3D11_TEXTURECUBE_FACE::D3D11_TEXTURECUBE_FACE_NEGATIVE_Z;

			D3D11_SUBRESOURCE_DATA subres[6]{};
			for (auto i = 0; i < 6; i++)
			{
				auto& srcImgIndex = srcImgIndexes[i];
				rect.x = (srcImgIndex % 4) * rect.w;
				rect.y = (srcImgIndex / 4) * rect.w;
				subres[i].SysMemPitch = sysMemPitch;
				DirectX::CopyRectangle(*simg.GetImages(), rect, *cube.GetImage(0, i, 0), TEX_FILTER_DEFAULT, 0, 0);
			}
			simg = std::move(cube);
			break;
		}
		}
		errchk::hres(CreateTexture(pDevice, simg.GetImages(), simg.GetImageCount(), simg.GetMetadata(), &pTexture), EnExParam);
		errchk::hres(DirectX::CreateShaderResourceView(pDevice, simg.GetImages(), simg.GetImageCount(), simg.GetMetadata(), &pTextureView), EnExParam);
	}
	void Texture::Bind()
	{
		pContext->PSSetShaderResources(0, 1, pTextureView.GetAddressOf());
	}
}
