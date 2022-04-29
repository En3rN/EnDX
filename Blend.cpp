#include "Blend.h"
#include "enexception.h"
#include <string>

namespace En3rN::DX
{
	Blender::Blender(State state): state(state)
	{
		D3D11_RENDER_TARGET_BLEND_DESC rtbd{};
		D3D11_BLEND_DESC blendDesc{};

		switch (state)
		{
		case Blender::State::Default:
			blendDesc.AlphaToCoverageEnable = FALSE;
			blendDesc.IndependentBlendEnable = FALSE;
			rtbd = {
				FALSE,
				D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD,
				D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD,
				D3D11_COLOR_WRITE_ENABLE_ALL,
			};
			break;
		case Blender::State::Enabled:
			blendDesc.AlphaToCoverageEnable = TRUE;
			blendDesc.IndependentBlendEnable = TRUE;
			rtbd.BlendEnable = TRUE;
			rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
			rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
			rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
			rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
			rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;
			break;
		case Blender::State::Custom:
			break;
		}
		for (UINT i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
			blendDesc.RenderTarget[i] = rtbd;
		errchk::hres(GetDevice()->CreateBlendState(&blendDesc, &pBlenderState));

	}	
	void Blender::Bind()
	{
		GetContext()->OMSetBlendState(pBlenderState.Get(), nullptr, 0xFFFFFFFF);

	}
}