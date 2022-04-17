#pragma once
#include "vec.h"
#include "Transform.h"
#include "Component.h"
#include "entt\entt.hpp"
#include <string>
#include <assimp\scene.h>

namespace En3rN::DX
{
	
	class Entity
	{
	public:
		using Container = std::vector<Entity>;

		Entity(entt::registry& registry, std::string&& name);

		Entity(entt::registry& registry, 
			aiNode* node,
			const Teqnique::Container& teqniques,
			const uint32_t startslotMeshes);

		Entity(Entity&& other) noexcept = default;
		Entity& operator = (Entity&& other) noexcept = default;

		const std::string& GetName() const;
		
		void SetName(std::string&& name);

		bool operator == (entt::entity entt) {
			return m_handle.entity() == entt;
		}
		template <class Component, typename ... Constructorargs>
		void AddComponent(Constructorargs ... args)
		{
			m_handle.emplace<Component>(args ...);
		}
		void AddChild(Entity&& entity);
		template <class T>
		T& GetComponent()
		{
			return m_handle.get<T>();
		}
		template <typename ... Ts>
		auto GetComponents()
		{
			return m_handle.try_get<Ts ...>();
		};
		Entity* FindChild(entt::entity entt) {
			
			if(*this == entt)
				return this;
			for(auto& child : m_children) {
				auto found = child.FindChild(entt);
				if(found)
					return found;
			}
			return nullptr;
		}
		Entity::Container& GetChilds();
		const Entity::Container& GetChilds() const;
		template<typename Component>
		void OnUpdate() {
			return;
		}
		template<>
		void OnUpdate<TransformComponent>() {
			auto& transform = GetComponent<TransformComponent>();
			transform.Transform = Transform::GetMatrix(transform.Position, transform.Angles, transform.Scale);
			auto totalTransform = transform.Transform * transform.ParentTransform;
			SetParentTransform(totalTransform);
			UpdateConstantBuffer(DirectX::XMMatrixTranspose(totalTransform));
		}
		template<>
		void OnUpdate<ModelRendererComponent>() {

		}
		entt::entity UISelector(entt::entity selected);
		auto UIControls();
		
		virtual ~Entity() = default;
	protected:
		void UpdateConstantBuffer(const Transform::Matrix& mat);
		void SetParentTransform(const Transform::Matrix& mat);
		size_t GenerateUID();
		entt::handle    m_handle;
		bool ui_open = false;
		Entity::Container m_children;
	};
	

}
