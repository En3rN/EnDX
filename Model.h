#pragma once
#include "Mesh.h"
#include "Material.h"
#include <vector>

namespace En3rN::DX
{
	class Model 
	{
		Model(const std::string& name);
	public:
	protected:
		Mesh::Container			m_meshes;
		Material::Container		m_material;
		std::string				m_name;
	};
}

