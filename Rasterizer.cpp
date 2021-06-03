#include "Rasterizer.h"
#include "enexception.h"

En3rN::DX::Rasterizer::Rasterizer(CullMode cull)
{
	CD3D11_RASTERIZER_DESC rasterizerDesc_CullFront(D3D11_DEFAULT);
	rasterizerDesc_CullFront.CullMode = (D3D11_CULL_MODE)cull;
	errchk::hres(pDevice->CreateRasterizerState(&rasterizerDesc_CullFront, &pRasterizerState), EnExParam);
}

void En3rN::DX::Rasterizer::Bind()
{
	pContext->RSSetState(pRasterizerState.Get());
}
