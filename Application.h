#pragma once
#include "Timer.h"
#include "Window.h"
#include "Renderer.h"
namespace En3rN::DX
{ 
	class Application
	{
	public:
		virtual bool OnCreate() = 0;
		virtual bool OnUpdate() = 0;

	protected:
		Application() = default;
		SekTimer			m_timer;
		Window				m_wnd;
		Renderer			m_renderer;
		Scene				m_scene;
		Entity::Container	m_entities;
		virtual~Application() = default;

	};
}

