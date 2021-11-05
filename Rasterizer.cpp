#include "Rasterizer.h"
#include "enexception.h"
namespace En3rN::DX
{

	Rasterizer::Rasterizer(State state) : state(state)
	{
		CD3D11_RASTERIZER_DESC desc(D3D11_DEFAULT);
		desc.CullMode = (D3D11_CULL_MODE)state;
		//desc.ScissorEnable = TRUE;
		desc.MultisampleEnable = TRUE;
		desc.AntialiasedLineEnable = TRUE;
		errchk::hres(pDevice->CreateRasterizerState(&desc, &pRasterizerState), EnExParam);
	}
	void Rasterizer::Bind()
	{
		pContext->RSSetState(pRasterizerState.Get());
	}
}
