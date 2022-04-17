#pragma once

#include "Job.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "Resource.h"
#include "ViewPort.h"
#include "Container.h"
#include <string>
#include <memory>
#include <vector>

namespace En3rN::DX
{
	class Renderer;
	class RenderPass
	{
	public:
		//using handle = std::unique_ptr<Pass>;
		using Container = ::std::vector<std::unique_ptr<RenderPass>>;		
		RenderPass(std::string&& name, const RenderTarget::Container& rendertargets, DepthStencil::Ptr depthStencil = nullptr);
		/*RenderPass(RenderPass&& other) noexcept = default;*/
		const RenderTarget::Ptr::Container& GetRTVs() const { return m_rtvs; }
		DepthStencil::Ptr GetDSV() { return m_dsv; }
		
		void AddJob(const RenderJob& job);
		void Bind();
		void SetRenderTargets(ID3D11DeviceContext* context);
		void UnbindRenderTargets(ID3D11DeviceContext* context);
		virtual void ProcessJobs();
		virtual void Execute(Renderer& renderer);
		const std::string& GetName() const { return m_name; }
		RenderJob::Container& GetJobs() { return m_jobs; }
		virtual ~RenderPass() = default;
	protected:
		std::string						m_name;
		/*VSConstantBuffer                m_vsConstantBuffer;
		PSConstantBuffer				m_psConstantBuffer;*/
		RenderTarget::Ptr::Container	m_rtvs; //renderTargets to use;
		DepthStencil::Ptr				m_dsv;
		ViewPort::Container				m_vps;
		RenderJob::Container			m_jobs;
		Bindable::Base::Container		m_binds = {
			BindableManager::Query<Transform::ConstantBuffer>(DirectX::XMMatrixIdentity(), 1u,1u,"Identity")
		};
	};
}
