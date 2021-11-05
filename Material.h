#pragma once
#include "Texture.h"
#include "Buffer.h"
#include "Shader.h"
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
		
		struct Data
		{
			Vec4f				diffuse;
			Vec4f				specular;
			Vec4f				emissive;
			Vec4f				specularPower;
		};
		/*class ConstantBuffer : public PSConstantBuffer<Data>
		{
		public:
			using handle = std::shared_ptr<ConstantBuffer>;
			ConstantBuffer(UINT count, UINT slot = 1, UINT offset=0) :
				PSConstantBuffer<Data>(count, slot) {}
		};*/
		using ConstantBuffer = PSConstantBuffer<Data>;

		Material() = default;
		Material(const aiMaterial* aimaterial);
		Material(Vec4f&& diffuse, Vec4f&& specular, Vec4f&& emissive, float specPower );
		Material(const Material & other) = default;
		Material(Material && other) = default;
		Material& operator = (const Material & other) = default;
		Material& operator = (Material && other) = default;
		~Material() = default;

		ConstantBuffer& GetMaterialConstantBuffer() { return *m_constantBuffer.get(); }

		auto Size() const {return UINT{ sizeof Data };};

		void SetDiffuse				(Vec4f&& diffuse);
		void SetSpecular			(Vec4f&& specular);
		void SetEmissive			(Vec4f&& emissive);
		void SetSpecularPower		(float specPower);

		void AddTextureMap(Map map, const std::shared_ptr<Texture> texture);
		
		const bool HasMap(Map map) const;

		void Bind() const;

		const std::string GetShaderEntryPoint() const;
		
	private:
		
		Data							m_data;
		std::bitset<3>					m_hasMap;
		Texture::Container				m_textures;
		PixelShader::handle				m_pixelShader;
		std::shared_ptr<ConstantBuffer>	m_constantBuffer;
	};
}

