#include "GfxResources.h"
#include "Graphics.h"
#include "EnDX.h"


namespace En3rN::DX
{
	GfxResources::GfxResources(){
		Graphics& gfx = EnDX::Get().GetWindow().GetGfx();
		pDevice = gfx.pDevice.Get();
		pContext = gfx.pContext.Get();
#ifdef _DEBUG
		pInfoQ = gfx.pInfoQ.Get();
//#else	pInfoQ = nullptr;
#endif // DEBUG;
	}
	ID3D11DeviceContext& GfxResources::GetContext(){
		Graphics& gfx =EnDX::Get().GetWindow().GetGfx();
		return *gfx.pContext.Get();
	}
	ID3D11Device& GfxResources::GetDevice(){
		Graphics& gfx = EnDX::Get().GetWindow().GetGfx();
		return *gfx.pDevice.Get();
	}
	ID3D11InfoQueue& GfxResources::GetInfoQ(){
		Graphics& gfx = EnDX::Get().GetWindow().GetGfx();
		return *gfx.pInfoQ.Get();
	}
}
