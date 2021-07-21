#pragma once
#include "Texture.h"
#include <stdint.h>
#include <vector>


namespace En3rN::DX
{
	class Material
	{
	public:
		enum class MapType {Texture, Normal, Specular};
		using Container = std::vector<Material>;
		Material();
		void Add(MapType type, Texture::handle);
		bool HasDiffuseMap();
		bool HasSpecularMap();
		bool HasNormalMap();
		~Material() = default;
		
	private:
		uint32_t				m_diffuse;
		uint8_t					m_specularIntensity;
		uint8_t					m_specularPower;
		bool					m_hasTexture;
		bool					m_hasSpecularmap;
		bool					m_hasNormalmap;
		Texture::Container		m_textures;
	};
}

