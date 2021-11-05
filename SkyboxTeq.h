#pragma once
#include "Teqnique.h"
#include "BindableManager.h"
#include "Sampler.h"
#include "Stencil.h"
#include "Rasterizer.h"
#include "Topology.h"


namespace En3rN::DX
{
	class TeqSkybox : public Teqnique
	{
	public:
		using handle = std::shared_ptr<TeqSkybox>;
		TeqSkybox()
		{
			AddStep(Step(Pass::Name::Skybox,
				BindableManager::Query<Topology>(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
				BindableManager::Query<Sampler>(Sampler::State::Wrap),
				BindableManager::Query<Rasterizer>(Rasterizer::State::Front),
				BindableManager::Query<Stencil>(Stencil::State::DepthOnlyFuncLessEqualNoWrite)
			));
		}
	};

}
