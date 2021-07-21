#pragma once
#include "Drawable.h"
#include "end3d11.h"

namespace En3rN::DX
{
	class Sampler : public Bindable
	{
	public:
		enum class State { Wrap=1, Mirror, Clamp,Border, Mirror1};
		Sampler(State state);
		std::string GetKey(State state);
		void Bind() override;
	private:
		State state;
		ComPtr<ID3D11SamplerState> pSampler;

	};
}

