#pragma once

#include "Step.h"
#include <vector>
#include <memory>

namespace En3rN::DX
{
	class Teqnique
	{
	public:
		struct Base
		{
			using handle = std::shared_ptr<Teqnique>;
			using Container = std::vector<handle>;
		};
		void AddStep(Step&& step)
		{
			m_steps.push_back(std::move(step));
		}
		
		std::string				GetPassName(uint32_t stepIndex) { return m_steps[stepIndex].GetPassName(); }
		const Step::Container&	GetSteps() const { return m_steps; }
		bool					IsActive() const { return m_active; }
		void					Toggle() { m_active ? m_active = false : true; }
	protected:
		bool					m_active = true;
		Step::Container			m_steps;
	};
}
