#pragma once
#include "Entity.h"
#include "Mesh.h"
#include "Drawable.h"
#include "Vertex.h"
#include <string>


namespace En3rN::DX
{
	class Plane : public Drawable
	{
	public:
		Plane(std::wstring file);
		void LoadTexture(std::wstring & file);
		void Draw() override;
	private:
	};
}

