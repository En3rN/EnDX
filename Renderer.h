#pragma once

#include "end3d11.h"
#include "Pass.h"
#include "Light.h"


namespace En3rN::DX
{
	class Renderer
	{
	public:
		Renderer(Graphics& gfx);
		template <typename ... Passes>
		Renderer(Graphics& gfx , Passes ... passes) {AddPass((passes)...);}
		Renderer(const Renderer& other) = default;
		Renderer(Renderer&& other) noexcept = default;
		Renderer& operator=(const Renderer& other) = default;
		Renderer& operator=(Renderer&& other)noexcept = default;
		~Renderer() = default;

		RenderPass& GetPass(const std::string& passName);
		RenderTarget& GetRenderTarget(const std::string& rtName);
		ID3D11DeviceContext* GetContext() { return m_context; }

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
		ID3D11DeviceContext* m_context;
		IDXGISwapChain* m_swapChain;
		D3D11_VIEWPORT m_viewPorts[8]{};
		D3D11_RECT m_scissorRect[8]{};
		
		RenderTarget::Container m_renderTargets;
		DepthStencil::Container	m_depthStencils;
		RenderPass::Container	m_passes;
	};
	
}

