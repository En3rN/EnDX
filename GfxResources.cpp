#include "GfxResources.h"
#include "Graphics.h"
#include "EnDX.h"


namespace En3rN::DX
{
	GfxResources::GfxResources(){
		Graphics& gfx = EnDX::Get().GetWindow().GetGfx();
		pDevice = gfx.pDevice.Get();
		pContext = gfx.pContext.Get();
	}
	ID3D11DeviceContext& GfxResources::GetContext(){
		Graphics& gfx =EnDX::Get().GetWindow().GetGfx();
		return *gfx.pContext.Get();
	}
	ID3D11Device& GfxResources::GetDevice(){
		Graphics& gfx = EnDX::Get().GetWindow().GetGfx();
		return *gfx.pDevice.Get();
	}	
}
