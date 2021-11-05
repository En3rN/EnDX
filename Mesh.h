#pragma once
#include "end3d11.h"
#include "Buffer.h"
#include "Topology.h"
#include "enBuffer.h"
#include "Material.h"
#include "Shader.h"
#include "Entity.h"
#include "InputLayout.h"
#include "Teqnique.h"
#include <vector>

class aiMesh;

namespace En3rN::DX
{	
	
	class Mesh : public Entity
	{
		friend class Job;
	public:
		using Container = std::vector<Mesh>;
		
		
		Mesh(const std::shared_ptr<VertexBuffer>& vertexbuffer, const std::shared_ptr<IndexBuffer>& indexbuffer, Material::Index materialIndex);
		Mesh(const aiMesh* aimesh);
		Mesh() = default;
		//Mesh(const Mesh& other) = default;
		Mesh(Mesh&& other) noexcept = default ;
		//Mesh& operator = (const Mesh& other) = default;
		Mesh& operator = (Mesh && other) noexcept = default;
		~Mesh() = default;
		
		const std::string&					GetName()		   const{ return m_name; };
		const VertexBuffer::handle&			GetVertexBuffer()  const{ return m_vertexBuffer; };
		const IndexBuffer::handle&			GetIndexBuffer()   const{ return m_indexBuffer; };
		Material::Index						GetMaterialIndex() const{ return m_materialIndex; }
		const Teqnique::Base::Container&	GetTeqniques()	   const{ return m_teqniques; }
		
		void SetVertexBuffer		(VertexBuffer::handle&& vertexBuffer);
		void SetIndexBuffer			(IndexBuffer::handle&& indexBuffer);
		void SetMaterialIndex		(Material::Index index);
		void AddTeqnique			(Teqnique::Base::handle teqnique);
		
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
		Teqnique::Base::Container	m_teqniques;
	};
}

