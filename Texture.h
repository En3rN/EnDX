#pragma once
#include "end3d11.h"
#include "iBindable.h"
#include "enString.h"
#include "vec.h"
#include <filesystem>
#include <string>

namespace En3rN::DX
{
	class Texture : public Bindable 
	{
	public:
		class RenderTarget;
		class ShaderResourceView;
		class MultiView;
		class Staging;
		using handle = std::shared_ptr<Texture>;
		using Container = std::vector<std::shared_ptr<Texture>>;
		enum class Type{Default, CubeMap};
		explicit Texture(std::filesystem::path file, UINT slot=0, Type type= Type::Default);
		explicit Texture(ComPtr<ID3D11Texture2D> tex, UINT slot, Type type = Type::Default);
		void Bind();
	private:
		UINT							 m_slot;
		ComPtr<ID3D11Texture2D>          m_texture;
		ComPtr<ID3D11ShaderResourceView> m_srv;
	};
	
	

}

