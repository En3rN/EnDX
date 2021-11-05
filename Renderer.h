#pragma once

#include "end3d11.h"
#include "Pass.h"
#include "Light.h"


namespace En3rN::DX
{
	class Pass;
	class Renderer
	{
	public:
		template <typename ... Passes>
		Renderer(Passes ... passes) {AddPass((passes)...);}
		Renderer(const Renderer& other) = default;
		Renderer(Renderer&& other) noexcept = default;
		Renderer& operator=(const Renderer& other) = default;
		Renderer& operator=(Renderer&& other)noexcept = default;
		~Renderer() = default;
		Pass& GetPass(Pass::Name name);
		void Init(ID3D11DeviceContext* context) { m_context = context; }
		void AddPass(Pass& pass);
		bool Bound(const Bindable::Base::handle& bindable);
		void Draw();

	private:
		template <typename ... Passes>
		void AddPass(Pass& pass, Passes ... rest)
		{
			AddPass(pass);
			AddPass((rest)...);
		}
		ID3D11DeviceContext* m_context;
		Pass::Container m_passes;
		Bindable::Base::Container m_currentBindings;
	};
	
}

