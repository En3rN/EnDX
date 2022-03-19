#include "Teqnique.h"
#include "Light.h"
#include "Sampler.h"
#include "Rasterizer.h"
#include "Blend.h"
#include "Stencil.h"


namespace En3rN::DX
{

	Teqnique Teqnique::Setup()
	{
		Teqnique setupTeq;
		setupTeq.AddStep(Step(Pass::Name::Setup));
		return setupTeq;
	}
	Teqnique Teqnique::Phong()
	{
		Teqnique phong("Phong");
		
		phong.AddStep(Step(Pass::Name::Phong,
			BindableManager::Query<Sampler>(Sampler::State::Wrap),
			BindableManager::Query<Rasterizer>(Rasterizer::State::Back),
			BindableManager::Query<Blender>(Blender::State::Default),
			BindableManager::Query<Stencil>(Stencil::State::DepthOnly),
			BindableManager::Query<Transform::ConstantBuffer> (1, 1)
		));
		return phong;
	}
	Teqnique Teqnique::Unlit()
	{
		Teqnique unlit("Unlit");
		unlit.AddStep(Step(Pass::Name::Unlit,
			BindableManager::Query<Sampler>(Sampler::State::Wrap),
			BindableManager::Query<Rasterizer>(Rasterizer::State::Back),
			BindableManager::Query<Blender>(Blender::State::Default),
			BindableManager::Query<Stencil>(Stencil::State::DepthOnly),
			BindableManager::Query<Transform::ConstantBuffer> (1, 1)
		));
		return unlit;
	}
	Teqnique Teqnique::Fullscreen()
	{
		Teqnique fullscreen;
		fullscreen.AddStep(Step(Pass::Name::Fullscreen,
			// todo off screen RenderTarget 
			BindableManager::Query<Sampler>(Sampler::State::Wrap),
			BindableManager::Query<Rasterizer>(Rasterizer::State::Back),
			BindableManager::Query<Blender>(Blender::State::Default),
			BindableManager::Query<Stencil>(Stencil::State::DepthOnly),
			BindableManager::Query<Transform::ConstantBuffer> (1, 1)
		));
		return fullscreen;
	}
	Teqnique Teqnique::Skybox()
	{
		Teqnique skybox;
		Step skyboxpass(Pass::Name::Skybox,
			BindableManager::Query<Sampler>(Sampler::State::Wrap),
			BindableManager::Query<Rasterizer>(Rasterizer::State::Front),
			BindableManager::Query<Stencil>(Stencil::State::DepthOnlyFuncLessEqualNoWrite),
			BindableManager::Query<Transform::ConstantBuffer>(2, 1)
		);
		return skybox;
	}
}
