#pragma once
#include "end3d11.h"
#include "iBindable.h"
#include "enString.h"
#include <filesystem>
#include <string>

namespace En3rN::DX
{
	class Texture : public Bindable 
	{
	public:
		using handle = std::shared_ptr<Texture>;
		using Container = std::vector<std::shared_ptr<Texture>>;
		enum class Type{Default, CubeMap};
		Texture(std::filesystem::path file, UINT slot=0, Type type= Type::Default);
		void Bind();
	private:
		UINT							 m_slot;
		ComPtr<ID3D11ShaderResourceView> pTextureView;
	};
	

}

