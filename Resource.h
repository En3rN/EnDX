#pragma once

#include "enexception.h"
#include "GfxResources.h";
#include <..\DirectXTex\DirectXTex\DirectXTex.h>
#include <variant>

namespace En3rN::DX
{
	class Resource : public GfxResources
	{		

	public:
		enum class Type { Unknown = -1, Buffer,Tex1D,Tex2D,Tex3D };
		using BindFlags = D3D11_BIND_FLAG;
		using MiscFlags = D3D11_RESOURCE_MISC_FLAG;
		using Dimention = D3D11_RESOURCE_DIMENSION;
		using DXPtr = ID3D11Resource*;
		using Desc_t = std::variant<
			D3D11_BUFFER_DESC,
			D3D11_TEXTURE1D_DESC,
			D3D11_TEXTURE2D_DESC,
			D3D11_TEXTURE3D_DESC>;
		Resource() = default;
		Resource(const Resource& other) = default;
		Resource(Resource&& other) noexcept = default;
		Resource& operator = (const Resource& other) = default;
		Resource& operator = (Resource && other) noexcept = default;
		~Resource() = default;

		Resource(UINT width, UINT height, UINT arr, UINT bindFlags = BindFlags::D3D11_BIND_SHADER_RESOURCE, UINT miscFlags = 0 );
		Resource( Dimention dimention, Desc_t );

		Resource(ComPtr<ID3D11Resource> resource) : m_res(std::move(resource)){}
		D3D11_RESOURCE_DIMENSION GetDimension() const;
		ID3D11Resource* GetP() const { return m_res.Get(); };
		ID3D11Resource*const* GetPP() const { return m_res.GetAddressOf(); }
		ID3D11Resource** ReleaseAndGetPP () { return m_res.ReleaseAndGetAddressOf(); }		
		Desc_t GetDesc();
		DirectX::ScratchImage GetAsScratch();
		template <typename T>
		ComPtr<T> GetResourceType() {
			ComPtr<T> result;
			errchk::hres(m_res.As<T>(&result));
			return result;
		}

		
		
	private:
		ComPtr<ID3D11Resource> m_res;
	};
}

