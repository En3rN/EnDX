#pragma once
#include "Pass.h"
#include "Step.h"
#include "Job.h"
#include "Renderer.h"
#include "Texture.h"
#include "Model.h"
#include "vec.h"
#include "enBuffer.h"
#include <algorithm>


namespace En3rN::DX
{
	class FullscreenPass: public RenderPass
	{
	public:
		//Lookup ref for Renderer.GetRenderTarget
		FullscreenPass(std::string&& name, const RenderTarget::Container& rendertargets, DepthStencil::Ptr depthStencil = nullptr) :
			RenderPass(std::move(name), rendertargets, depthStencil)
		{
			Vec3f positions[] = {
				{-1,-1,0},{-1,1,0},{1,1,0},{1,-1,0}
			};
			Vec3f texCords[] = {
				{0,1,0},{0,0,0},{1,0,0},{1,1,0}
			};
			enBuffer::Indecies indecies = {
				0,1,3,3,1,2
			};
			enBuffer vBuf;
			vBuf.add_element(positions, "Position",std::size(positions));
			vBuf.add_element(texCords, "TexCoord", std::size(texCords));
			
			auto vs = BindableManager::Query<VertexShader>(m_name, "main");
			Bindable::Base::Container binds = {
				BindableManager::Query<InputLayout>(vs->GetSignatures(), vs->GetBlob(), "m_name"),
				BindableManager::Query<VertexBuffer>(vBuf, m_name),
				BindableManager::Query<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
				BindableManager::Query<IndexBuffer>(indecies, "FullScreenSurface")
			};
			binds.push_back(std::move(vs));
			m_binds = std::move(binds);
		}
		void ProcessJobs() override 
		{
			std::string input{};
		}
		void Execute(Renderer& renderer) override
		{
			static Resource dummy;
			static Texture::handle input; 
			static PixelShader ps = *BindableManager::Query<PixelShader>("Unlit", "Diffuse").get();

			for(auto& job : m_jobs) {
				auto& step = job.GetStep();
				for(auto& bindable : step.GetBindables())
					bindable->Bind();
			}
				ps.Bind();				
				input = std::make_shared<Texture>( renderer.GetRenderTarget( "Outline" ).GetResource() );
				input->Bind();
				GetContext()->DrawIndexed(6, 0, 0);
				ID3D11ShaderResourceView* nullSrv = nullptr;				
				GetContext()->PSSetShaderResources(0, 1, &nullSrv);
			m_jobs.clear();
		}
	private:
		std::string m_inputPassName;
	};
}

