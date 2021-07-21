#include "Rasterizer.h"
#include "enexception.h"

En3rN::DX::Rasterizer::Rasterizer(State state): state(state)
{
	CD3D11_RASTERIZER_DESC desc(D3D11_DEFAULT);
	desc.CullMode = (D3D11_CULL_MODE)state;
	errchk::hres(pDevice->CreateRasterizerState(&desc, &pRasterizerState), EnExParam);
}

std::string En3rN::DX::Rasterizer::GetKey(State state)
{
	return typeid(Rasterizer).name()+'#'+std::to_string((int)state);
}

void En3rN::DX::Rasterizer::Bind()
{
	pContext->RSSetState(pRasterizerState.Get());
}
