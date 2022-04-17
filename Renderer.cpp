#include "Renderer.h"
#include "Mesh.h"
#include "Step.h"
#include "Shader.h"
#include "Transform.h"
#include "InputLayout.h"
#include "enexception.h"
#include "Graphics.h"
#include "Texture.h"
#include "FullscreenPass.h"
#include "OutlineBlurPass.h"

#include <algorithm>

namespace En3rN::DX
{
	Renderer::Renderer(Graphics& gfx): 
		m_context(gfx.pContext.Get()) , 
		m_swapChain(gfx.pSwapChain.Get())
	{
		auto bId = std::string("BackBuffer");
		auto desc = std::get<2>(gfx.GetBackBuffer().GetDesc());
		auto outLine = std::string("Outline");
				
		m_renderTargets.push_back(RenderTarget(bId,gfx.GetBackBuffer()));
		m_renderTargets.push_back(RenderTarget(outLine, desc.Width, desc.Height,true));
		m_depthStencils.push_back(DepthStencil(desc.Width, desc.Height));

		RenderTarget::Container backBuffer = { GetRenderTarget(bId) };
		RenderTarget::Container outlineRTV = { GetRenderTarget(outLine) };		
		
		
		AddPass(std::make_unique <RenderPass>("Phong", backBuffer, m_depthStencils[0].GetP()));
		AddPass(std::make_unique <RenderPass>("Unlit", backBuffer, m_depthStencils[0].GetP()));
		AddPass(std::make_unique <RenderPass>("MaskBackBuffer", backBuffer, m_depthStencils[0].GetP()));
		AddPass(std::make_unique <DrawOutlineBlurPass>("DrawOutline", outlineRTV));
		AddPass(std::make_unique <FullscreenPass>("FullScreen", backBuffer, m_depthStencils[0].GetP()));
		AddPass(std::make_unique <RenderPass>("imGui", backBuffer));
	}
	RenderPass& En3rN::DX::Renderer::GetPass(const std::string& passName)
	{
		auto result = std::find_if(begin(m_passes), end(m_passes), [&](const auto& pass) {
			return pass->GetName() == passName;
			});
		if(result != end(m_passes))
			return *result->get();
		else
			throw EnExcept("Pass Not Found :" + passName);
	}

	RenderTarget& Renderer::GetRenderTarget(const std::string& rtName)
	{
		auto result = std::find_if(begin(m_renderTargets), end(m_renderTargets), [&](const auto& rt) {
			return rt.GetName() == rtName;
			});
		if(result != end(m_renderTargets))
			return *result;
		else
			throw EnExcept("RenderTarget Not Found :" + rtName);
	}

	void Renderer::AddRenderTarget(const RenderTarget& renderTarget)
	{
		m_renderTargets.push_back(renderTarget);
	}

	void Renderer::AddPass(std::unique_ptr<RenderPass>&& pass)
	{

		m_passes.push_back(std::move(pass));
	}

	bool Renderer::Bound(const Bindable::Base::handle& bindable)
	{
		return false;
	}

	void En3rN::DX::Renderer::Draw()
	{
		auto execute = [&](RenderPass& pass) {
			auto& rtvs = pass.GetRTVs();
			m_context->OMSetRenderTargets(rtvs.size(), rtvs.data(), pass.GetDSV());
			for (auto& job : pass.GetJobs()){
				auto& mesh = job.GetMesh();
				auto& material = job.GetMaterial();
				mesh.Bind();
				material.Bind();
				for (auto& bindable : job.GetStep().GetBindables()){
					bindable->Bind();
				}
				m_context->DrawIndexed(mesh.GetIndexBuffer()->GetCount(), 0, 0);
			};
			pass.GetJobs().clear();
		};
		auto callClear = [](auto& buffer) {
			buffer.Clear();
		};
		auto bindAndCallExec = [&](auto& pass) {
			
			auto& rtvs = pass->GetRTVs();
			for(auto i = 0; i < std::size(m_viewPorts) && i < rtvs.size(); ++i) {
				static Resource resource;
				rtvs[i]->GetResource(resource.ReleaseAndGetPP());
				auto desc = std::get<2>(resource.GetDesc());
				m_viewPorts[i].Width = desc.Width;
				m_viewPorts[i].Height = desc.Height;
				m_viewPorts[i].MaxDepth = 1;
			}
			m_context->OMSetRenderTargets(rtvs.size(), rtvs.data(), pass->GetDSV());
			m_context->RSSetViewports(8, m_viewPorts);
			pass->Execute(*this); 
		};
		auto executePass = [&](auto& pass) {
			pass->SetRenderTargets(m_context);
			pass->Bind();
			pass->Execute(*this);
		};
		std::for_each(begin(m_renderTargets), end(m_renderTargets), callClear);
		std::for_each(begin(m_depthStencils), end(m_depthStencils), callClear);
		std::for_each(begin(m_passes),end(m_passes), executePass);		
	}
}