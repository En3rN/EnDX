#include "Light.h"
#include "Camera.h"
#include <algorithm>

namespace En3rN::DX
{
	Light::Light(entt::registry& registry) : 
		m_registry(registry),
		m_cBuff(BindableManager::Query<ConstantBuffer>(1, 0))
	{
		m_cBuff->Bind();
	}
	void Light::UIControls()
	{
		ImGui::DragFloat4("Ambient", &m_data.Ambient.x, 0.1f, 0, 1);
		ImGui::DragFloat4("Color", &m_data.Light->Color.x, 0.1f, 0, 1);
		ImGui::DragFloat3("Attenuation", &m_data.Light->Attenuation.x, 0.1f, 0, 1);
		ImGui::DragFloat3("Position", &m_data.Light->Position.x);
		ImGui::DragFloat3("Direction", &m_data.Light->Direction.x,0.05f, -1, 1);
		ImGui::DragFloat("Cone",&m_data.Light->ConeInner, 0.05f, 0, 1);
		ImGui::DragFloat("Cone", &m_data.Light->ConeOuter, 0.05f, 0, 1);
	}
	void Light::OnAttach()
	{
		//register with owner
		//register with renderer
	}

	void Light::OnUpdate()
	{
		auto ambientView = m_registry.view<AmbientLightComponent>();
		auto pointLightView = m_registry.view<PointLightComponent>();
		auto spotLightView = m_registry.view<SpotLightComponent>();
		auto directionalLightView = m_registry.view<DirectionalLightComponent>();
		//auto cameraView = m_registry.view<CameraComponent>();
		const auto& cameraPosition = Camera::GetActiveCamera().GetPosition();

		for (auto [entity, ambient] : ambientView.each()){
			m_data.Ambient = ambient.Ambient;
		}
		uint32_t index{};
		for(auto [entity, light] : directionalLightView.each()) {
			m_data.Light[index].Color = light.Color;
			m_data.Light[index].IsDirectional = light.IsDirectional;
			m_data.Light[index].Direction = Transform::GetDirection(m_registry.get<TransformComponent>(entity).Angles);
			if(index < numLights)
				++index;
			else
				break;
		}
		if(index < numLights)
		{
			m_common.clear();
			for(auto [entity, light] : pointLightView.each()) {
				Data::Common data;
				data.Color = light.Color;
				data.Attenuation = light.Attenuation;
				data.Position = m_registry.get<TransformComponent>(entity).Position;
				data.IsDirectional = false;
				m_common.push_back(data);
			}
			for(auto [entity, light] : spotLightView.each()) {
				Data::Common data;
				auto& transform = m_registry.get<TransformComponent>(entity);
				data.Color = light.Color;
				data.Attenuation = light.Attenuation;
				data.ConeInner = light.ConeInner;
				data.ConeOuter = light.ConeOuter;
				data.IsDirectional = false;
				data.Position = transform.Position;
				data.Direction = Transform::GetDirection(transform.Angles);
				m_common.push_back(data);
			}
			std::sort(begin(m_common), end(m_common), [&](const Data::Common& left, const Data::Common& right) {
				auto distToLeft = (left.Position - cameraPosition).Length();
				auto distToRight = (right.Position - cameraPosition).Length();
				auto closer = distToLeft < distToRight;
				/*if(closer)
					Logger::Debug("Distance to Camera: %f return %i", distToLeft, closer);*/
				return closer;
				}
			);
			uint32_t cIndex{};
			for(index; index < numLights; ++index) {
				if(cIndex < m_common.size()) {
					m_data.Light[index] = m_common[cIndex];
					++cIndex;
				}
			}
		}
		//update position/direction
		auto data = m_data;
		for(auto& l : data.Light) {
			DirectX::XMVector3Transform(l.Position, Camera::GetActiveCamera().GetViewMatrix());
			DirectX::XMVector3Transform(l.Direction, Camera::GetActiveCamera().GetViewMatrix());
		}
		m_cBuff->Update(m_data);
	}

	void Light::OnDetach()
	{
		//remove from renderer
	}

	
}