#pragma once
#include "Entity.h"
#include "Helpers.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

namespace En3rN::DX {

	class MyGuI :private Entity
	{
	public:
		
		struct ImGuiConfig;
		struct WindowComponent;
		struct ComboParam;
		MyGuI( Entity&& e );
		MyGuI( Entity&& e , WindowComponent windowcomponent);
		MyGuI( MyGuI&& other ) noexcept = default;
		MyGuI& operator = ( MyGuI&& other ) noexcept = default;
		virtual ~MyGuI() = default;
		virtual bool OnCreate();
		bool AddMyGuIWindow( MyGuI&& myguiwindow );
		template<typename Component>
		bool ComponentControls( Component& component ) { return false; }
		virtual bool Draw();
	private:		
		entt::entity m_selectedEntity = entt::null;		//eller Entity
		std::vector<MyGuI> m_children;
	};
	// drawfunc, bool open, string name, imgui::windowflags, bool dockspace, imguidocknodeflags
	
	struct MyGuI::WindowComponent 
	{
		std::function<bool( entt::registry&, entt::entity& )> DrawFunc = 
			[]( entt::registry& registry, entt::entity& selectedEnnt){
			auto view = registry.view<TagComponent, UIDComponent>();
			for( auto [entity, tag, uid] : view.each() ) {
				ImGui::Text( Helpers::tag( uid.uId, tag.tag ).c_str());
			}
			return false; 
		};
		bool Open = true;
		std::string Name = "MyWindow";
		ImGuiWindowFlags Windowflags = ImGuiWindowFlags_None;
		bool DockSpace = true;
		ImGuiDockNodeFlags DockFlags = ImGuiDockNodeFlags_PassthruCentralNode;
	};

	struct MyGuI::ComboParam
	{
		std::string items;
		uint32_t	count{};
	};	
}

