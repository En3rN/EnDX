#pragma once
#include "vec.h"
#include "entt\entt.hpp"
#include <string>

namespace En3rN::DX
{
	class Entity
	{
	public:
		using Container = std::vector<Entity>;

		Entity(entt::registry& registry, std::string&& name);

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
		
		entt::entity UISelector(entt::entity selected);
		auto UIControls();
		
		virtual ~Entity() = default;
	protected:
		size_t GenerateUID();
		entt::handle    m_handle;
	private:
		bool ui_open = false;
		Entity::Container m_children;
	};
	

}
