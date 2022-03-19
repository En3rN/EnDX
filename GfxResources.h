#pragma once
#include "end3d11.h"

namespace En3rN::DX
{
	class GfxResources
	{
		friend class Drawable;
		friend class Bindable;
		friend class InfoManager;
	protected:
		GfxResources();
		GfxResources(GfxResources&& other) noexcept = default;
		GfxResources& operator = (GfxResources&& other) noexcept = default;
		virtual ~GfxResources() = default;
		static ID3D11DeviceContext& GetContext();
		static ID3D11Device& GetDevice();
		ID3D11Device* pDevice;
		ID3D11DeviceContext* pContext;
#ifdef _DEBUG
		ID3D11InfoQueue* pInfoQ = nullptr;
#endif // _DEBUG || DEBUG
	};
}

