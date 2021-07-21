#include "Blend.h"
#include "enexception.h"
#include <string>

namespace En3rN::DX
{
	Blend::Blend(State state): state(state)
	{
		D3D11_RENDER_TARGET_BLEND_DESC rtbd{};
		D3D11_BLEND_DESC blendDesc{};

		switch (state)
		{
		case Blend::State::Default:
			blendDesc.AlphaToCoverageEnable = FALSE;
			blendDesc.IndependentBlendEnable = FALSE;
			rtbd = {
				FALSE,
				D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD,
				D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD,
				D3D11_COLOR_WRITE_ENABLE_ALL,
			};
			break;
		case Blend::State::Enabled:
			blendDesc.AlphaToCoverageEnable = TRUE;
			rtbd.BlendEnable = TRUE;
			rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
			rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
			rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
			rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
			rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;
			break;
		case Blend::State::Custom:
			break;
		}
		for (UINT i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
			blendDesc.RenderTarget[i] = rtbd;
		errchk::hres(pDevice->CreateBlendState(&blendDesc, &pBlenderState), EnExParam);

	}
	std::string Blend::GetKey()
	{
		return typeid(Blend).name()+'#'+std::to_string((int)state);
	}
	void Blend::Bind()
	{
		pContext->OMSetBlendState(pBlenderState.Get(), nullptr, 0xFFFFFFFF);

	}
}