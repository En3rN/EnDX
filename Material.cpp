#include "Material.h"
namespace En3rN::DX
{

	Material::Material() : 
		m_diffuse(0), m_specular(0), 
		m_texture(false),m_specularmap(false), m_normalmap(false)
	{
	}
	bool Material::HasDiffuseMap()
	{
		return m_texture;
	}
	bool Material::HasSpecularMap()
	{
		return m_specular;
	}
	bool Material::HasNormalMap()
	{
		return m_normalmap;
	}
}