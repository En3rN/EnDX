#include "MyGui.h"
namespace En3rN::DX {
	MyGuI::MyGuI( Entity&& e ):Entity(std::move(e)) {
		AddComponent<WindowComponent>( WindowComponent{} );
		OnCreate();
	}
	MyGuI::MyGuI( Entity&& entity, WindowComponent windowcomponent ) : Entity(std::move(entity)){
		AddComponent<WindowComponent>( std::move(windowcomponent) );		
		OnCreate();
	}

	bool MyGuI::OnCreate() {
		return false;
	}

	bool MyGuI::AddMyGuIWindow( MyGuI&& myguiwindow ) {
		m_children.emplace_back( std::move(myguiwindow));
		return true;
	}

	bool MyGuI::Draw() {		
		
		auto& wc = GetComponent<WindowComponent>();
		if( wc.DockSpace )
			ImGui::DockSpaceOverViewport( ImGui::GetMainViewport(), wc.DockFlags );
		ImGui::ShowDemoWindow(); 
		ImGui::Begin( wc.Name.c_str(), &wc.Open, wc.Windowflags );
		wc.DrawFunc( *m_handle.registry(), m_selectedEntity );
		ImGui::End();
		for( auto& child : m_children ) {
			auto& wc = child.GetComponent<WindowComponent>();
			ImGui::Begin( wc.Name.c_str(), &wc.Open, wc.Windowflags );
			wc.DrawFunc( *m_handle.registry(), m_selectedEntity );
			ImGui::End();
		}
		return true;
	}

}