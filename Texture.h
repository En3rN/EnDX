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
		enum class Type{Default, CubeMap};
		Texture(std::wstring file, Type type= Type::Default);
		static std::string GetKey(std::wstring filename, Type type= Type::Default);
		void Bind();
	private:
		//std::string filename;
		ComPtr<ID3D11Resource> pTexture;
		ComPtr<ID3D11ShaderResourceView> pTextureView;
	};
}

