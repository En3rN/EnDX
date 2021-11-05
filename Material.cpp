#include "Material.h"
#include "enBuffer.h"
#include "assimp\material.h"
#include "Config.h"


namespace En3rN::DX
{

	/*Material::Material() : 
		m_data{ (Vec4f(0,0,0,0)),0,0,false,false, false }
	{
		m_constantBuffer = std::make_shared<ConstantBuffer>(*this, 1, Size(), 1);
	}*/
	Material::Material(const aiMaterial* aimaterial): m_data{},
		m_constantBuffer(BindableManager::Query<ConstantBuffer>(1, 1))
	{
		using std::filesystem::path;
		for (auto i = 0u; i < aimaterial->mNumProperties; ++i) {
		}
		aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, m_data.diffuse);
		aimaterial->Get(AI_MATKEY_SHININESS, m_data.specular);
		aimaterial->Get(AI_MATKEY_SHININESS_STRENGTH, m_data.specularPower);
		aiString aipath;
		auto modelPath = Config::GetFolders().modelPath.string();
		for (auto i = 0u; i < aimaterial->GetTextureCount(aiTextureType_DIFFUSE); ++i) {
			aimaterial->GetTexture(aiTextureType_DIFFUSE, i, &aipath);
			auto texturePath = Config::GetFolders().modelPath; 
			texturePath /= path(aipath.C_Str());
			AddTextureMap(Map::Diffuse, BindableManager::Query<Texture>(texturePath, 0));
		}
		for (auto i = 0u; i < aimaterial->GetTextureCount(aiTextureType_SPECULAR); ++i) {
			aimaterial->GetTexture(aiTextureType_SPECULAR, i, &aipath);
			auto texturePath = Config::GetFolders().modelPath;
			texturePath /= path(aipath.C_Str());
			AddTextureMap(Map::Specular, BindableManager::Query<Texture>(texturePath, 1));
		}
		for (auto i = 0u; i < aimaterial->GetTextureCount(aiTextureType_NORMALS); ++i) {
			aimaterial->GetTexture(aiTextureType_NORMALS, i, &aipath);
			auto texturePath = Config::GetFolders().modelPath;
			texturePath /= path(aipath.C_Str());
			AddTextureMap(Map::Normal, BindableManager::Query<Texture>(texturePath, 2));
		}
	}
	Material::Material(Vec4f&& diffuse, Vec4f&& specular, Vec4f&& emissive, float specPower) :
		m_data{ std::move(diffuse),std::move(specular),std::move(emissive),specPower },
		m_constantBuffer(BindableManager::Query<ConstantBuffer>(1,1)){}

	void Material::SetDiffuse(Vec4f&& diffuse)
	{
		m_data.diffuse = std::move(diffuse);
	}
	void Material::SetSpecular(Vec4f&& specular)
	{
		m_data.specular = std::move(specular);
	}
	void Material::SetEmissive(Vec4f&& emissive)
	{
		m_data.emissive = std::move(emissive);
	}
	void Material::SetSpecularPower(float specPower)
	{
		m_data.specularPower.x = specPower;
	}
	
	void Material::AddTextureMap(Map map, const std::shared_ptr<Texture> texture)
	{
		m_hasMap[std::underlying_type<Map>::type(map)];
		m_textures.push_back(texture);		
	}
	const bool Material::HasMap(Map map) const
	{
		return m_hasMap[std::underlying_type<Map>::type(map)];
	}

	void Material::Bind() const
	{
		for (auto& t : m_textures)
			t->Bind();
		m_constantBuffer->Update(m_data);
		m_constantBuffer->Bind();
		//m_pixelShader->Bind();
	}
	
	const std::string Material::GetShaderEntryPoint() const
	{
		std::string ep("Mat");
		if (HasMap(Map::Diffuse))
			ep += "Dif";
		if (HasMap(Map::Specular))
			ep += "Spec";
		if (HasMap(Map::Normal))
			ep += "Norm";
		return ep;
	}
	//Material::ConstantBuffer::ConstantBuffer(UINT count, UINT stride, UINT slot, UINT offset) :
	//	Buffer(count,stride,slot,offset)
	//{
	//	D3D11_BUFFER_DESC pdesc{};
	//	pdesc.ByteWidth = stride * count;
	//	pdesc.Usage = D3D11_USAGE_DYNAMIC;
	//	pdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//	pdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//	pdesc.MiscFlags = 0;
	//	pdesc.StructureByteStride = stride;
	//	errchk::hres(pDevice->CreateBuffer(&pdesc, nullptr, &pBuffer), EnExParam);
	//}
	/*void Material::ConstantBuffer::Update(const Entity& e)
	{
		
	}*/
	//void Material::ConstantBuffer::Update(const Material& mat)
	//{
	//	D3D11_MAPPED_SUBRESOURCE msr{};
	//	errchk::hres(pContext->Map(pBuffer.Get(), slot, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0u, &msr), EnExParam);
	//	memcpy(msr.pData, &mat.m_data, mat.Size());
	//	pContext->Unmap(pBuffer.Get(), slot);
	//}
	//void Material::ConstantBuffer::Bind()
	//{
	//	pContext->PSSetConstantBuffers(slot, 1, pBuffer.GetAddressOf());
	//}
}