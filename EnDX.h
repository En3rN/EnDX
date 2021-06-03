#pragma once
#include "Window.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Scene.h"
#include "logger.h"
#include "iHandle.h"
#include "Event.h"
#include <string>
#include <vector>

namespace En3rN::DX
{
	class EnDX : unPtr(EnDX) 
	{
	public:
		virtual ~EnDX();
		static EnDX& Get();
		Window& GetWindow();
		bool NewWindow(int height = 800, int width = 600, bool fullscreen = false);
		virtual bool NewScene();
		virtual bool AddObjectToScene();
		bool LoadScene();
		bool Run();
		bool ProcessMsg();
		void UpdateScene(float dt);
		void DrawScene();
		
		
	private:
		EnDX() = default;
		Timer<float, sek>			timer;
		std::vector<Window::handle> windows;
		std::vector<Scene::handle>	scenes;
		
		uint8_t						activeWindow=0;
		static EnDX::handle			instance;
	};
}

