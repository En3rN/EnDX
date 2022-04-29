#pragma once

#include "end3d11.h"
#include "Pass.h"
#include "Light.h"


namespace En3rN::DX
{
	class Renderer : public GfxResources
	{
	public:
		Renderer();
		Renderer(const Renderer& other) = default;
		Renderer(Renderer&& other) noexcept = default;
		Renderer& operator=(const Renderer& other) = default;
		Renderer& operator=(Renderer&& other)noexcept = default;
		template <typename ... Passes>
		Renderer(Passes ... passes){
			AddPass((passes)...)
		;}
		~Renderer() noexcept = default;

		RenderPass& GetPass(const std::string& passName);
		RenderTarget& GetRenderTarget(const std::string& rtName);		

		void AddRenderTarget(const RenderTarget& renderTarget);
		void AddPass(std::unique_ptr<RenderPass>&& pass);
		template <typename ... Passes>
		void AddPass(RenderPass pass, Passes ... rest)
		{
			AddPass(pass);
			AddPass(rest...);
		}
		bool Bound(const Bindable::Base::handle& bindable);
		void Draw();

	private:
		
		RenderTarget::Container m_renderTargets;
		DepthStencil::Container	m_depthStencils;
		RenderPass::Container	m_passes;
	};
	
}

