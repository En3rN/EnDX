#include "Entity.h"
#include "Component.h"
#include "logger.h"
#include "RandomFactory.h"
#include "Transform.h"
#include <assimp\scene.h>


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
    Entity::Entity(entt::registry& registry, 
        aiNode* node, 
        const Teqnique::Container& teqniques, 
        const uint32_t startslotMeshes) :
        m_handle (registry, registry.create())
    {
        AddComponent<UIDComponent>(GenerateUID());
        AddComponent<TagComponent>(node->mName.C_Str());
        auto transform = TransformComponent();
        auto teqs = teqniques;
        if(!node->mTransformation.IsIdentity()) {
            for (auto& teq :teqs){
                for(auto& step : teq.GetSteps()) {
                    transform.Transform = DirectX::XMLoadFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&node->mTransformation));
                    //TODO skikkelig matrix decompose;
                    step.AddBindable(std::make_shared<Transform::ConstantBuffer>(transform.Transform, 1u, 1u,std::to_string(GetComponent<UIDComponent>().uId)));
                }
            }
        }
        AddComponent<TransformComponent>(std::move(transform));
        auto mrc = ModelRendererComponent();
        for(auto i = 0; i < node->mNumMeshes; ++i){
            mrc.MeshIndecies.push_back((node->mMeshes[i] + startslotMeshes));
            auto& mesh = Scene::Meshes[mrc.MeshIndecies.back()];
            for(auto teq : teqniques)
                mesh.AddTeqnique(teq, Scene::Materials);
        };
            AddComponent<ModelRendererComponent>(std::move(mrc));
        for(auto i = 0; i < node->mNumChildren; ++i) {
            m_children.push_back(Entity(registry, node->mChildren[i],teqniques, startslotMeshes));
        }
        
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

    Entity::Container& Entity::GetChilds()
    {
        return m_children;
    }

    const Entity::Container& Entity::GetChilds() const
    {
        return m_children;
    }
    

    entt::entity Entity::UISelector(entt::entity selected)
    {
        const auto& uid = GetComponent<UIDComponent>().uId;
        bool open = false;
        ImGui::PushID( uid );
        ImGuiTreeNodeFlags flags{};
        if(selected == *this)
            flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected;
        if(m_children.empty())
            flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Leaf;
        open = ImGui::TreeNodeEx(GetName().c_str(), flags);
        if(ImGui::IsItemClicked()) {            
            selected = m_handle.entity();
            std::string debug = GetName() + "\n";
            Logger::Debug(debug.c_str());
        }
        if(open) {
            for(auto& entity : m_children)
                selected = entity.UISelector(selected);
            ImGui::TreePop();
        }
        ImGui::PopID();
        return selected;
    }
    void Entity::UpdateConstantBuffer(const Transform::Matrix& mat)
    {
        auto& mrc = GetComponent<ModelRendererComponent>();
        for(auto& meshIndex : mrc.MeshIndecies) {
            auto& mesh = Scene::Meshes[meshIndex];
            for(auto& teq : mesh.Teqniques())
                for(auto& step : teq.GetSteps()) {
                    auto& context = teq.GetContext();
                    auto cbuf = step.GetBindable<Transform::ConstantBuffer>();                   
                    if(cbuf)
                        cbuf->Update(mat);
                    else {
                        cbuf = std::make_shared<Transform::ConstantBuffer>(mat,1u, 1u);
                        step.AddBindable(cbuf);
                    }
                }
        };
    }
    void Entity::SetParentTransform(const Transform::Matrix& mat)
    {
        for(auto& child : m_children){
            auto& tc = child.GetComponent<TransformComponent>();
            tc.ParentTransform = mat;
            child.OnUpdate<TransformComponent>();
        }
    }

    size_t Entity::GenerateUID()
	{
		return RandomFactory().UID();
	}
};