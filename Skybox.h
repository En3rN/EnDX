#pragma once
#include "Drawable.h"
#include "Plane.h"

namespace En3rN::DX
{
	class Skybox : public Drawable
	{
	public:
		Skybox(std::string file);
		void Draw() override;
		void Update(float dt) override;
	private:
	};
}
