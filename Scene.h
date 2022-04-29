#pragma once
#include "end3d11.h"
#include "Model.h"
#include "Camera.h"
#include "Renderer.h"
#include "UIControlsI.h"
#include "Helpers.h"
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
		Entity& GetEntity(entt::entity entt);
		Mesh::Index FindMesh(const std::string & name);
		Material::Index FindMaterial(const std::string & name);
		void AddCamera(Camera camera);
		void LoadModel(Entity& entity);
		void AddModel(Model::handle model);
		void Bind(Renderer & renderer);
		void Update(float dt);
		void Draw();
		
		bool UIControls();
		
		template <typename T, typename ... Ts>
		bool UIComponents(T& component, Ts& ... rest)
		{
			bool change = false;
			
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
				UIControls( *component );
			}
			if constexpr (sizeof ...(rest) > 0)
				UIComponents((rest)...);
			return change;
		}
		template <class T>
		bool UIControls(T& component){
			return false;
		}
		bool UIControls(ModelRendererComponent& mrc)
		{
			/*auto tag = []( auto meshid, auto name ) {
				std::string s = "##" + std::to_string( meshid ) + name;
				return s;
			};*/
			auto change = false;
			static std::string outPut;
			for(auto& meshIndex : mrc.MeshIndecies) {
				int i = 0;
				auto& mesh = Meshes[meshIndex];
				auto& material = Materials[mesh.GetMaterialIndex()];
				auto& teqniques = mesh.Teqniques();
				outPut = "Mesh: " + mesh.GetName() + ":";
				ImGui::Text(outPut.c_str());
				ImGui::Text("Materials:");
				if(material.UIControls())
				{
					for(auto& teq : teqniques)
						for(auto& step : teq.GetSteps())
							if(step.GetPassName() == "Phong") {
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
				for(auto& teq : teqniques) {
					if( ImGui::Checkbox(Helpers::tag(meshIndex,teq.GetName()).c_str(), &teq.Active()) ) {
						
						change = true;
						std::stringstream l;
						l << mesh.GetName() << ' ' << teq.GetName();
						Logger::Debug( l.str().c_str() );
					}
				}
				/*if( ImGui::Button( "AddTeqnique" ) ) {
					ImGui::BeginChild( "TeqniqueSelector" );
					auto teqIndex= ImGui::Combo( "Select", 1, teqniqueCombo.items.c_str() ) ) {
						std::string teqName;
						auto teq = Teqnique::Teqniques.begin();
							mesh.AddTeqnique( Teqnique::Teqniques[ teqName ](),Scene::Materials);
					}
					ImGui::EndChild();
				}*/
			}
			if(mrc.MeshIndecies.empty() )
				for( auto& teq : mrc.Teqniques ) {
					change += ImGui::Checkbox( teq.GetName().c_str(), &teq.Active() );
					if( change ) {

					}
				}
					
				
			return change;
		}
		bool UIControls(TransformComponent& tc)
		{
			bool change = false;
			change += ImGui::DragFloat3("Position", tc.Position,.1f);
			change += ImGui::DragFloat3("Angles", tc.Angles, .1f);
			change += ImGui::DragFloat3("Scale", tc.Scale, .1f);
			return change;
		}

		static Mesh::Container Meshes;
		static Material::Container Materials;

	private:
		entt::registry m_registry;
		Entity::Container m_entities;		
		Node::handle m_rootNode = std::make_unique<Node>("MyScene");
		Camera::Container cameras;
		Model::Container models;
		Renderer* m_renderer;
	};
}

