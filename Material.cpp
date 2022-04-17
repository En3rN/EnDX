#include "Material.h"
#include "enBuffer.h"
#include "assimp\material.h"
#include "Config.h"
#include "ImFileDialog\ImFileDialog.h"
#include "Pass.h"
#include "Topology.h"


namespace En3rN::DX
{
	Material::Material(aiMaterial* aimaterial, std::filesystem::path modelPath): m_data{},
		m_constantBuffer(std::make_shared<ConstantBuffer>(1, 1)),
		m_name(aimaterial->GetName().C_Str()),
		m_updateConstantBuffer(true)
	{
		using std::filesystem::path;
		for (auto i = 0u; i < aimaterial->mNumProperties; ++i) {
			std::stringstream debug;
			debug << aimaterial->mProperties[i]->mKey.C_Str()
				<< " : " << aimaterial->mProperties[i]->mSemantic
				<< " : " << aimaterial->mProperties[i]->mType << '\n';

			OutputDebugString(debug.str().c_str());
		}
		aiColor3D aicolor{};
		if(aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aicolor) == AI_FAILURE)
		{
			m_data.diffuse = { 0.001,0.001, 0.001, 1 };
			OutputDebugString ("Diffuse Failed\n");
		}
		else
			m_data.diffuse = { aicolor.r,aicolor.g,aicolor.b };
		
		if(aimaterial->Get(AI_MATKEY_COLOR_SPECULAR, aicolor) == AI_FAILURE)
		{
			m_data.specular = { 1,1,1 };
			OutputDebugString ("SpecularColor Failed\n");
		}
		else
			m_data.specular = { aicolor.r,aicolor.g,aicolor.b };
		if(aimaterial->Get(AI_MATKEY_COLOR_EMISSIVE, aicolor) == AI_FAILURE)
		{
			m_data.emissive = {0,0,0 };
			OutputDebugString ("SpecularColor Failed\n");
		}
		else
			m_data.emissive = { aicolor.r,aicolor.g,aicolor.b };
		
		if(aimaterial->Get(AI_MATKEY_OPACITY, m_data.diffuse.w) == AI_FAILURE)
		{
			m_data.diffuse.w = 1;
			OutputDebugString ("Opacity Failed\n");

		}
		if(aimaterial->Get(AI_MATKEY_SHININESS, m_data.specularPower)== AI_FAILURE)
		{
			m_data.specularPower = { 8 };
			OutputDebugString ("SpecularPower Failed\n");
		}
		if(aimaterial->Get(AI_MATKEY_SHININESS_STRENGTH, m_data.specularIntensity)==AI_FAILURE)
		{
			m_data.specularIntensity = { 1 };
			OutputDebugString ("SpecularIntensity Failed\n");
		}		
		aiString aipath;
		for (auto i = 0u; i < aimaterial->GetTextureCount(aiTextureType_DIFFUSE); ++i) {
			aimaterial->GetTexture(aiTextureType_DIFFUSE, i, &aipath);
			auto texturePath = modelPath; 
			texturePath /= path(aipath.C_Str());
			AddTextureMap(Map::Diffuse, BindableManager::Query<Texture>(texturePath, 0));
		}
		
		for (auto i = 0u; i < aimaterial->GetTextureCount(aiTextureType_NORMALS); ++i) {
			aimaterial->GetTexture(aiTextureType_NORMALS, i, &aipath);
			auto texturePath = modelPath;
			texturePath /= path(aipath.C_Str());
			AddTextureMap(Map::Normal, BindableManager::Query<Texture>(texturePath, 1));
		}
		for (auto i = 0u; i < aimaterial->GetTextureCount(aiTextureType_SPECULAR); ++i) {
			aimaterial->GetTexture(aiTextureType_SPECULAR, i, &aipath);
			auto texturePath = modelPath;
			texturePath /= path(aipath.C_Str());
			AddTextureMap(Map::Specular, BindableManager::Query<Texture>(texturePath, 2));
		}
		for (auto i = 0u; i < aimaterial->GetTextureCount(aiTextureType_HEIGHT); ++i) {
			aimaterial->GetTexture(aiTextureType_HEIGHT, i, &aipath);
			auto texturePath = modelPath;
			texturePath /= path(aipath.C_Str());
			AddTextureMap(Map::Normal, BindableManager::Query<Texture>(texturePath,1));
		}
		UpdateConstantBuffer();
	}
	Material::Material(Material::Data && data) :
		m_data{m_data = std::move(data)},
		m_constantBuffer(std::make_shared<ConstantBuffer>(1, 1)),
		m_updateConstantBuffer(true){}

	void Material::SetDiffuse(Vec4f&& diffuse)
	{
		m_data.diffuse = std::move(diffuse);
		m_updateConstantBuffer = true;
	}
	void Material::SetSpecular(Vec3f&& specular)
	{
		m_data.specular = std::move(specular);
		m_updateConstantBuffer = true;
	}
	void Material::SetEmissive(Vec3f&& emissive)
	{
		m_data.emissive = std::move(emissive);
		m_updateConstantBuffer = true;
	}
	void Material::SetSpecularIntensity(float specIntensity)
	{
		m_data.specularIntensity = specIntensity;
		m_updateConstantBuffer = true;
	}
	void Material::SetSpecularPower(float specPower)
	{
		m_data.specularPower = specPower;
		m_updateConstantBuffer = true;
	}

	void Material::UpdateConstantBuffer()
	{
		if(m_updateConstantBuffer) {
			m_updateConstantBuffer = false;
			m_constantBuffer->Update(m_data);
		}
		return;
	}
	
	void Material::AddTextureMap(Map map, const std::shared_ptr<Texture> texture)
	{
		m_hasMap[std::underlying_type<Map>::type(map)] = true;
		m_textures.push_back(texture);		
	}
	const bool Material::HasMap(Map map) const
	{
		return m_hasMap[std::underlying_type<Map>::type(map)];
	}

	void Material::SetPixelShader(std::filesystem::path filename)
	{
		m_debugShader = BindableManager::Query<PixelShader>(filename);
	}

	void Material::Bind() const
	{
		for (auto& t : m_textures)
			t->Bind();
		m_constantBuffer->Bind();
	}
	
	const std::string Material::GetShaderEntryPoint() const
	{
		using namespace std::string_literals;
		std::string ep{};
		if (HasMap(Map::Diffuse))
			ep += "Diffuse"s;
		if (HasMap(Map::Specular))
			ep += "Specular"s;
		if (HasMap(Map::Normal))
			ep += "Normal"s;
		if(ep.empty())
			ep += "main"s;
		return ep;
	}

	bool Material::UIControls()
	{
		bool change = false;
		bool normalMap = HasMap(Map::Normal);
		bool diffMap = HasMap(Map::Diffuse);
		bool specMap = HasMap(Map::Specular);
		static ImGuiColorEditFlags flags = ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs;
		m_updateConstantBuffer += ImGui::ColorEdit4("Diffuse", &m_data.diffuse.x, flags);
		m_updateConstantBuffer += ImGui::ColorEdit3("Specular", &m_data.specular.x,flags); 
		m_updateConstantBuffer += ImGui::DragFloat("SpecularIntensity", &m_data.specularIntensity, .0001f);
		m_updateConstantBuffer += ImGui::DragFloat("SpecularPower", &m_data.specularPower,.0001f);
		m_updateConstantBuffer += ImGui::ColorEdit4("Emissive", &m_data.emissive.x, flags);
		if(ImGui::Checkbox("Diffuse Map",&diffMap))
		{
			m_hasMap[std::underlying_type<Map>::type(Map::Diffuse)] = diffMap;
			change = true;
		}
		if(ImGui::Checkbox("Specular Map",&specMap))
		{
			m_hasMap[std::underlying_type<Map>::type(Map::Specular)] = specMap;
			change = true;
		}
		if(ImGui::Checkbox("Normal Map",&normalMap))
		{
			m_hasMap[std::underlying_type<Map>::type(Map::Normal)] = normalMap;
			change = true;
		}
		UpdateConstantBuffer();
		return change;
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
	//	errchk::hres(pDevice->CreateBuffer(&pdesc, nullptr, &pBuffer));
	//}
	/*void Material::ConstantBuffer::Update(const Node& e)
	{
		
	}*/
	//void Material::ConstantBuffer::Update(const Material& mat)
	//{
	//	D3D11_MAPPED_SUBRESOURCE msr{};
	//	errchk::hres(pContext->Map(pBuffer.Get(), slot, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0u, &msr));
	//	memcpy(msr.pData, &mat.m_data, mat.Size());
	//	pContext->Unmap(pBuffer.Get(), slot);
	//}
	//void Material::ConstantBuffer::Bind()
	//{
	//	pContext->PSSetConstantBuffers(slot, 1, pBuffer.GetAddressOf());
	//}
}