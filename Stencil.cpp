#include "Stencil.h"
#include "enexception.h"

namespace En3rN::DX
{
	DepthStencilState::DepthStencilState(Depth depth, Stencil stencil, UINT stencilRef ) :
		m_depth(depth), m_stencil(stencil),m_ref(stencilRef)
	{
		D3D11_DEPTH_STENCIL_DESC desc{};
		desc.DepthEnable = TRUE;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		desc.StencilEnable = FALSE;
		desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		desc.BackFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;

		desc.FrontFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NOT_EQUAL;
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;

		switch(depth)
		{
		case En3rN::DX::DepthStencilState::Depth::Enable:
			break;
		case En3rN::DX::DepthStencilState::Depth::Disable:
			desc.DepthEnable = FALSE;
			break;
		case En3rN::DX::DepthStencilState::Depth::FuncLessEqualNoWrite:
			desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;	// D3D11_COMPARISON_LESS_EQUAL D3D11_COMPARISON_EQUAL
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
			break;
		default:
			break;
		}
		
		switch(stencil)
		{
		case En3rN::DX::DepthStencilState::Stencil::Disable:
			break;
		case En3rN::DX::DepthStencilState::Stencil::Read:
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			desc.StencilEnable = TRUE;
			desc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
			desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			break;
		case En3rN::DX::DepthStencilState::Stencil::Write:
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			desc.StencilEnable = TRUE;
			desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
			desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
			break;
		default:
			break;
		}

		errchk::hres(pDevice->CreateDepthStencilState(&desc, &pStencilState));
	}
	void DepthStencilState::Bind()
	{
		pContext->OMSetDepthStencilState(pStencilState.Get(), m_ref);
	}
}
