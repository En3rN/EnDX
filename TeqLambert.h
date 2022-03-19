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
#include "Blend.h"



namespace En3rN::DX
{
	class Model;
	//class for initializing default teqniques
	class Teqnique::Lambertian : public Teqnique
	{
	public:
		Teqnique::Lambertian()
		{
			AddStep(Step(Pass::Name::Phong,
				BindableManager::Query<Sampler>(Sampler::State::Wrap),
				BindableManager::Query<Rasterizer>(Rasterizer::State::Back),
				BindableManager::Query<Blender>(Blender::State::Default),
				BindableManager::Query<Stencil>(Stencil::State::DepthOnly),
				BindableManager::Query<Transform::ConstantBuffer> (1,1)
			));
		}
	private:

	};

}
