#pragma once
#include "Teqnique.h"
#include "BindableManager.h"
#include "Sampler.h"
#include "Stencil.h"
#include "Rasterizer.h"
#include "Topology.h"
#include "Buffer.h"
#include "Material.h"
#include "Transform.h"



namespace En3rN::DX
{
	class Model;
	class TeqLambertian : public Teqnique
	{
	public:
		using handle = std::shared_ptr<TeqLambertian>;
		TeqLambertian()
		{
			
			AddStep(Step(Pass::Name::Lambertian,
				BindableManager::Query<Sampler>(Sampler::State::Wrap),
				BindableManager::Query<Rasterizer>(Rasterizer::State::Back),
				BindableManager::Query<Stencil>(Stencil::State::DepthOnly),
				BindableManager::Query<Material::ConstantBuffer> (1,1),
				BindableManager::Query<Transform::ConstantBuffer> (1,1),
				BindableManager::Query<PixelShader>("test.cso")
			));
		}
	private:

	};

}
