#pragma once
#include "end3d11.h"
#include "iBindable.h"
#include "enString.h"
#include "vec.h"
#include "Resource.h"
#include "enexception.h"
#include <filesystem>
#include <string>

namespace En3rN::DX
{
	
	//aka ShadeResourceView
	class Texture : public Bindable 
	{
	public:
		class RenderTarget;		
		class MultiView;
		class Staging;
		class UAInput;
		using handle = std::shared_ptr<Texture>;
		using Container = std::vector<std::shared_ptr<Texture>>;
		using DXPtr = ID3D11ShaderResourceView*;
		enum class Flag{Default, CubeMap, NoCopy};

		Texture() = default;
		Texture(const Texture& other) = default;
		Texture(Texture&& other) noexcept = default;
		Texture& operator= (const Texture& other) = default;
		Texture& operator= (Texture&& other) noexcept = default;
		~Texture() = default;

		Texture(std::filesystem::path file, UINT slot=0, Flag type= Flag::Default);
		Texture( Resource resource, UINT slot = 0, Flag flag = Flag::Default, std::string_view tag = "" );		
		Resource GetResource();
		DXPtr GetP() {
			return m_srv.Get();
		}

		void SetBindslot(UINT slot);
		void Bind() override;
	protected:
		UINT							 m_slot{};
		ComPtr<ID3D11ShaderResourceView> m_srv;
	};
	class Texture::UAInput : public Texture
	{
	public:
		Texture::UAInput(Resource resource)
		{
			UINT stride{};
			auto tDesc = std::get<2>(resource.GetDesc());

			switch(tDesc.Format)
			{
			default:
				stride = 4;
				break;
			}
			D3D11_BUFFER_DESC desc{};
			desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS |
				D3D11_BIND_SHADER_RESOURCE;
			desc.ByteWidth = tDesc.Width * tDesc.Height * stride;
			desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			desc.StructureByteStride = stride;

			ComPtr<ID3D11Buffer> buffer;
			auto simg = resource.GetAsScratch();

			D3D11_SUBRESOURCE_DATA subres{};
			subres.pSysMem = simg.GetImage(0, 0, 0)->pixels;
			subres.SysMemPitch = simg.GetMetadata().width;

			errchk::hres(pDevice->CreateBuffer(&desc, &subres, &buffer));
			
			D3D11_BUFFER_DESC descBuf{};
			
			buffer->GetDesc(&descBuf);

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
			
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
			srvDesc.BufferEx.FirstElement = 0;

			srvDesc.Format = DXGI_FORMAT_UNKNOWN;
			srvDesc.BufferEx.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;

			errchk::hres(pDevice->CreateShaderResourceView(buffer.Get(), &srvDesc, &m_srv));
		}
	};
}

