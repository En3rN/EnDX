#include "Material.h"
namespace En3rN::DX
{

	Material::Material() : 
		m_diffuse(0), m_specularIntensity(0), m_specularPower(0),
		m_hasTexture(false),m_hasSpecularmap(false), m_hasNormalmap(false)
	{
	}
	void Material::Add(MapType type, Texture::handle)
	{

	}
	bool Material::HasDiffuseMap()
	{
		return m_hasTexture;
	}
	bool Material::HasSpecularMap()
	{
		return m_hasSpecularmap;
	}
	bool Material::HasNormalMap()
	{
		return m_hasNormalmap;
	}
}