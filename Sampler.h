#pragma once
#include "iBindable.h"
#include "end3d11.h"

namespace En3rN::DX
{
	class Sampler : public Bindable//, public ActiveBindable<Sampler>
	{
	public:
		using handle = std::shared_ptr<Sampler>;
		enum class State { Wrap=1, Mirror, Clamp,Border, Mirror1};
		Sampler(State state);
		void Bind() override;
	private:
		State state;
		ComPtr<ID3D11SamplerState> pSampler;
	};
}

