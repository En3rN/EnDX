#pragma once
#include "Step.h"
#include <entt\entt.hpp>
#include <vector>
#include <memory>

namespace En3rN::DX
{
	class Teqnique
	{
	public:		
		using Container = std::vector<Teqnique>;
		using Index = uint32_t;
		struct Component;
		Teqnique(std::string name, bool startActive = true): m_name(name), m_active(startActive){};
		template <typename ... Steps>
		Teqnique(std::string name, bool startActive, Steps ... steps) : m_name(std::move(name)), m_active(startActive)
		{
			AddStep(steps...);			
		}
		Teqnique(const Teqnique& other) = default;
		Teqnique(Teqnique&& other) noexcept = default;
		Teqnique& operator=(const Teqnique& other) = default;
		Teqnique& operator=(Teqnique&& other) noexcept = default;
		~Teqnique() = default;
		template <typename ... Steps>
		void AddStep(Step step, Steps... steps)
		{
			AddStep(step);
			AddStep(steps...);
		}
		void AddStep(Step step)
		{
			m_steps.push_back(step);
		}
		template <class Func>
		void Context(Func func)
		{
			func(m_context);
		}
		enBuffer& GetContext() { return m_context; }
		const std::string&		GetName()	const	{ return m_name; }
		const Step::Container&	GetSteps() const	{ return m_steps; }
		Step::Container&		GetSteps()			{ return m_steps; }
		bool&					Active()			{ return m_active; }
		bool					IsActive() const	{ return m_active; }
		void					Toggle() { m_active ? m_active = false : true; }
		bool operator == (const Teqnique& other)	{ return m_name == other.GetName(); }

	protected:
		std::string				m_name;
		enBuffer				m_context;
		bool					m_active = true;
		Step::Container			m_steps;

	public:
		static std::map<std::string, std::function<Teqnique()>> Teqniques;
	};
	struct Teqnique::Component
	{
		Teqnique::Container Teqniques;
	};
}
