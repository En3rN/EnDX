#pragma once
#include "Texture.h"
#include "Buffer.h"
#include "Shader.h"
#include "Teqnique.h"
#include <bitset>
#include <stdint.h>
#include <vector>

class aiMaterial;

namespace En3rN::DX
{
	class Material
	{
	public:
		using Container = std::vector<Material>;
		using Index = uint32_t;
		enum class Map { Diffuse, Normal, Specular };
		
		// vec4f diff, vec3f specColor, float sIntenity, vec3f specPow
		struct Data
		{
			Vec4f				diffuse{};
			Vec3f				specular{};
			float				specularPower{};
			Vec3f				emissive{};
			float				specularIntensity{};
		};
		
		using ConstantBuffer = PSConstantBuffer<Data>;

		Material() = default;
		Material(aiMaterial* aimaterial, std::filesystem::path modelPath="");
		Material(Material::Data && data);
		Material(const Material & other) = default;
		Material(Material && other) = default;
		Material& operator = (const Material & other) = default;
		Material& operator = (Material && other) = default;
		~Material() = default;

		ConstantBuffer& GetMaterialConstantBuffer() { return *m_constantBuffer.get(); }
		const Data& GetData() const { return m_data; }
		const std::string GetName() const { return m_name; }

		auto Size() const {return UINT{ sizeof Data };};

		void SetDiffuse				(Vec4f&& diffuse);
		void SetSpecular			(Vec3f&& specular);
		void SetEmissive			(Vec3f&& emissive);
		void SetSpecularIntensity	(float specIntensity);
		void SetSpecularPower		(float specPower);
		//void SetShader				(const Teqnique& tecnique);

		void UpdateConstantBuffer();

		void AddTextureMap(Map map, const std::shared_ptr<Texture> texture);
		
		const bool HasMap(Map map) const;
		void SetPixelShader(std::filesystem::path filename);

		void Bind() const;

		const std::string GetShaderEntryPoint() const;

		bool UIControls();
		
	private:
		std::string						m_name;
		Data							m_data;
		std::bitset<3>					m_hasMap;
		Texture::Container				m_textures;
		bool							m_updateConstantBuffer;
		std::shared_ptr<ConstantBuffer>	m_constantBuffer;
		PixelShader::handle				m_debugShader = nullptr;
	};
}

