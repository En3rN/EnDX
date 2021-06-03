#pragma once
#include "Texture.h"
#include <stdint.h>
#include <vector>


namespace En3rN::DX
{
	class Material
	{
	public:
		using Container = std::vector<Material>;
		Material();
		bool HasDiffuseMap();
		bool HasSpecularMap();
		bool HasNormalMap();

		~Material() = default;
		
	private:
		uint32_t				m_diffuse;
		uint8_t					m_specular;
		bool					m_texture;
		bool					m_specularmap;
		bool					m_normalmap;
		Texture::Container		m_textures;
	};
}

