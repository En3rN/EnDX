#include "Rasterizer.h"
#include "enexception.h"
namespace En3rN::DX
{
	Rasterizer::Rasterizer(State state) : state(state)
	{
		CD3D11_RASTERIZER_DESC desc{};
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = (D3D11_CULL_MODE)state;
		desc.FrontCounterClockwise = FALSE;
		desc.DepthBias = D3D11_DEFAULT_DEPTH_BIAS;
		desc.DepthBiasClamp = D3D11_DEFAULT_DEPTH_BIAS_CLAMP;
		desc.SlopeScaledDepthBias = D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		desc.DepthClipEnable = TRUE;
		desc.ScissorEnable = FALSE;
		desc.MultisampleEnable = TRUE;
		desc.AntialiasedLineEnable = TRUE;
		errchk::hres(GetDevice()->CreateRasterizerState(&desc, &pRasterizerState));
	}
	void Rasterizer::Bind()
	{
		GetContext()->RSSetState(pRasterizerState.Get());
	}
}
