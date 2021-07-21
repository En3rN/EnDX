#pragma once
#include <vector>
#include "end3d11.h"
#include "Drawable.h"
#include "Camera.h"

namespace En3rN::DX
{
	class Window;
	class Graphics;
	class Keyboard;
	class Mouse;
	class Scene: unPtr(Scene)
	{
	public:
		Scene()= default;
		~Scene() = default;
		Scene(Scene && other) = default;
		void AddDrawable(Drawable::handle drawable);
		Scene& operator << (Drawable::handle drawable)
		{
			AddDrawable(std::move(drawable));
			return *this;
		}
		void Bind();
		void Update(float dt);
		void Draw();

	private:
		Camera camera { 0, 0,-4 ,5, 1000};
		int selected = 0;
		std::vector<std::unique_ptr<Drawable>> drawables;
		std::vector<int> dynamicDrawableIndex;
	};
}

