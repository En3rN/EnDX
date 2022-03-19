#pragma once

#include "Step.h"
#include <vector>
#include <memory>

namespace En3rN::DX
{
	class Teqnique
	{
	public:
		using Container = std::vector<Teqnique>;
		using Index = uint32_t;
		Teqnique() = default;
		Teqnique(std::string name) : m_name(std::move(name)) {}
		Teqnique(const Teqnique& other) = default;
		Teqnique(Teqnique&& other) noexcept = default;
		Teqnique& operator=(const Teqnique& other) = default;
		Teqnique& operator=(Teqnique&& other) noexcept = default;
		~Teqnique() = default;

		void AddStep(Step&& step)
		{
			m_steps.push_back(std::move(step));
		}
		std::string&			GetName()			{ return m_name; }
		std::string				GetPassName(uint32_t stepIndex) { return m_steps[stepIndex].GetPassName(); }
		const Step::Container&	GetSteps() const	{ return m_steps; }
		Step::Container&		GetSteps()			{ return m_steps; }
		bool&					Active()			{ return m_active; }
		bool					IsActive() const	{ return m_active; }
		void					Toggle() { m_active ? m_active = false : true; }

		//static factory funcs for default teqniques
		static Teqnique Setup();
		static Teqnique Phong();
		static Teqnique Unlit();
		static Teqnique Fullscreen();
		static Teqnique Skybox();

	protected:
		std::string				m_name;
		bool					m_active = true;
		Step::Container			m_steps;
	};
}
