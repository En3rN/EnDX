#pragma once

#include "iBindable.h"
#include "BindableManager.h"
#include "Shader.h"
#include "Sampler.h"
#include "Rasterizer.h"
#include "Blend.h"
#include "Stencil.h"
#include "Transform.h"
#include <vector>


namespace En3rN::DX
{
	//Step is a class that hooks up with a RenderPass to later add jobs to that pass,
	//There may be several steps in a Teqnicque
	class Step
	{
	public:
		using Container = std::vector<Step>;

		struct RenderPassAndInput
		{
			std::string renderPassLookUpName{};
			//lookup name for making texture from a renderTarget
			std::string inputLookUpName{};
		};
		//renderpasses to target, custom bindables to replace with the defaults
		template <typename ... Bindables>
		Step(RenderPassAndInput outputAndInput, Bindables ... bindables) :
			m_io(std::move(outputAndInput))
		{			
			AddBindables((bindables)...);
		}
		//renderpass to target, default bindable pool
		Step(RenderPassAndInput outputAndInput): m_io(std::move(outputAndInput)) {}
		Step(const Step& other) = default;
		Step(Step&& other) noexcept = default;
		Step& operator = (const Step& other) = default;
		Step& operator = (Step&& other) noexcept = default;
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
				m_bindables.insert(m_bindables.begin(), bindable);
			else
				m_bindables.push_back(bindable);
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
		auto GetBindable()
		{
			std::shared_ptr<T> result;
			for(auto& bindable : m_bindables) {
				result = std::dynamic_pointer_cast<T>(bindable);
				if(result)
					return result;
			}
			return result;
		}
		template <typename ... Bindables>
		void AddBindables(Bindable::Base::handle bindable, Bindables ... rest)
		{
			AddBindable(bindable);
			if constexpr(sizeof...(rest) > 0)
				AddBindables((rest)...);
		}
		const Bindable::Base::Container& GetBindables() const { return m_bindables; }
		Bindable::Base::Container& GetBindables()  { return m_bindables; }

		//const Pass::Name					GetRenderPassSlot() const { return m_renderPass; }
		const std::string& GetPassName() const
		{
			using namespace std::string_literals;
			/*switch(m_renderPass)
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
			}*/
			return m_io.renderPassLookUpName;
		}
		const std::string& GetInputName() const
		{
			return m_io.inputLookUpName;
		}

	private:	
		RenderPassAndInput			m_io{};
		Bindable::Base::Container	m_bindables{
			BindableManager::Query<Sampler>(Sampler::State::Wrap),
			BindableManager::Query<Rasterizer>(Rasterizer::State::Back),
			BindableManager::Query<Blender>(Blender::State::Default),
			BindableManager::Query<DepthStencilState>(DepthStencilState::Depth::Enable),
		};
	};
}
