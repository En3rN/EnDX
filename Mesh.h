#pragma once
#include "end3d11.h"
#include "Buffer.h"
#include "Topology.h"
#include "enBuffer.h"
#include "Material.h"
#include "Shader.h"
#include "InputLayout.h"
#include "Teqnique.h"
#include <string>
#include <vector>

struct aiMesh;

namespace En3rN::DX
{
	class Mesh
	{
		//friend class Job;
	public:
		using Container = std::vector<Mesh>;
		using Index = uint32_t;
		
		
		Mesh(const VertexBuffer::handle& vertexbuffer, 
			 const IndexBuffer::handle& indexbuffer,
			 const Topology::handle& topology,
			 const VertexShader::handle& vertexshader,
			 const InputLayout::handle& inputlayout,
			 Material::Index materialIndex);
		Mesh(const aiMesh* aimesh, Material::Index offset =0);
		Mesh() = default;
		//Mesh(const Mesh& other) = default;
		Mesh(Mesh&& other) noexcept = default ;
		//Mesh& operator = (const Mesh& other) = default;
		Mesh& operator = (Mesh && other) noexcept = default;
		~Mesh() = default;
		
		const std::string&			GetName()		   const;
		const VertexBuffer::handle& GetVertexBuffer()  const;
		const IndexBuffer::handle&	GetIndexBuffer()   const;
		Material::Index				GetMaterialIndex() const;
		const Teqnique::Container&	GetTeqniques()	   const;
		Teqnique::Container&		Teqniques();
		
		void SetName(const std::string& name);
		void SetVertexBuffer(VertexBuffer::handle&& vertexBuffer);
		void SetIndexBuffer(IndexBuffer::handle&& indexBuffer);
		void SetMaterialIndex(Material::Index index);
		void AddTeqnique(Teqnique teqnique, const Material::Container& materials);
		
		void Update(float dt);
		void Bind() const;

	private:
		enBuffer					m_buffer;
		std::string					m_name;
		Topology::handle			m_topology;
		VertexBuffer::handle		m_vertexBuffer;
		IndexBuffer::handle			m_indexBuffer;
		VertexShader::handle		m_vertexShader;
		InputLayout::handle			m_inputLayout;
		Material::Index				m_materialIndex;
		Teqnique::Container			m_teqniques;
	};
}

