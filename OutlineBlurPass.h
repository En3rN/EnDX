#pragma once
#include "Pass.h"
#include "Shader.h"
#include "Renderer.h"
#include "Job.h"
#include "InfoManager.h"
#include "ScopedBinder.h"

namespace En3rN::DX
{
	class DrawOutlineBlurPass : public RenderPass
	{
	public:
		DrawOutlineBlurPass(std::string&& name, const RenderTarget::Container& rendertargets, DepthStencil::Ptr depthStencil = nullptr) :
			RenderPass(std::move(name), rendertargets, depthStencil)
		{
			m_binds.push_back(BindableManager::Query<ComputeShader>("VerticalBlurCS.cso"));
		}
		void Execute(Renderer& renderer) override
		{
			for( auto& job : m_jobs ) {
				auto& mesh = job.GetMesh();
				auto& material = job.GetMaterial();
				auto& step = job.GetStep();
				auto& bindables = step.GetBindables();
				mesh.Bind();
				material.Bind();

				for( auto& bindable : bindables ) {
					bindable->Bind();
				}
				GetContext()->DrawIndexed( mesh.GetIndexBuffer()->GetCount(), 0, 0 );
			};
			m_jobs.clear();

			if( !InfoManager::Empty() )
				throw EnExDXInfo();
			UnbindRenderTargets();
			if( !InfoManager::Empty() )
				throw EnExDXInfo();
			static Resource resource = renderer.GetRenderTarget("Outline").GetResource();
			static Texture input(resource);
			auto desc = std::get<2>(resource.GetDesc());
			auto dim = resource.GetDimension();
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			static Texture output = *BindableManager::Query<Texture>(resource, 0, Texture::Flag::NoCopy, m_name);

			static Texture::DXPtr unBindTex[]={nullptr,nullptr};
			static Texture::DXPtr bindTex[] = { input.GetP(),output.GetP()};
			
			GetContext()->CSSetShaderResources( 0, 2, bindTex );

			assert(desc.Width < D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
			assert(desc.Height < D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
			assert(desc.ArraySize < D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
			GetContext()->Dispatch(1, 1, 1);
			if( !InfoManager::Empty() )
				throw EnExDXInfo();
			
			GetContext()->CSSetShaderResources( 0, 2, unBindTex );
			if(!InfoManager::Empty())
				throw EnExDXInfo();

			//renderer.GetRenderTarget("Outline") = RenderTarget("Outline", output.GetResource());
		}
	};
}

