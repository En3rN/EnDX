#include "Pass.h"
#include "Mesh.h"
#include "Renderer.h"
#include <algorithm>

namespace En3rN::DX
{
	RenderPass::RenderPass(std::string&& name, const RenderTarget::Container& rendertargets, DepthStencil::Ptr depthStencil):
		m_name(std::move(name)),
		m_dsv(depthStencil)
	{
		std::for_each(begin(rendertargets), end(rendertargets),
			[&](const RenderTarget& rtv) {
				auto resource = rtv.GetResource();
				auto type = resource.GetDimension();
				auto desc = resource.GetDesc();
				switch(type)
				{
				case D3D11_RESOURCE_DIMENSION_UNKNOWN:
					break;
				case D3D11_RESOURCE_DIMENSION_BUFFER:
					m_vps.push_back(CD3D11_VIEWPORT(resource.GetResourceType<ID3D11Buffer>().Get(), rtv.GetP()));
					break;
				case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
					m_vps.push_back(CD3D11_VIEWPORT(resource.GetResourceType<ID3D11Texture1D>().Get(), rtv.GetP()));
					break;
				case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
					m_vps.push_back(CD3D11_VIEWPORT(resource.GetResourceType<ID3D11Texture2D>().Get(), rtv.GetP()));
					break;
				case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
					m_vps.push_back(CD3D11_VIEWPORT(resource.GetResourceType<ID3D11Texture3D>().Get(), rtv.GetP()));
					break;
				default:
					break;
				}
				m_rtvs.push_back(rtv.GetP());
			});
	}
	void RenderPass::AddJob(const RenderJob& job)
	{
		m_jobs.push_back(job);
		return;
	}
	void RenderPass::Bind()
	{
		std::ranges::for_each(m_binds, [](const auto& bind) { bind->Bind(); });
	}
	void RenderPass::SetRenderTargets(ID3D11DeviceContext* context)
	{
		context->OMSetRenderTargets(std::size(m_rtvs), m_rtvs.data(), m_dsv);
		context->RSSetViewports(std::size(m_vps), m_vps.data());
	}
	void RenderPass::UnbindRenderTargets(ID3D11DeviceContext* context)
	{
		ID3D11RenderTargetView* nullRTV = nullptr;
		context->OMSetRenderTargets(std::size(m_rtvs), &nullRTV, m_dsv);
	}
	void RenderPass::ProcessJobs()
	{
	}
	void RenderPass::Execute(Renderer& renderer)
	{
		for(auto& job : m_jobs) {
			auto& mesh = job.GetMesh();
			auto& material = job.GetMaterial();
			auto& step = job.GetStep();
			mesh.Bind();
			material.Bind();
			for(auto& bindable : step.GetBindables()) {
				bindable->Bind();
			}
			renderer.GetContext()->DrawIndexed(mesh.GetIndexBuffer()->GetCount(), 0, 0);
		};
		m_jobs.clear();
	}
}