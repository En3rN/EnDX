#include "Probe.h"
#include "Entity.h"
#include "Helpers.h"
#include "GfxResources.h"



bool En3rN::DX::EntityTreeProbe::Visit( Entity& entity ) {

    ImGui::PushID( entity.GetUid() );
    ImGuiTreeNodeFlags flags{};
    auto& io = ImGui::GetIO();
    if( selected == entity ) {
        flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected;
        pEntity = &entity;
    }
    bool open = false;
    if( entity.GetChilds().empty() )
        flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Leaf;
    open = ImGui::TreeNodeEx( Helpers::tag(entity.GetUid(),entity.GetName()).c_str(), flags);
    if( ImGui::IsItemClicked() ) {
        selected = entity;
        pEntity = &entity;
        std::string debug = entity.GetName() + "\n";
        Logger::Debug( debug.c_str() );
    }
    if( open ) {
        for( auto& child : entity.GetChilds() )
            Visit( child );
        ImGui::TreePop();
    }
    ImGui::PopID();    
    return open;
}
