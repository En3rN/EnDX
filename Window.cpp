#include "Window.h"
#include "enString.h"
#include "EnDX.h"
#include "WindowsMessageMap.h"
#include "enexception.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace En3rN::DX
{
	uint8_t Window::wCount = 0;
	HINSTANCE Window::hInstance = 0;
	
	Window::Window(uint16_t with, uint16_t height, bool fullscreen): WindowWidth(with), WindowHeight(height){
		wCount++;
		appName = "EnDX Engine";
		wName = "EnDX Engine";		
		
		if (wCount == 1){
			hInstance = GetModuleHandle(nullptr);
			WNDCLASSEX wcex;
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_OWNDC;
			wcex.lpfnWndProc = SetupProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = hInstance;
			wcex.hIcon = nullptr;
			wcex.hCursor = nullptr;
			wcex.hbrBackground = nullptr;
			wcex.lpszMenuName = nullptr;
			wcex.lpszClassName = appName.c_str();
			wcex.hIconSm = nullptr;
			ATOM atom = RegisterClassEx(&wcex);
		}

		RECT wr;
		wr.left = 350;
		wr.right = WindowWidth + wr.left;
		wr.top = 100;
		wr.bottom = WindowHeight + wr.top;

		AdjustWindowRect(&wr, WS_CAPTION | WS_SYSMENU | WS_OVERLAPPED, FALSE);
		hWnd = CreateWindow(appName.c_str(), wName.c_str(),
			WS_CAPTION | WS_SYSMENU | WS_OVERLAPPED,
			wr.left, wr.top, wr.right - wr.left, wr.bottom - wr.top,
			nullptr, nullptr, hInstance, this);
		if (!hWnd) {
			OutputDebugString("Window Creation Failed");
			throw EnExHR((HRESULT)GetLastError(), EnExParam);
			return;
		}
		else {
			BOOL sWnd = ShowWindow(hWnd, SW_SHOW);
		}
		pGfx = std::make_unique<Graphics>(hWnd,WindowWidth,WindowHeight, fullscreen);
		RAWINPUTDEVICE rid;
		rid.usUsagePage = 0x01; // mouse page
		rid.usUsage = 0x02; // mouse usage
		rid.dwFlags = 0;
		rid.hwndTarget = nullptr;
		if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
		{
			throw EnExcept("FAILED to register RawMouse",EnExParam);
		}
	}
	Window::~Window(){
		Logger::SetLevel(Logger::Level::Off);
		wCount--;
		OutputDebugString("Window Destroyed");
		DestroyWindow(hWnd);
		if(wCount<1) UnregisterClass(appName.c_str(), GetModuleHandle(nullptr));
	}
	HWND Window::GetHWND(){
		return EnDX::Get().GetWindow().hWnd;
	}
	EventHandler& Window::GetEventHandler()
	{
		return EnDX::Get().GetWindow().eventHandler;
	}
	Graphics& Window::GetGfx(){
		return *pGfx.get();
	}
	Keyboard& Window::GetKbd(){
		return kbd;
	}
	Mouse& Window::GetMouse(){
		return mouse;
	}
	bool Window::ProcessMsg(){
		MSG msg;
		if (PeekMessage(&msg, nullptr, NULL, NULL, PM_REMOVE) > 0)
		{
			if (msg.message == WM_QUIT) return false;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return true;
	}
	bool Window::Run(){
		//	// LoadScene
		////hgfx->LoadCube();
		//pGfx->LoadScene();
		//while (ProcessMsg()){			
		//	pGfx->BeginFrame(timer.GetElapsed());
		//	//HandleInput
		//	//HandleLogic
		//	//UpdateScene
		//	pGfx->UpdateScene(timer.GetFrameTimer(), kbd, mouse);
		//	//hgfx->UpdateCube(timer.GetFrameTimer()); //(timer.GetFrameTimer(),kbd, mouse);
		//	//DrawFrame
		//	
		//	pGfx->DrawScene();

		//	pGfx->EndFrame();
		//}
		return false;
	}
	void Window::HideCursor() 
	{
		while (::ShowCursor(FALSE) >= 0);
	}
	void Window::ShowCursor()
	{
		while (::ShowCursor(TRUE) < 0);
	}
	void Window::EnableCursor()
	{
		cursorEnabled = true;
		ShowCursor();
		ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
		FreeCursor();
	}
	void Window::DisableCursor()
	{
		cursorEnabled = false;
		HideCursor();
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
		ConfineCursor();
	}
	void Window::ConfineCursor() 
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		MapWindowPoints(hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
		ClipCursor(&rect);
	}
	void Window::FreeCursor() 
	{
		ClipCursor(nullptr);
	}
	RecT Window::GetRect(){
		Window& wnd= EnDX::Get().GetWindow();
		return RecT { wnd.WindowHeight,wnd.WindowWidth };
	}
	float Window::GetAspectRatio(){
		Window& wnd = EnDX::Get().GetWindow();
		return ((float)wnd.WindowWidth / (float)wnd.WindowHeight);
	}
	RecT Window::GetSceenSize()
	{
		RecT rc;
		rc.right = ::GetSystemMetrics(SM_CXSCREEN);
		rc.bottom = ::GetSystemMetrics(SM_CYSCREEN);
		return rc;
		
	}
	bool Window::OnEvent(Event& e)
	{
		if (e.category == Event::Category::Window)
		{
			switch (e.type)
			{
			case Event::Type::RawCapture:
				if (cursorEnabled){
					DisableCursor();
					return true;
				}
				else{
					EnableCursor();
					return true;
				}
			case Event::Type::FullScreen:
				pGfx->SetFullscreen();
				pGfx->OnResize(WindowWidth, WindowHeight);
				pGfx->SetPresent(true);
				return true;
			default:
				return false;
			}
		}
		return false;
	}
	LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
		if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam)) return true;
		if (showMessage) {
			WindowsMessageMap msgmap;
			std::string msg = msgmap(message, wParam, lParam);
			//Logger::Debug(msg.c_str());
			//OutputDebugString(msg.c_str());
		}
		switch (message)
		{
		case WM_KILLFOCUS:
			kbd.Clear();
			mouse.ToggleRawCaptureMode();
			//EnableCursor();
			return 0;
		case WM_SIZE: {
			if (pGfx) {
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				if (pGfx)pGfx->OnResize(width, height);
				Logger::Debug("size");
			}
			break;
		}
		case WM_CLOSE:
			if (wCount < 2) PostQuitMessage(69);
			return 0;
			// keyboard
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			if (ImGui::GetIO().WantCaptureKeyboard) break;
			eventHandler.AddEvent(Event::Create(Event::Category::Keyboard, Event::Type::KeyDown, wParam, lParam));
			kbd.OnKeyPress((uint8_t)wParam);
			break;
		}
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			if (ImGui::GetIO().WantCaptureKeyboard) break;
			eventHandler.AddEvent(Event::Create(Event::Category::Keyboard, Event::Type::KeyUp, wParam, lParam));
			kbd.OnKeyRelease((uint8_t)wParam);
			break;
		}
			//mouse
		case WM_MOUSEMOVE:
		{
			eventHandler.AddEvent(Event::Create(Event::Category::Mouse, Event::Type::Move, wParam, lParam));
			POINTS pos = MAKEPOINTS(lParam);
			mouse.OnMove(pos.x, pos.y);
			break;
		}
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		{
			if (ImGui::GetIO().WantCaptureMouse == true) break;
			Event e(Event::Category::Mouse, Event::Type::KeyDown, wParam, lParam);
			mouse.OnPress((uint8_t)wParam);
			break;
		}
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		{
			if (ImGui::GetIO().WantCaptureMouse == true) break;
			Event e(Event::Category::Mouse, Event::Type::KeyUp, wParam, lParam);
			mouse.OnRelease((uint8_t)wParam);
			break;
		}
		case WM_STYLECHANGED:
			if (pGfx) {
				STYLESTRUCT* style = (STYLESTRUCT*)(lParam);
				/*if (wParam & GWL_EXSTYLE)
				{
					if (style->styleNew & 0x00000000) {
						pGfx->SetFullscreen(true);
						pGfx->OnResize(::GetSystemMetrics(SM_CXFULLSCREEN), ::GetSystemMetrics(SM_CXFULLSCREEN));
					}
					if (style->styleNew & WS_EX_OVERLAPPEDWINDOW)
					{
						pGfx->SetFullscreen(false);
						pGfx->OnResize(WindowWidth, WindowHeight);
					}
					break;
				}
				if (wParam & GWL_STYLE)
				{
					if (style->styleNew & 0x00000000) {
						pGfx->SetFullscreen(true);
						pGfx->OnResize(::GetSystemMetrics(SM_CXFULLSCREEN), ::GetSystemMetrics(SM_CXFULLSCREEN));
					}
					if (style->styleNew & WS_CAPTION | WS_SYSMENU | WS_OVERLAPPED)
					{
						pGfx->SetFullscreen(false);
						pGfx->OnResize(WindowWidth, WindowHeight);
					}
				}*/
				if (style->styleNew == style->styleOld)
				{
					OutputDebugString("Style\n");
					pGfx->SetPresent(false);
					eventHandler.AddEvent(Event::Create(Event::Category::Window, Event::Type::FullScreen, wParam, lParam));
				}
			}
			break;
		case WM_INPUT:
		{
			Timer t("WM_INPUT");
			if (!mouse.RawCapture()){return true;}
			UINT size{};
			// first get the size of the input data
			if (GetRawInputData(
				reinterpret_cast<HRAWINPUT>(lParam),
				RID_INPUT,
				nullptr,
				&size,
				sizeof(RAWINPUTHEADER)) == -1)
			{
				// bail msg processing if error
				break;
			}
			rawBuffer.resize(size);
			// read in the input data
			if (GetRawInputData(
				reinterpret_cast<HRAWINPUT>(lParam),
				RID_INPUT,
				rawBuffer.data(),
				&size,
				sizeof(RAWINPUTHEADER)) != size)
			{
				// bail msg processing if error
				break;
			}
			// process the raw input data
			auto& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
			if (ri.header.dwType == RIM_TYPEMOUSE &&
				(ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
			{
				mouse.OnRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
			}
			return true;
		}
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	LRESULT CALLBACK Window::SetupProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
		switch (message){
		case WM_CREATE:{
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			Window* pWindow = reinterpret_cast<Window*>(pCreate->lpCreateParams);
			if (pWindow == nullptr){
				OutputDebugString("Critical Error: Pointer to window container is null during WM_NCCREATE.");
				exit(-1);
			}
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Window::RedirectProc));
			return pWindow->WndProc(hWnd, message, wParam, lParam);
			}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		
	}
	LRESULT CALLBACK Window::RedirectProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		Window* const pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		return pWindow->WndProc(hWnd, message, wParam, lParam);
		
	}
}
