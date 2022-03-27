#pragma once
#include "end3d11.h"
#include "Model.h"
#include "Camera.h"
#include "Renderer.h"
#include "UIControlsI.h"
//#include "Component.h"
#include <vector>

namespace En3rN::DX
{
	class Window;
	class Graphics;
	class Keyboard;
	class Mouse;
	class Scene: unPtr(Scene)
	{
	public:
		//Scene() = default;
		~Scene() = default;
		Scene(Renderer * renderer);
		Scene(Scene && other) = default;
		void OnCreate(); // create entities in this function
		void OnSceneChange();
		void AddEntity(Entity&& entity);
		Entity& CreateEntity(std::string&& tag = "EmptyEntity"); //constructs and stores an entity, returns &
		void AddCamera(Camera camera);
		void LoadModel(Entity& entity);
		void AddModel(Model::handle model);
		void Bind(Renderer & renderer);
		void Update(float dt);
		void Draw();
		
		bool UIControls();

		
		template<typename Function, typename Tuple, size_t ... I>
		auto call(Function f, Tuple& t, std::index_sequence<I ...>)
		{
			return f(std::get<I>(t) ...);
		}
		
		template<typename ...Ts>
		void unpackTuple(std::tuple<Ts...>& tuple)
		{
			std::apply([&](Ts&... tupleArgs){
					UIComponents(tupleArgs ...);
				}, tuple
			);
		}

		template <typename T, typename ... Ts>
		void UIComponents(T& component, Ts& ... rest)
		{
			
			if (component)
			{
				static ImGuiColorEditFlags coloreditFlags = ImGuiColorEditFlags_DisplayRGB;
				if constexpr(std::is_same_v<AmbientLightComponent*, T>)
					ImGui::ColorEdit3("Ambientlight color", &component->Ambient.x, coloreditFlags);
				if constexpr(std::is_same_v<DirectionalLightComponent*, T>)
					ImGui::ColorEdit3("Directionallight Color", &component->Color.x, coloreditFlags);
				if constexpr(std::is_same_v<PointLightComponent*, T>) {
					ImGui::ColorEdit3("Pointlight Color", &component->Color.x, coloreditFlags);
					ImGui::DragFloat3("Attenuation", &component->Attenuation.x);
				}
				if constexpr(std::is_same_v<SpotLightComponent*, T>) {
					ImGui::ColorEdit3("Spotlight Color", &component->Color.x, coloreditFlags);
					ImGui::DragFloat3("Attenuation", &component->Attenuation.x);
					ImGui::SliderFloat("ConeInner", &component->ConeInner, 0, 1);
					ImGui::SliderFloat("ConeOuter", &component->ConeOuter, 0, 1);
				}
				if constexpr(std::is_same_v<TransformComponent*, T>) {
					UIControls(*component);
				}
				if constexpr(std::is_same_v<Transform*, T>) {
					component->UIControls();
				}
				if constexpr(std::is_same_v<ModelRendererComponent*, T>) {
					UIControls(*component);
				}
			}
			if constexpr (sizeof ...(rest) > 0)
				UIComponents((rest)...);
			return;
		}
		template <class T>
		auto UIControls(T& component)
		{
			return false;
		}
		auto UIControls(ModelRendererComponent& mrc)
		{
			auto change = false;
			static std::string outPut;
			for(auto i : mrc.Meshes) {
				auto& mesh = m_meshes[i];
				auto& material = m_materials[mesh.GetMaterialIndex()];
				outPut = "Mesh: " + mesh.GetName() + ":";
				ImGui::Text(outPut.c_str());
				ImGui::Text("Materials:");
				if(material.UIControls())
				{
					for(auto& teq : mesh.Teqniques())
						for(auto& step : teq.GetSteps())
							if(step.GetRenderPassSlot() == Pass::Name::Phong) {
								auto ps = BindableManager::Query<PixelShader>(
									step.GetPassName(), 
									material.GetShaderEntryPoint());
								step.AddBindable(ps);
								std::stringstream l;
								l << std::boolalpha << "HasBindable<Pixelshader>:" << step.HasBindable<PixelShader>();
								Logger::Debug(l.str().c_str());
							}
				}
				ImGui::Text("Teqniques:");
				for(auto& teq : mesh.Teqniques()) {
					change += ImGui::Checkbox(teq.GetName().c_str(), &teq.Active());
				}
			}
				//teq.UIControls(); // TODO uicontrols
			return change;
		}
		auto UIControls(TransformComponent& tc)
		{
			static bool change = false;
			change += ImGui::DragFloat3("Position", tc.Position,.1f);
			change += ImGui::DragFloat3("Angles", tc.Angles, .1f);
			change += ImGui::DragFloat3("Scale", tc.Scale, .1f);
			return change;
		}

	private:
		Entity CreateEntities(aiNode* node, Vec3f scale);
		entt::registry m_registry;
		Entity::Container m_entities;
		Mesh::Container m_meshes;
		Material::Container m_materials;
		Node::handle m_rootNode = std::make_unique<Node>("MyScene");
		Camera::Container cameras;
		//Light lights;
		Model::Container models;
		
		Renderer* m_renderer;
	};
}

