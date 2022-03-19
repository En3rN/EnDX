#pragma once


#include "Component.h"
#include "Buffer.h"
#include "Scene.h"

namespace En3rN::DX
{

	class Light 
	{
	public:
		static constexpr int numLights = 3;
		using Container = std::vector<Light>;
		struct Data
		{
			Vec4f Ambient{};
			struct Common 
			{
				Vec4f Color = {0,0,0,0};
				Vec4f Attenuation = {1,0.5,0.25,0.125};
				Vec3f Position{};
				BOOL  IsDirectional{};
				Vec3f Direction{};
				float Cone{};
			} Light[numLights];
		};
		using ConstantBuffer = PSConstantBuffer<Data>;
		
		Light(entt::registry& registry);

		std::shared_ptr<ConstantBuffer> GetLightCB() { return m_cBuff; }

		const	Data& GetData() const	{ return m_data; }
				Data& GetData()			{ return m_data; }

		void SetData(const Data& data) { m_data = data; }
		void UIControls();

		//components funcs
		void OnAttach();
		void OnUpdate();
		void OnDetach();

		virtual ~Light() = default;

	protected:
		Data							m_data;
		std::vector<Data::Common>		m_common;
		entt::registry&					m_registry;
		std::shared_ptr<ConstantBuffer> m_cBuff;
	};

	//components
	
	

	
}

