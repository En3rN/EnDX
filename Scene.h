#pragma once
#include "Model.h"
#include "end3d11.h"
#include "Drawable.h"
#include "Camera.h"
#include "Renderer.h"
#include <vector>

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
		Scene(Renderer * renderer) : m_renderer(renderer) {}
		Scene(Scene && other) = default;
		void AddDrawable(Drawable::handle drawable);
		Scene& operator << (Drawable::handle drawable)
		{
			AddDrawable(std::move(drawable));
			return *this;
		}
		void AddModel(Model::handle model);
		void Bind(Renderer & renderer);
		void Update(float dt);
		void Draw();

	private:
		Renderer* m_renderer;
		Camera camera{ 0, 0,-4 ,10,0.5f, 1000 };
		int selected = 0;
		std::vector<std::unique_ptr<Drawable>> drawables;
		Model::Container models;
	};
}

