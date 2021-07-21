#pragma once
#include "Entity.h"
#include "Mesh.h"
#include "Drawable.h"
#include "Vertex.h"
#include "Behavior.h"
#include <string>


namespace En3rN::DX
{
	class Plane : public DynamicDrawable
	{
	public:
		Plane();
		Plane(Vec3f pos);
		void LoadTexture(std::wstring & file);
		void Update(float dt) override;
		void Draw() override;
	private:
		Behaviors::Behavior behavior;
	};
}

