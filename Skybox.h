#pragma once
#include "Drawable.h"
#include "Plane.h"

namespace En3rN::DX
{
	class Skybox : public DynamicDrawable
	{
	public:
		Skybox(std::wstring file);
		void Draw() override;
		void Update(float dt) override;
	private:
	};
}
