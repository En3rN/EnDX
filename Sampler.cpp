#include "Sampler.h"
#include "enexception.h"

namespace En3rN::DX
{
	Sampler::Sampler(State state) : state(state)
	{
		D3D11_SAMPLER_DESC sampDesc{};
		sampDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
		sampDesc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)state;
		sampDesc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)state;
		sampDesc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)state;
		sampDesc.MipLODBias = 0;
		sampDesc.MinLOD = -D3D11_FLOAT32_MAX;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		sampDesc.MaxAnisotropy = 16;
		errchk::hres(pDevice->CreateSamplerState(&sampDesc, &pSampler));
	}
	void Sampler::Bind()
	{
		pContext->PSSetSamplers(0, 1, pSampler.GetAddressOf());
	}
	/*bool Sampler::IsBound()
	{
		ID3D11SamplerState* current;
		pContext->PSGetSamplers(0, 1, &current);
		if (pSampler.Get() == current)
			return true;
		return false;
	}*/
}