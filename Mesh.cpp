#include "Mesh.h"
#include "BindableManager.h"
#include <assimp\mesh.h>
#include <assert.h>

namespace En3rN::DX
{	
	Mesh::Mesh(const std::shared_ptr<VertexBuffer>& vertexbuffer, const std::shared_ptr<IndexBuffer>& indexbuffer, Material::Index materialIndex) :
		m_vertexBuffer(vertexbuffer), m_indexBuffer(indexbuffer), m_materialIndex(materialIndex) {}
	Mesh::Mesh(const aiMesh* aimesh): 
		m_name{aimesh->mName.C_Str()}, m_materialIndex(aimesh->mMaterialIndex)
	{
		if (aimesh->HasPositions())
			m_buffer.add_element(aimesh->mVertices, "Position", sizeof(aiVector3D), aimesh->mNumVertices);
	
		for (auto i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i) {
			if (aimesh->HasTextureCoords(i)) {
				std::string semantic("TexCoord[" + std::to_string(i) + ']');
				m_buffer.add_element(aimesh->mTextureCoords[i], semantic, sizeof(aiVector3D), aimesh->mNumVertices);
			}
		}

		if(aimesh->HasNormals())
			m_buffer.add_element(aimesh->mNormals,"Normals", sizeof(aiVector3D), aimesh->mNumVertices);
		if (aimesh->HasTangentsAndBitangents()){
			m_buffer.add_element(aimesh->mTangents, "Tangents", sizeof(aiVector3D) , aimesh->mNumVertices);
			m_buffer.add_element(aimesh->mBitangents, "BiTangents", sizeof(aiVector3D) , aimesh->mNumVertices);
		}
		for (auto i = 0u; i < AI_MAX_NUMBER_OF_COLOR_SETS; ++i){
			if (aimesh->HasVertexColors(i)){
				std::string semantic("Color[" + std::to_string(i) + ']');
				m_buffer.add_element(aimesh->mColors[i], semantic, sizeof(aiColor4D), aimesh->mNumVertices);
			}
		}
		enBuffer::Indecies indecies;
		for (auto face = 0u; face < aimesh->mNumFaces; ++face)
			for (auto index = 0u; index < aimesh->mFaces[face].mNumIndices; ++index)
				indecies.push_back(aimesh->mFaces[face].mIndices[index]);
		m_topology = BindableManager::Query<Topology>((D3D_PRIMITIVE_TOPOLOGY)aimesh->mPrimitiveTypes);
		m_vertexBuffer = std::make_shared<VertexBuffer>(m_buffer, m_name);
		m_indexBuffer = std::make_shared<IndexBuffer>(indecies, m_name);
		m_vertexShader = BindableManager::Query<VertexShader>("VertexShader.cso");
		m_inputLayout = BindableManager::Query<InputLayout>(m_vertexShader->GetSignatures(), m_vertexShader->GetBlob(), GetName());
		
	}
	
	void Mesh::SetVertexBuffer(VertexBuffer::handle&& vertexBuffer)
	{
		m_vertexBuffer = std::move(vertexBuffer);
	}

	void Mesh::SetIndexBuffer(IndexBuffer::handle&& indexBuffer)
	{
		m_indexBuffer = std::move(indexBuffer);
	}

	void Mesh::SetMaterialIndex(Material::Index index)
	{
		m_materialIndex = index;
	}

	void Mesh::AddTeqnique(Teqnique::Base::handle teqnique)
	{
		m_teqniques.push_back(teqnique);
	}

	void Mesh::Update(float dt)
	{

	}

	void Mesh::Bind() const
	{
		m_topology->Bind();
		m_vertexShader->Bind();
		m_vertexBuffer->Bind();
		m_indexBuffer->Bind();
		m_inputLayout->Bind();
	}
}
