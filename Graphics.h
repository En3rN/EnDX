#pragma once
#include "end3d11.h"
#include "iHandle.h"
#include "enexception.h"
#include "logger.h"
#include "Scene.h"
#include "InfoManager.h"
#include "Renderer.h"

namespace En3rN::DX
{
	class Keyboard;
	class Mouse;
	class Graphics: unPtr(Graphics)
	{
		friend class GfxResources;
	public:
		Graphics(HWND hWind, uint16_t with, uint16_t height, bool fullscreen);
		Renderer& GetRenderer() { return m_renderer; }
		void ClearState();
		void SetPresent(bool present);
		void SetFullscreen();
		void OnResize(int width, int height);
		void CreateBuffers(uint16_t width, uint16_t height);
		void LoadScene();
		void BeginFrame(float dt);
		/*void UpdateScene(float dt, Keyboard & kbd, Mouse & mouse);
		void DrawScene();*/
		void EndFrame();
		~Graphics()=default;
	private:
		D3D11_VIEWPORT viewport[8]{};
		InfoManager::handle infoManager;
		ComPtr<IDXGIFactory2> pIDXGIFactory;
		ComPtr<IDXGIAdapter> pAdapter;
		ComPtr<ID3D11Device> pDevice;
		ComPtr<ID3D11DeviceContext> pContext;
		ComPtr<IDXGISwapChain> pSwapChain;
		ComPtr<ID3D11DepthStencilState> pDepthStensilState;
		ComPtr<ID3D11DepthStencilView> pDepthStencilView;
		ComPtr<ID3D11RenderTargetView> pRenderTargetView;
		Renderer m_renderer;
		float angle = 0;
		bool show_demo_window = false;
		bool fullscreen;
		bool present = true;
		//bool showlogger = true;
	};

}