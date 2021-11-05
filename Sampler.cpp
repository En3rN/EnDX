#include "Sampler.h"
#include "enexception.h"

namespace En3rN::DX
{
	Sampler::Sampler(State state) : state(state)
	{
		CD3D11_SAMPLER_DESC sampDesc(D3D11_DEFAULT);

		sampDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)state;
		sampDesc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)state;
		sampDesc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)state;
		sampDesc.MipLODBias = 0;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
		sampDesc.MaxAnisotropy = 16;
		errchk::hres(pDevice->CreateSamplerState(&sampDesc, &pSampler), EnExParam);
	}
	void Sampler::Bind()
	{
		if (IsBound())
			return;
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