#pragma once
#include "enWin.h"
#include "iHandle.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Timer.h"
#include "RecT.h"
#include "Event.h"
#include <DirectXMath.h>
#include <string>

namespace En3rN::DX
{	
	class Window : shPtr(Window) , public EventListener
	{
	public:
		Window(uint16_t with, uint16_t height, bool fullscreen = false);
		~Window();
		static HWND GetHWND();
		static EventHandler& GetEventHandler();
		Graphics& GetGfx();
		Keyboard& GetKbd();
		Mouse& GetMouse();
		bool ProcessMsg();
		bool Run();
		void HideCursor();
		void ShowCursor();
		void EnableCursor();
		void DisableCursor();
		void ConfineCursor();
		void FreeCursor();
		RecT static GetRect();
		float static GetAspectRatio();
		static RecT GetSceenSize();
		virtual bool OnEvent(Event& e) override;
		LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK SetupProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK RedirectProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	private:		
		HWND				hWnd;
		uint16_t			WindowWidth;
		uint16_t			WindowHeight;
		EventHandler		eventHandler;
		Graphics::handle	pGfx;
		Keyboard			kbd;
		Mouse				mouse;
		std::string			appName;
		std::string			wName;
		std::vector<BYTE>	rawBuffer;
		static HINSTANCE	hInstance;
		static uint8_t		wCount;
		bool				showMessage = true;
		bool				init = false;
		bool				cursorEnabled = true;
	};
}

