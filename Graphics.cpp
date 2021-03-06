#include "enWin.h"
#include "Graphics.h"
#include "enexception.h"
#include "vec.h"
#include "Vertex.h"
#include "logger.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "Cube.h"
#include "RenderTarget.h"
#include <array>
#include <d3dcompiler.h>
#include <vector>
#include <cmath>
#include <complex>
#include <DirectXMath.h>
#include "Plane.h"
#include "InfoManager.h"
#include "RenderTarget.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "DXGI.lib")


namespace En3rN::DX
{	
	
	Graphics::Graphics(HWND hWind, uint16_t width, uint16_t height, bool fullscreen): 
		fullscreen(fullscreen)
	{
		errchk::hres(CreateDXGIFactory1(IID_IDXGIFactory2, &pDXGIFactory));
		int i = 0;
		std::vector<ComPtr<IDXGIAdapter>> adapters{};

		for(auto i =0; pDXGIFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			adapters.push_back(pAdapter);
			DXGI_ADAPTER_DESC desc{};
			pAdapter->GetDesc(&desc);
			std::wstringstream ss{};
			ss << desc.Description << '\t' << desc.DedicatedVideoMemory << '\t' << desc.DeviceId << '\n\r';
			OutputDebugStringW(ss.str().c_str());
		}
		std::ranges::sort(adapters, 
			[](const ComPtr<IDXGIAdapter>& lhs, const ComPtr<IDXGIAdapter>& rhs){
				DXGI_ADAPTER_DESC desc[2]{};
				lhs->GetDesc(&desc[0]);
				rhs->GetDesc(&desc[1]);
				return desc[0].DedicatedVideoMemory > desc[1].DedicatedVideoMemory;
			}
		);
		pAdapter = adapters.at(0);
		
		DXGI_ADAPTER_DESC ades{};
		errchk::hres(pAdapter->GetDesc(&ades));
		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // DEBUG
		D3D_FEATURE_LEVEL featLvl[]
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0
		};
		D3D_FEATURE_LEVEL resultFeatLvl{};
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferDesc.Width = 0;
		swapChainDesc.BufferDesc.Height = 0;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.OutputWindow = hWind;
		swapChainDesc.Windowed = fullscreen ?  FALSE : TRUE;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		//swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		errchk::hres(D3D11CreateDeviceAndSwapChain(
			pAdapter.Get(),
			D3D_DRIVER_TYPE_UNKNOWN,
			nullptr,
			creationFlags,
			featLvl,
			(UINT)std::size(featLvl),
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&pSwapChain,
			&pDevice,
			&resultFeatLvl,
			&pContext));
		/*ComPtr<ID3D11Resource> backbuffer;
		pSwapChain->GetBuffer(0, IID_ID3D11Resource, &backbuffer);
		Resource res(backbuffer);
		RenderTarget rt(res);
		RenderPass renderpass("Phong", rt);
		m_renderer.AddPass(renderpass);*/

		//m? f? laget pass !!!! f?r ikke laget her;

		infoManager = std::make_unique<InfoManager>(*pDevice.Get());

		CreateBuffers(width, height);
		//D3D11_VIEWPORT
		
		/*spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
		spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Data\\Fonts\\comic_sans_ms_16.spritefont");*/

		//// Setup Dear ImGui contexten
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigDockingWithShift = true;
		//// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		////ImGui::StyleColorsClassic();
		//// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(hWind);
		ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());
	}

	void Graphics::ClearState()
	{
		pContext->ClearState();
		//pContext->Flush();
	}

	void Graphics::SetPresent(bool present)
	{
		this->present = present;
	}

	void Graphics::SetFullscreen(){
		(fullscreen) ? fullscreen=false: fullscreen=true;		
		pSwapChain->SetFullscreenState(fullscreen, nullptr);
	}
	void Graphics::OnResize(int width, int height){
		try {			
			
			DXGI_MODE_DESC mode{};
			mode.Width = width;
			mode.Height = height;			
			mode.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
			mode.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			mode.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			pSwapChain->ResizeTarget(&mode);
			if(pRenderTargetView.Get()) errchk::hres(pRenderTargetView.Get()->Release());
			if(pDepthStencilView.Get()) errchk::hres(pDepthStencilView.Get()->Release());
			errchk::hres(pSwapChain->ResizeBuffers(0,
				width,height,
				DXGI_FORMAT_UNKNOWN,
				DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));
			CreateBuffers(width,height);
		}
		catch (EnExHR e) {
			MessageBox(nullptr, e.what(), e.GetType(), MB_OK);
		}
	}
	void Graphics::CreateBuffers(uint16_t width, uint16_t height){
		// RenderTarget View
		ComPtr<ID3D11Texture2D> pBackBuffer;
		errchk::hres(pSwapChain->GetBuffer(0, IID_ID3D11Texture2D, &pBackBuffer));
		D3D11_TEXTURE2D_DESC texDesc{};
		pBackBuffer->GetDesc(&texDesc);
		errchk::hres(pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, &pRenderTargetView));
		// DepthStencil View
		D3D11_DEPTH_STENCIL_VIEW_DESC dsVDesc{};
		dsVDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsVDesc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
		D3D11_TEXTURE2D_DESC backBufferDesc{};
		pBackBuffer->GetDesc(&backBufferDesc);
		ComPtr<ID3D11Texture2D> pDepthBuf;
		D3D11_TEXTURE2D_DESC depthTexDesc{};
		depthTexDesc.Width = backBufferDesc.Width;
		depthTexDesc.Height = backBufferDesc.Height;
		depthTexDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.SampleDesc.Count = 1;
		depthTexDesc.SampleDesc.Quality = 0;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;
		errchk::hres(pDevice->CreateTexture2D(&depthTexDesc, nullptr, &pDepthBuf));
		errchk::hres(pDevice->CreateDepthStencilView(pDepthBuf.Get(), &dsVDesc, pDepthStencilView.GetAddressOf()));
		viewport[0].TopLeftX = 0;
		viewport[0].TopLeftY = 0;
		viewport[0].Width = backBufferDesc.Width;
		viewport[0].Height = backBufferDesc.Height;
		viewport[0].MaxDepth = 1;
		viewport[0].MinDepth = 0;


		D3D11_RECT scissorRect[8]{};
		

		//pContext->RSSetScissorRects(8, scissorRect);
		pContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());
		pContext->RSSetViewports(8, viewport);
	}
	void Graphics::LoadScene()
	{
		
	};
	void Graphics::BeginFrame(float dt){
		if (!InfoManager::Empty())
			throw EnExDXInfo();
		pContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());
		pContext->RSSetViewports(8, viewport);
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		
		float mod = sin(dt);
		struct Color 
		{
			float r;
			float g;
			float b;
			float a;
		};
		Color color = {mod ,mod, 1 ,1};
		pContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		pContext->ClearRenderTargetView(pRenderTargetView.Get(), &color.r);
	}

	Resource Graphics::GetBackBuffer()
	{
		ComPtr<ID3D11Resource> resource;
		pSwapChain->GetBuffer(0,IID_ID3D11Resource,&resource);
		return Resource(resource);
	}
	
	void Graphics::EndFrame(){
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		ImGui::EndFrame();
		ImGui::UpdatePlatformWindows();
		if (present)errchk::hres(pSwapChain->Present(0, 0));
	}
} // nameSpace
