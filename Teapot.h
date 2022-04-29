#pragma once
#include "Drawable.h"
#include <..\..\DirectXTK\Inc\GeometricPrimitive.h>
namespace En3rN::DX
{

	class Teapot : Drawable
	{
	public:
		Teapot()
		{
			using namespace DirectX;
			GeometricPrimitive::CreateTeapot(GetContext());
		}

	};
}
