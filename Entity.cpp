#include "Entity.h"
#include "Component.h"
#include "logger.h"
#include "RandomFactory.h"


namespace En3rN::DX
{
	//ennt ecample

    //void update(entt::registry& registry) {
    //    auto view = registry.view<const position, velocity>();

    //    // use a callback
    //    view.each([](const auto& pos, auto& vel) { /* ... */ });

    //    // use an extended callback
    //    view.each([](const auto entity, const auto& pos, auto& vel) { /* ... */ });

    //    // use a range-for
    //    for(auto [entity, pos, vel] : view.each()) {
    //        // ...
    //    }

    //    // use forward iterators and get only the components of interest
    //    for(auto entity : view) {
    //        auto& vel = view.get<velocity>(entity);
    //        // ...
    //    }
    //}

    //int main() {
    //    entt::registry registry;

    //    for(auto i = 0u; i < 10u; ++i) {
    //        const auto entity = registry.create();
    //        registry.emplace<position>(entity, i * 1.f, i * 1.f);
    //        if(i % 2 == 0) { registry.emplace<velocity>(entity, i * .1f, i * .1f); }
    //    }

    //    update(registry);
    //}


    Entity::Entity(entt::registry& registry, std::string&& name) :
        m_handle (registry, registry.create())
    {
        m_handle.emplace<UIDComponent>(GenerateUID());
        m_handle.emplace<TagComponent>(std::move(name));
    }

    const std::string& Entity::GetName() const
    {
        { return m_handle.get<TagComponent>().tag; }
    }

    void Entity::SetName(std::string&& name)
	{
		m_handle.get<TagComponent>().tag = std::move(name);
	}

    void Entity::AddChild(Entity&& entity)
    {
        m_children.push_back(std::move(entity));
    }

    entt::entity Entity::UISelector(entt::entity selected)
    {        
        ImGuiTreeNodeFlags flags{};
        if(selected == m_handle.entity())
            flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected;
        if(m_children.empty())
            flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Leaf;
        ui_open = ImGui::TreeNodeEx(GetName().c_str(), flags);
        if(ImGui::IsItemClicked()) {            
            selected = m_handle.entity();
            std::string debug = GetName() + "\n";
            Logger::Debug(debug.c_str());
        }
        if(ui_open) {
            for(auto& entity : m_children)
                selected = entity.UISelector(selected);
            ImGui::TreePop();
        }
        return selected;
    }

    auto Entity::UIControls()
    {
        auto [tag, uid, transform] = m_handle.try_get<TagComponent,UIDComponent, Transform>();

        return;
    }

    size_t Entity::GenerateUID()
	{
		return RandomFactory().UID();
	}  

};