#pragma once

#include "Pass.h"
#include "iBindable.h"
#include <vector>


namespace En3rN::DX
{
	class Step
	{
	public:
		using Container = std::vector<Step>;
		template <typename ... Bindables>
		Step(Pass::Name renderPass, Bindables ... bindables) : m_renderPass(renderPass)
		{
			AddBindable((bindables)...);
		}
		/*Step(Step&& other) noexcept = default;
		Step(Step & other) = default;
		Step& operator = (Step && other) noexcept = default;
		Step& operator = (Step & other) = default;
		~Step() = default;*/

		void AddBindable(Bindable::Base::handle&& bindable)
		{
			m_bindables.push_back(std::move(bindable));
		}
		
		const Bindable::Base::Container&	GetBindables() const { return m_bindables; }
		const Pass::Name					GetRenderPassSlot() const { return m_renderPass; }
		const std::string					GetPassName()
		{
			switch (m_renderPass)
			{
			case En3rN::DX::Pass::Name::Lambertian:
				return std::string("Lambertian");
			case En3rN::DX::Pass::Name::Skybox:
				return std::string("Skybox");
			default:
				return std::string("Invalid Pass");
				break;
			}
		}
	
		template <typename ... Bindables>
		void AddBindable(Bindable::Base::handle bindable, Bindables ... rest)
		{
			m_bindables.push_back(bindable);
			AddBindable((rest)...);
		}
		Pass::Name					m_renderPass;
		Bindable::Base::Container	m_bindables;
	};

}
