#pragma once

#include "Pass.h"
#include "iBindable.h"
#include "Shader.h"
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
			AddBindables((bindables)...);
		}
		Step(Pass::Name renderPass): m_renderPass(renderPass){}
		Step(Step&& other) noexcept = default;
		Step(const Step & other) = default;
		Step& operator = (Step && other) noexcept = default;
		Step& operator = (const Step & other) = default;
		~Step() = default;

		// Adds bindable to container, if passed by derived bindable type it tries to replace
		// current bindable of that type
		template<typename T>
		void AddBindable(std::shared_ptr<T> bindable)
		{
			if constexpr(!std::is_same_v<Bindable, T>) {
				for(auto& stored : m_bindables) {
					if(std::dynamic_pointer_cast<T>(stored)) {
						stored = bindable;
						return;
					}
				}
			}
			if(std::dynamic_pointer_cast<Shader>(bindable))
				m_bindables.insert(m_bindables.begin(), std::move(bindable));
			else
				m_bindables.push_back(std::move(bindable));
			return;
		}
		template <typename T>
		bool HasBindable()
		{
			for(auto& bindable : m_bindables)
				if(std::dynamic_pointer_cast<T>(bindable))
					return true;
			return false;
		}
		template <typename T>
		T* GetBindable()
		{
			std::shared_ptr<T> result;
			for(auto& bindable : m_bindables) {
				result = std::dynamic_pointer_cast<T>(bindable);
				if(result)
					return result.get();
			}
			return nullptr;
		}
		
		const Bindable::Base::Container&	GetBindables() const { return m_bindables; }
		const Pass::Name					GetRenderPassSlot() const { return m_renderPass; }
		const std::string					GetPassName() const
		{
			using namespace std::string_literals;
			switch(m_renderPass)
			{
			case En3rN::DX::Pass::Name::Setup:
				return "null"s;
			case En3rN::DX::Pass::Name::Phong:
				return "Phong"s;
			case En3rN::DX::Pass::Name::Unlit:
				return "Unlit"s;
			case En3rN::DX::Pass::Name::WriteMask:
				return "WriteMask"s;
			case En3rN::DX::Pass::Name::ReadMask:
				return "ReadMask"s;
			case En3rN::DX::Pass::Name::Fullscreen:
				return "Fullscreen"s;
			case En3rN::DX::Pass::Name::Skybox:
				return "Skybox"s;
			default:
				break;
			}
		}
	
		template <typename ... Bindables>
		void AddBindables(Bindable::Base::handle bindable, Bindables ... rest)
		{
			AddBindable(bindable);
			if constexpr (sizeof...(rest)>0)
				AddBindables((rest)...);
		}
		Pass::Name					m_renderPass;
		Bindable::Base::Container	m_bindables;
	};

}
