#include "GfxResources.h"
#include "Graphics.h"
#include "EnDX.h"


namespace En3rN::DX
{	
	Graphics& GfxResources::GetGfx() {
		static Graphics& gfx = EnDX::Get().GetWindow().GetGfx();		
		return gfx;
	}
	ID3D11DeviceContext* GfxResources::GetContext(){
		
		return GetGfx().pContext.Get();
	}
	ID3D11Device* GfxResources::GetDevice(){
		return GetGfx().pDevice.Get();
	}
	IDXGISwapChain* GfxResources::GetSwapChain() {
		return GetGfx().pSwapChain.Get();
	}

}
