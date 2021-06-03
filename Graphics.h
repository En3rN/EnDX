#pragma once
#include "end3d11.h"
#include "iHandle.h"
#include "enexception.h"
#include "logger.h"
#include "Scene.h"
#include "InfoManager.h"

namespace En3rN::DX
{
	class Keyboard;
	class Mouse;
	class Graphics: unPtr(Graphics)
	{
		friend class GfxResources;
	public:
		Graphics(HWND hWind, uint16_t with, uint16_t height, bool fullscreen);
		void ClearState();
		void SetFullscreen(bool fullscreen);
		void OnResize(int width, int height);
		void CreateBuffers(uint16_t width, uint16_t height);
		void LoadCube();
		void UpdateCube(float dt);
		void LoadScene();
		void BeginFrame(float dt);
		/*void UpdateScene(float dt, Keyboard & kbd, Mouse & mouse);
		void DrawScene();*/
		void EndFrame();
		~Graphics()=default;
	private:
		D3D11_VIEWPORT viewport{};
		InfoManager::handle infoManager;
		ComPtr<IDXGIFactory> pIDXGIFactory;
		ComPtr<IDXGIAdapter> pAdapter;
		ComPtr<ID3D11Device> pDevice;
		ComPtr<ID3D11DeviceContext> pContext;
		ComPtr<IDXGISwapChain> pSwapChain;
		ComPtr<ID3D11DepthStencilState> pDepthStensilState;
		ComPtr<ID3D11DepthStencilView> pDepthStencilView;
		ComPtr<ID3D11RenderTargetView> pRenderTargetView;
		ComPtr<ID3D11BlendState> pBlendState;
		ComPtr<ID3D11InfoQueue> pInfoQ;
		ComPtr<ID3D11PixelShader> pPixelShader;
		ComPtr<ID3D11Buffer> pCBuf;
		float angle = 0;
		bool show_demo_window = true;
		bool fullscreen;
		//bool showlogger = true;
	};

}