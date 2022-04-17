#include "Mesh.h"
#include "BindableManager.h"
#include "Scene.h"
#include <assimp\mesh.h>
#include <assert.h>

namespace En3rN::DX
{	
	Mesh::Mesh(const VertexBuffer::handle& vertexbuffer, const IndexBuffer::handle& indexbuffer, const Topology::handle& topology, const VertexShader::handle& vertexshader, const InputLayout::handle& inputlayout, Material::Index materialIndex):
		m_vertexBuffer(vertexbuffer), 
		m_indexBuffer(indexbuffer),
		m_topology(topology),
		m_vertexShader(vertexshader),
		m_inputLayout(inputlayout),
		m_materialIndex(materialIndex) 
	{}
	
	Mesh::Mesh(const aiMesh* aimesh, Material::Index offset):
		m_name{aimesh->mName.C_Str()}, m_materialIndex(aimesh->mMaterialIndex+offset)
	{
		if (aimesh->HasPositions())
			m_buffer.add_element(aimesh->mVertices, "Position", aimesh->mNumVertices, sizeof(aiVector3D));
	
		for (auto i = 0; i < aimesh->GetNumUVChannels(); ++i) {
			if (aimesh->HasTextureCoords(i)) {
				std::string semantic("TexCoord[" + std::to_string(i) + ']');
				m_buffer.add_element(aimesh->mTextureCoords[i], semantic, aimesh->mNumVertices, sizeof(aiVector3D));
			}
		}

		if(aimesh->HasNormals())
			m_buffer.add_element(aimesh->mNormals,"Normals", aimesh->mNumVertices, sizeof(aiVector3D));
		if (aimesh->HasTangentsAndBitangents()){
				m_buffer.add_element(aimesh->mTangents, "Tangents", aimesh->mNumVertices, sizeof(aiVector3D));
				m_buffer.add_element(aimesh->mBitangents, "BiTangents", aimesh->mNumVertices, sizeof(aiVector3D));
		}
		for (auto i = 0u; i < aimesh->GetNumColorChannels(); ++i){
			if (aimesh->HasVertexColors(i)){
				std::string semantic("Color[" + std::to_string(i) + ']');
				m_buffer.add_element(aimesh->mColors[i], semantic, aimesh->mNumVertices, sizeof(aiColor4D));
			}
		}
		enBuffer::Indecies indecies;
		for (auto face = 0u; face < aimesh->mNumFaces; ++face)
			for (auto index = 0u; index < aimesh->mFaces[face].mNumIndices; ++index)
				indecies.push_back(aimesh->mFaces[face].mIndices[index]);
		m_topology = BindableManager::Query<Topology>((D3D_PRIMITIVE_TOPOLOGY)aimesh->mPrimitiveTypes);
		m_vertexBuffer = BindableManager::Query<VertexBuffer>(m_buffer, m_name);
		m_indexBuffer = BindableManager::Query<IndexBuffer>(indecies, m_name);
		m_vertexShader = BindableManager::Query<VertexShader>("VertexShader.cso");
		m_inputLayout = BindableManager::Query<InputLayout>(m_vertexShader->GetSignatures(), m_vertexShader->GetBlob(), GetName());
	}
	
	const std::string& Mesh::GetName() const 
	{ return m_name; }

	const VertexBuffer::handle& Mesh::GetVertexBuffer() const 
	{ return m_vertexBuffer; }

	const IndexBuffer::handle& Mesh::GetIndexBuffer() const 
	{ return m_indexBuffer; }

	Material::Index Mesh::GetMaterialIndex() const 
	{ return m_materialIndex; }

	const Teqnique::Container& Mesh::GetTeqniques() const 
	{ return m_teqniques; }

	Teqnique::Container& Mesh::Teqniques() 
	{ return m_teqniques; }

	void Mesh::SetName(const std::string& name)
	{
		m_name = name;
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

	void Mesh::AddTeqnique(Teqnique teqnique, const Material::Container& materials){
		for(auto& step : teqnique.GetSteps()) 
		{
			if(!step.HasBindable<PixelShader>()) {
				step.AddBindable(BindableManager::Query<PixelShader>(
					step.GetPassName(),
					materials[m_materialIndex].GetShaderEntryPoint()));
			}
			/*auto vs = BindableManager::Query<VertexShader>(step.GetPassName(), materials[m_materialIndex].GetShaderEntryPoint());
			auto il = BindableManager::Query<InputLayout>(vs->GetSignatures(), vs->GetBlob(), m_name);*/
		}
		auto found = std::find(begin(m_teqniques), end(m_teqniques), teqnique);
		if(found != std::end(m_teqniques))
		{
			auto it = m_teqniques.erase(found);
			m_teqniques.insert(it, teqnique);
			return;
		}
		m_teqniques.push_back(teqnique);
	}

	void Mesh::Update(float dt)
	{
		int dummy = 0;
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
