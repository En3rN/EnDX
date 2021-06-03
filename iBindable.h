#pragma once
#include "iHandle.h"
#include "GfxResources.h"
#include <vector>
//#include <DirectXMath.h>

namespace En3rN::DX
{
	class Buffer;
	class Graphics;
	class Keyboard;
	class Mouse;
	class DynamicBindable;
	class Camera;
	class Bindable : public GfxResources , unPtr(Bindable)
	{
	public:
		Bindable() = default;
		Bindable(Bindable && other) noexcept = default;
		virtual ~Bindable() = default;
		virtual void Bind() = 0;
	protected:
	};
	
	class DynamicBindable : public Bindable
	{
	public:
		DynamicBindable() = default;
		DynamicBindable(DynamicBindable && other) = default;
		virtual ~DynamicBindable() = default;
		virtual void Update() = 0;
	};

	
}
