#pragma once
#include "end3d11.h"
#include "iBindable.h"
#include "..\DirectXTex\DirectXTex\DirectXTex.h"
#include <string>

namespace En3rN::DX
{
	class Texture : public Bindable 
	{
	public:
		using Container = std::vector<ID3D11ShaderResourceView*>;
		enum class Type{Default, Cube, SkyBox};
		Texture(std::wstring file, Type type= Type::Default, DirectX::TEX_FILTER_FLAGS flags= DirectX::TEX_FILTER_FLAGS::TEX_FILTER_DEFAULT);
		void Bind();
	private:
		ComPtr<ID3D11Resource> pTexture;
		ComPtr<ID3D11ShaderResourceView> pTextureView;
	};
}

