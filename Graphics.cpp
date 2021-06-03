#include "enWin.h"
#include "Graphics.h"
#include "enexception.h"
#include "vec.h"
#include "Vertex.h"
#include "logger.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "Cube.h"
#include <array>
#include <d3dcompiler.h>
#include <vector>
#include <cmath>
#include <complex>
#include <DirectXMath.h>
#include "Plane.h"
#include "InfoManager.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "DXGI.lib")


namespace En3rN::DX
{	
	
	Graphics::Graphics(HWND hWind, uint16_t width, uint16_t height, bool fullscreen): fullscreen(fullscreen)
	{
		errchk::hres(CreateDXGIFactory1(__uuidof(IDXGIFactory1), &pIDXGIFactory), EnExParam);
		int i = 0;
		IDXGIAdapter* adapter = nullptr;
		std::vector<IDXGIAdapter*> adapters{};

		errchk::hres(pIDXGIFactory->EnumAdapters(i, &pAdapter), pInfoQ.Get(), EnExParam);
		
		DXGI_ADAPTER_DESC ades{};
		errchk::hres(pAdapter->GetDesc(&ades), pInfoQ.Get(), EnExParam);
		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // DEBUG
		D3D_FEATURE_LEVEL featLvl[]
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0
		};
		D3D_FEATURE_LEVEL aFeatLvl[1]{};
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = height;
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
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

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
			aFeatLvl,
			&pContext),
			EnExParam);

		CreateBuffers(width, height);
		pContext->RSSetViewports(1, &viewport);
		/*spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
		spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Data\\Fonts\\comic_sans_ms_16.spritefont");*/

		//// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigDockingWithShift = true;
		

		//// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		////ImGui::StyleColorsClassic();

		//// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(hWind);
		ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());
		infoManager = std::make_unique<InfoManager>(*pDevice.Get());
	}

	void Graphics::ClearState()
	{
		pContext->ClearState();
		pContext->Flush();
	}

	void Graphics::SetFullscreen(bool fullscreen){
		this->fullscreen = fullscreen;
		BOOL f = fullscreen ? TRUE : FALSE;
		pSwapChain->SetFullscreenState(f, nullptr);
	}

	void Graphics::OnResize(int width, int height){
		try {			
			/*RecT wnd = Window::GetRect();
			RECT wwind{};
			GetWindowRect(Window::GetHWND(), &wwind);*/
			/*if(pRenderTargetView.Get())pRenderTargetView.Reset();
			if(pDepthStencilView.Get())pDepthStencilView.Reset();*/
			DXGI_MODE_DESC mode{};
			mode.Width = width;
			mode.Height = height;			
			mode.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
			mode.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			mode.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			pSwapChain->ResizeTarget(&mode);
			errchk::hres(pSwapChain->ResizeBuffers(0,
				width,height,
				DXGI_FORMAT_UNKNOWN,
				DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH), 
				pInfoQ.Get(),
				EnExParam);
			CreateBuffers(width,height);
		}
		catch (EnExHRDXInfo e) {
			MessageBox(nullptr, e.what(), e.GetType(), MB_OK);
		}
	}
	void Graphics::CreateBuffers(uint16_t width, uint16_t height){
		
		// RenderTarget View
		ComPtr<ID3D11Texture2D> pBackBuffer;
		errchk::hres(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackBuffer), pInfoQ.Get(), EnExParam);
		errchk::hres(pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, &pRenderTargetView), pInfoQ.Get(), EnExParam);
		// DepthStencil View
		D3D11_DEPTH_STENCIL_VIEW_DESC dsVDesc{};
		dsVDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsVDesc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
		ComPtr<ID3D11Texture2D> pDepthBuf;
		D3D11_TEXTURE2D_DESC depthTexDesc{};
		depthTexDesc.Width = width;
		depthTexDesc.Height = height;
		depthTexDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.SampleDesc.Count = 1;
		depthTexDesc.SampleDesc.Quality = 0;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;
		errchk::hres(pDevice->CreateTexture2D(&depthTexDesc, nullptr, &pDepthBuf), pInfoQ.Get(), EnExParam);
		errchk::hres(pDevice->CreateDepthStencilView(pDepthBuf.Get(), &dsVDesc, &pDepthStencilView),pInfoQ.Get(), EnExParam);
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = width;
		viewport.Height = height;
		viewport.MaxDepth = 1;
		viewport.MinDepth = 0;
	}
	void Graphics::LoadScene()
	{
		
	};
	void Graphics::BeginFrame(float dt){
		InfoManager::Update();
		pContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());
		pContext->RSSetViewports(1, &viewport);
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
		Logger::Draw("Logger");
		float mod = sin(dt);
		struct Color 
		{
			float r;
			float g;
			float b;
			float a;
		};
		Color color = { 0 , 0, 0 ,0 };

		pContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 1);
		pContext->ClearRenderTargetView(pRenderTargetView.Get(), &color.r);
	}
	
	void Graphics::EndFrame(){
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		ImGui::EndFrame();
		ImGui::UpdatePlatformWindows();
		DXGI_PRESENT_PARAMETERS param{};

		//errchk::hres(pSwapChain->Present1(0, 0, &param), EnExParam);
		errchk::hres(pSwapChain->Present(0,0),EnExParam);
	}
	void Graphics::LoadCube()
	{
		struct VertexT
		{
			float x, y, z;
			uint8_t r, g, b, a;
		};
		VertexPosColor VertexBuf[] =
		{
			{ -0.5f, -0.5f, -0.5f, 255,   0,   0, 255 },
			{  0.5f, -0.5f, -0.5f,	 0, 255,   0, 255 },
			{ -0.5f,  0.5f, -0.5f,   0,   0, 255, 255 },
			{  0.5f,  0.5f, -0.5f, 255, 255, 000, 255 },
			{ -0.5f, -0.5f,  0.5f, 255, 000, 255, 255 },
			{  0.5f, -0.5f,  0.5f,   0, 255, 255, 255 },
			{ -0.5f,  0.5f,  0.5f,   0,   0,   0, 255 },
			{  0.5f,  0.5f,  0.5f, 255, 255, 255, 255 },
		};

		uint16_t Indecies[] =
		{
			0,2,1,2,3,1,
			1,3,5,3,7,5,
			2,6,3,3,6,7,
			4,5,7,4,7,6,
			0,4,2,2,4,6,
			0,1,4,1,5,4,
		};
		struct ConstantBufferT
		{
			DirectX::XMMATRIX transform;
		};

		const ConstantBufferT cb =
		{
			{
				DirectX::XMMatrixTranspose(
						DirectX::XMMatrixRotationZ(angle) *
						DirectX::XMMatrixRotationY(angle) *
						DirectX::XMMatrixTranslation(0,0,4) *
						DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 10.0)
				)
			}
		};

		ComPtr<ID3D11Buffer> pBuffer;
		D3D11_BUFFER_DESC pdesc;
		pdesc.ByteWidth = sizeof(VertexBuf);
		pdesc.Usage = D3D11_USAGE_DEFAULT;
		pdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		pdesc.CPUAccessFlags = 0;
		pdesc.MiscFlags = 0;
		pdesc.StructureByteStride = sizeof(VertexPosColor);
		D3D11_SUBRESOURCE_DATA subRes{ VertexBuf,0,0 };
		errchk::hres(pDevice->CreateBuffer(&pdesc, &subRes, &pBuffer), pInfoQ.Get(), EnExParam);

		ComPtr<ID3D11Buffer> pIndecies;
		pdesc = {};
		pdesc.ByteWidth = sizeof(Indecies);
		pdesc.Usage = D3D11_USAGE_DEFAULT;
		pdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		pdesc.CPUAccessFlags = 0;
		pdesc.MiscFlags = 0;
		pdesc.StructureByteStride = sizeof(uint16_t);
		subRes = { Indecies,0,0 };
		errchk::hres(pDevice->CreateBuffer(&pdesc, &subRes, &pIndecies), pInfoQ.Get(), EnExParam);


		pdesc = {};
		pdesc.ByteWidth = sizeof(cb);
		pdesc.Usage = D3D11_USAGE_DYNAMIC;
		pdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		pdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		pdesc.MiscFlags = 0;
		pdesc.StructureByteStride = 0;
		subRes = { &cb,0,0 };
		errchk::hres(pDevice->CreateBuffer(&pdesc, &subRes, &pCBuf), pInfoQ.Get(), EnExParam);


		ComPtr<ID3DBlob> blob;
		errchk::hres(D3DReadFileToBlob(L"C:/Users/ChristerAndre/Source/Repos/En3rN/EnDX/PixelShader.cso", &blob), pInfoQ.Get(), EnExParam);
		errchk::hres(pDevice->CreatePixelShader(
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			nullptr, &pPixelShader),
			__FILE__, __LINE__);
		errchk::hres(D3DReadFileToBlob(L"C:/Users/ChristerAndre/Source/Repos/En3rN/EnDX/VertexShader.cso", &blob), pInfoQ.Get(), EnExParam);
		ComPtr<ID3D11VertexShader> pVertexShader;
		errchk::hres(pDevice->CreateVertexShader(
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			nullptr, &pVertexShader),
			pInfoQ.Get(), EnExParam);
		pContext->VSSetShader(pVertexShader.Get(), 0, 0);
		pContext->PSSetShader(pPixelShader.Get(), 0, 0);
		pContext->VSSetConstantBuffers(0, 1, pCBuf.GetAddressOf());

		UINT stride = (UINT)sizeof(VertexPosColor);
		UINT offset = 0;
		D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		ComPtr<ID3D11InputLayout> pInpLayout;
		errchk::hres(pDevice->CreateInputLayout(
			ied, std::size(ied),
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			&pInpLayout),
			pInfoQ.Get(),
			EnExParam);

		pContext->IASetInputLayout(pInpLayout.Get());
		pContext->IASetVertexBuffers(0, 1, pBuffer.GetAddressOf(), &stride, &offset);
		pContext->IASetIndexBuffer(pIndecies.Get(), DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);
		pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	void Graphics::UpdateCube(float dt)
	{
		angle += dt;
		struct ConstantBuffer
		{
			DirectX::XMMATRIX transform;
		};

		const ConstantBuffer cb =
		{
			{
				DirectX::XMMatrixTranspose(
						DirectX::XMMatrixRotationZ(angle) *
						DirectX::XMMatrixRotationY(angle) *
						DirectX::XMMatrixTranslation(0,0,4) *
						DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 10.0)
				)
			}
		};
		D3D11_MAPPED_SUBRESOURCE msr{};
		errchk::hres(pContext->Map(pCBuf.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr), pInfoQ.Get(), EnExParam);
		memcpy(msr.pData, &cb, sizeof(cb));
		pContext->Unmap(pCBuf.Get(), 0u);

		/*pDevContext->UpdateSubresource(pCBuf.Get(), 0, 0, &cb, 0, 0);
		pDevContext->VSSetConstantBuffers(0, 1, pCBuf.GetAddressOf());*/
	}
	
} // nameSpace
