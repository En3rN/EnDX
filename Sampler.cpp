#include "Sampler.h"
#include "enexception.h"

En3rN::DX::Sampler::Sampler(State state) : state(state)
{
	CD3D11_SAMPLER_DESC sampDesc(D3D11_DEFAULT);
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)state;
	sampDesc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)state;
	sampDesc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)state;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
	errchk::hres(pDevice->CreateSamplerState(&sampDesc, &pSampler), EnExParam);
}

 std::string En3rN::DX::Sampler::GetKey(State state)
{
	return typeid(Sampler).name() + '#' + std::to_string((int)state);
}

void En3rN::DX::Sampler::Bind()
{
	pContext->PSSetSamplers(0, 1, pSampler.GetAddressOf());
}
