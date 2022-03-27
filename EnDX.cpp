#include "EnDX.h"
#include "Graphics.h"
#include "Plane.h"
#include "Cube.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "Skybox.h"
#include "teapot.h"
#include "RandomFactory.h"
#include "RenderTarget.h"

namespace En3rN::DX
{
    EnDX::handle EnDX::instance(new EnDX());

    bool EnDX::Run()
    {
        assert(!windows.empty());
        Window& wnd = GetWindow();
        Event::Handler& eventHandler = wnd.GetEventHandler();
        Graphics& gfx = wnd.GetGfx();
        Keyboard& kbd = wnd.GetKbd();
        Mouse& mouse = wnd.GetMouse();
        wnd.Register();
        mouse.Register();
        RenderTarget rtv(256, 256);
        DepthStencil dsv(256, 256);
        rtv.Set(&dsv);
        LoadScene();
        while (ProcessMsg())
        {
            auto deltaTime = timer.GetFrameTimer();
            gfx.BeginFrame(timer.GetElapsed());
            //HandleInput
            eventHandler.ProcessEvents();
            Camera::GetActiveCamera().OnRawCapture(deltaTime, kbd, mouse);
            //HandleLogic
            //UpdateScene
            UpdateScene(deltaTime);
            //DrawFrame
            DrawScene();
            gfx.EndFrame();
        }
    return false;
    }
    bool EnDX::ProcessMsg()
    {
        MSG msg{};
        while (PeekMessage(&msg, nullptr, NULL, NULL, PM_REMOVE) > 0)
        {
            if (msg.message == WM_QUIT) 
                return false;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return true;
    }
    EnDX::~EnDX()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }
    EnDX& EnDX::Get(){
        return *instance.get();
    }    
    Window& EnDX::GetWindow()
    {
        return *windows.at(activeWindow).get();
    }
    
    bool EnDX::NewWindow(int height, int width, bool fullscreen){  
        activeWindow = (uint8_t)windows.size();
        windows.emplace_back(std::make_shared<Window>(height, width, fullscreen));
        return true;
    }
    bool EnDX::NewScene()
    {
        scenes.emplace_back(std::make_unique<Scene>(&GetWindow().GetGfx().GetRenderer()));
        return true;
    }
    bool EnDX::AddObjectToScene()
    {
        return false;
    }
    bool EnDX::LoadScene()
    {
        scenes.emplace_back(std::make_unique<Scene>(&GetWindow().GetGfx().GetRenderer()));
        auto& scene = *scenes.back().get();
        RandomFactory factory;
       
        /*for (int i = 0; i < 100; ++i)
        {
            scene.AddDrawable(factory());
        };*/
        //scene.AddModel(Model::LoadPrimitive());
        //scene.AddModel(Model::LoadModel("cube.obj"));
        //scene.AddModel(Model::LoadModel("/primitive/sphere.obj"));
        //scene.AddModel(Model::LoadModel("/nanosuit/nanosuit.obj", {.2f,.2f,.2f}));
        //scene.AddModel(Model::LoadModel("/muro/muro.obj"));
        //scene.AddModel(Model::LoadModel("/gobber/gobber.obj"));
        //scene.AddModel(Model::LoadModel("/sponza/sponza.obj", {0.05f,0.05f,0.05f}));
        //scene.AddDrawable(std::make_unique<Skybox>("0-desert-skybox.png"));
        
        
        return false;
    }
    void EnDX::UpdateScene(float dt)
    {
        for (auto& scene : scenes)
            scene.get()->Update(dt);
    }

    void EnDX::DrawScene()
    {
        for (auto& scene : scenes)
            scene.get()->Draw();        
    }
}