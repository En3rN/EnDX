#include "Scene.h"
#include "Graphics.h"
#include "Entity.h"
#include "vec.h"
#include "logger.h"
#include "enMath.h"
#include "Teqnique.h"
#include "Transform.h"
#include "Sampler.h"
#include "Rasterizer.h"
#include "Blend.h"
#include "Stencil.h"
#include "enexception.h"
#include "Component.h"
#include "TransformConstantBufferScaling.h"

#include <filesystem>
#include <memory>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <assimp\version.h>

namespace En3rN::DX
{
	Mesh::Container Scene::Meshes{};
	Material::Container Scene::Materials{};

	En3rN::DX::Scene::Scene(Renderer* renderer) :
		m_renderer(renderer)
		//lights(m_registry)
	{
		AddCamera({ 0, 0, -4, 10, 0.5f, 1000 });
		AddCamera(Camera(Transform({ 0,0,4 }, { 0,PI,0 })));
		//Light::Data ld{};
		//ld.Ambient = { 0.1f,0.1f,0.1f };
		//ld.Light->Color = { 1,1,1,1 };
		////ld.Light->Direction = { -.25f,.5f,-.25f };
		//ld.Light->IsDirectional = false;
		//ld.Light->Cone = 1;
		//AddLight(ld);
		OnCreate();
		/*auto onTransFormConstruct = m_registry.on_construct<TransformComponent>();
		auto onUpdateTransform = m_registry.on_update<TransformComponent>();
		
		
		onTransFormConstruct.connect<&Transform::Listener>(onTransFormConstruct);*/
		auto view = m_registry.view<ModelRendererComponent, TransformComponent>();
		for(auto [entt, mrc, transform] : view.each()){
			transform.Transform = Transform::GetMatrix(transform.Position,transform.Angles,transform.Scale);
			auto entity = std::find(begin(m_entities), end(m_entities), entt);
			LoadModel(*entity);			
		}
	}

	// create entities here
	void En3rN::DX::Scene::OnCreate()
	{
		Material::Data mData;
		mData.diffuse = { 1,0,0,1 };

		Teqnique Phong("Phong", true, Step({ "Phong" }));
		Teqnique Unlit("Unlit", true, Step({ "Unlit" }));
		Teqnique Debug("Debug", true, Step({ "Unlit" }, std::make_shared<PixelShader>("Phong", "debug")));
		Teqnique Outline("Outline", true,
			Step({ "MaskBackBuffer" },
				BindableManager::Query<NullPixelShader>(),
				BindableManager::Query<DepthStencilState>(DepthStencilState::Depth::Disable, DepthStencilState::Stencil::Write, 1)
			),
			Step({ "DrawOutline" , "Outline" },
				BindableManager::Query<PixelShader>("Unlit", "main"),
				std::make_shared<Material::ConstantBuffer>(mData, 1, 1, "outlineMat"),
				std::make_shared<TransformConstantBufferScaling>(1.05f)				
			),
			Step({ "FullScreen","DrawOutline" },
				BindableManager::Query<NullPixelShader>(),	//HACK --> To avoid mesh.Addbindable to insert shader based on pass and material
				BindableManager::Query<DepthStencilState>(DepthStencilState::Depth::Disable, DepthStencilState::Stencil::Read, 1),
				BindableManager::Query<Blender>(Blender::State::Enabled)
			));
		Outline.Context([](auto& context) {
			Vec3f color{ 1,0,0 };
			context.add_element(color, "Color");			
			});

		auto test = Teqnique::Teqniques.find( "Outline" );
		auto func = Teqnique::Teqniques[ "Outline" ];
		auto teq = func();
		
				

		auto& ambientDirectionPointlight = CreateEntity();
		TransformComponent tc;
		tc.Scale = { 0.1f,.1f,.1f };
		tc.Position = { 0,0,0 };
		//tc.Angles = { +1.6, 2, 0 };
		ambientDirectionPointlight.AddComponent<TransformComponent>(std::move(tc));
		ambientDirectionPointlight.AddComponent<DirectionalLightComponent>();
		ambientDirectionPointlight.AddComponent<PointLightComponent>();
		ambientDirectionPointlight.AddComponent<AmbientLightComponent>();
		ambientDirectionPointlight.SetName("Lights");
		auto mrc = ModelRendererComponent("primitive/sphere.obj",{Unlit, func()});
		ambientDirectionPointlight.AddComponent<ModelRendererComponent>(std::move(mrc));
		
		auto& gobber = CreateEntity("Gobber");
		tc = TransformComponent();
		tc.Scale = { .5f,.5f,.5f };
		gobber.AddComponent<TransformComponent>(std::move(tc));
		mrc = ModelRendererComponent("gobber/gobber.obj", {  Phong , Debug, func()});
		gobber.AddComponent<ModelRendererComponent>(std::move(mrc));
		
		auto& Pointlight2 = CreateEntity();
		tc = TransformComponent();
		tc.Scale = { 0.1f,.1f,.1f };
		tc.Position = { 10, 0, 20 };
		Pointlight2.AddComponent<TransformComponent>(std::move(tc));
		Pointlight2.AddComponent<PointLightComponent>();
		Pointlight2.SetName("Lights");
		mrc = ModelRendererComponent("primitive/sphere.obj",{ Unlit ,func()});
		Pointlight2.AddComponent<ModelRendererComponent>(std::move(mrc));

		auto& SpotLight = CreateEntity("SpotLight");
		tc = TransformComponent();
		tc.Scale = { 0.1f,.1f,.1f };
		tc.Position = { 10, 0, 5 };
		tc.Angles = { -1,0,0 };
		SpotLight.AddComponent<TransformComponent>(std::move(tc));
		SpotLight.AddComponent<SpotLightComponent>();
		mrc = ModelRendererComponent("primitive/sphere.obj",{ Unlit,func()});
		SpotLight.AddComponent<ModelRendererComponent>(std::move(mrc));

		auto& cubeBrick = CreateEntity();
		cubeBrick.SetName("BrickCube");
		tc=TransformComponent();
		tc.Position = { 0,0,10 };
		cubeBrick.AddComponent<TransformComponent>(std::move(tc));
		mrc = ModelRendererComponent("primitive/cube.obj", { Phong , Debug, func()});
		cubeBrick.AddComponent<ModelRendererComponent>(std::move(mrc));


		/*auto& cube = CreateEntity("BrickWall");
		tc = TransformComponent();
		tc.Position = { 0,0,5 };
		cube.AddComponent<TransformComponent>(std::move(tc));
		mrc = ModelRendererComponent("brick_wall/brick_wall.obj",{ Phong , Debug});
		cube.AddComponent<ModelRendererComponent>(std::move(mrc));*/

		/*auto& sphere = CreateEntity();
		sphere.SetName("Bricksphere");
		tc = TransformComponent();
		tc.Position = { 0,0,2 };
		sphere.AddComponent<TransformComponent>(std::move(tc));	
		
		mrc = ModelRendererComponent("primitive/sphere.obj", { Phong , Debug});
		sphere.AddComponent<ModelRendererComponent>(std::move(mrc));*/

		


		/*auto& sponsa = CreateEntity("Sponza");
		tc = TransformComponent();
		tc.Scale = { .01f,.01f,.01f };
		sponsa.AddComponent<TransformComponent>(std::move(tc));
		mrc = ModelRendererComponent("sponza/sponza.obj", {}, { Phong , Debug });
		sponsa.AddComponent<ModelRendererComponent>(std::move(mrc));

		auto& nano = CreateEntity("Nano");
		tc = TransformComponent();
		tc.Scale = { .5,.5,.5 };
		nano.AddComponent<TransformComponent>(std::move(tc));
		mrc = ModelRendererComponent("nanosuit/nanosuit.obj", {}, { Phong,Debug });
		nano.AddComponent<ModelRendererComponent>(std::move(mrc));*/


	}
	void Scene::OnSceneChange()
	{
		auto view = m_registry.view<Model>();
		/*for(auto [entity, model] : view.each())
		{
			m_rootNode->AddChild(model.GetRootNode());
		}*/
		view.each([&](auto& model) {
			m_rootNode->AddChild(model.GetRootNode());
			});
	}
	Entity& Scene::CreateEntity(std::string&& tag)
	{
		m_entities.emplace_back(Entity(m_registry, std::move(tag)));
		return m_entities.back();
	}

	Entity& Scene::GetEntity(entt::entity entt)
	{		
		for(auto& entity : m_entities){
			auto found = entity.FindChild(entt);
			if(found)
				return *found;
		}
		throw EnExcept("Entity Not Found");
	}

	Mesh::Index Scene::FindMesh(const std::string& name)
	{
		auto it = std::find_if(begin(Meshes), end(Meshes),
			[&](auto& mesh) {
				return mesh.GetName() == name;
			});
		if(it == std::end(Meshes)) return Meshes.size();
		return it - std::end(Meshes);
	}

	Material::Index Scene::FindMaterial(const std::string& name)
	{
		auto it = std::find_if(begin(Materials), end(Materials),
			[&](auto& mat) {
				return mat.GetName() == name;
			});
		if(it == std::end(Materials)) return Materials.size();
		return it - std::end(Materials);
		
	}


	void En3rN::DX::Scene::AddEntity(Entity&& entity)
	{
		m_entities.push_back(std::move(entity));
	}

	void En3rN::DX::Scene::AddCamera(Camera camera)
	{
		cameras.AddCamera(camera);
	}

	void Scene::LoadModel(Entity& entity)
	{
		auto [mrc, transform] = entity.GetComponents<ModelRendererComponent, TransformComponent>();
		Vec3f scale{};
		transform ? scale = transform->Scale : scale = { 1,1,1 };
		auto modelPath = mrc->Path.string();		
		std::filesystem::path path = Config::GetFolders().assets;
		path /= modelPath;		
		std::stringstream version;
		version << "Assimp version: " << "Major:" << aiGetVersionMajor() << " Minor:" << aiGetVersionMinor() << " Revision:" << aiGetVersionRevision() << std::endl;
		OutputDebugString(version.str().c_str());

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path.string(),
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_ConvertToLeftHanded |
			aiProcess_CalcTangentSpace |
			aiProcess_GenNormals
		);
		// If the import failed, report it
		if(!scene) {
			std::string err = "Failed to load model: ";
			err += importer.GetErrorString();
			throw EnExcept(err);
		}
		Logger::Debug(importer.GetErrorString());

		auto startSlotMeshes = Meshes.size();
		auto startSlotMaterials = Materials.size();
		for( auto i = 0; i < scene->mNumCameras; ++i ) {
			scene->mCameras[i];
		};
		for(auto i = 0; i < scene->mNumLights; ++i) {
		};
		for(auto i = 0; i < scene->mNumMaterials; ++i) {
			Materials.push_back(Material(scene->mMaterials[i], path.remove_filename()));
		};
		for(auto i = 0; i < scene->mNumMeshes; ++i) {
			Meshes.push_back(Mesh(scene->mMeshes[i], startSlotMaterials));
		};
		
		auto& teqniques = mrc->Teqniques;
		auto& meshIndecies = mrc->MeshIndecies;

		if(scene->mRootNode->mNumChildren > 1)
			for(auto i = 0; i < scene->mRootNode->mNumChildren; ++i)
				entity.AddChild(Entity(m_registry, scene->mRootNode->mChildren[i], teqniques, startSlotMeshes));
		else
			for(auto i = 0; i < scene->mRootNode->mNumChildren; ++i) {
				auto node = scene->mRootNode->mChildren[i];
				for(auto i = 0; i < node->mNumMeshes; ++i) {
					meshIndecies.push_back(node->mMeshes[i] + startSlotMeshes);
					auto& mesh = Meshes[meshIndecies.back()];
					for(auto teq : teqniques)
						mesh.AddTeqnique(teq, Materials);
				};
			}
		entity.OnUpdate<TransformComponent>();
	}

	/*void En3rN::DX::Scene::AddLight(Light light)
	{
		lights.push_back(light);
	}*/

	void En3rN::DX::Scene::AddModel(Model::handle model)
	{
		models.push_back(model);
	}

	void En3rN::DX::Scene::Bind(Renderer& renderer)
	{
	}

	void En3rN::DX::Scene::Update(float dt)
	{
		Camera::GetActiveCamera().SetConstantBuffer();
		static Light lights(m_registry);
		lights.OnUpdate();
		//Model modelsystem;		
		static entt::observer transformObserver(m_registry, entt::collector.update<TransformComponent>());
		static entt::observer mrcObserver(m_registry, entt::collector.update<ModelRendererComponent>());
		transformObserver.each([&](entt::entity entt) {
			auto& entity = GetEntity(entt);
			entity.OnUpdate<TransformComponent>();
			});
		mrcObserver.each([&](entt::entity entt) {
			auto& entity = GetEntity(entt);
			entity.OnUpdate<ModelRendererComponent>();
			});
		auto sink = m_registry.on_update<TransformComponent>();
		
		
		



		/*for(auto entt : transformObserver){
			auto& entity = GetEntity(entt);
			entity.OnUpdate<TransformComponent>();
		}
		transformObserver.clear();*/

		/*auto modelView = m_registry.view<ModelRendererComponent, TransformComponent>();
		for(auto [entt, mrc, transform] : modelView.each()) {
			transform.Transform = DirectX::XMMatrixIdentity() *
				DirectX::XMMatrixRotationRollPitchYaw(transform.Angles.x, transform.Angles.y, transform.Angles.z) *
				DirectX::XMMatrixTranslation(transform.Position.x, transform.Position.y, transform.Position.z) *
				DirectX::XMMatrixScaling(transform.Scale.x, transform.Scale.y, transform.Scale.z);
		}*/

		auto view = m_registry.view<Model::handle>();
		for(auto [entity, model] : view.each())
			model->Update(dt);

		for(auto& model : models)
			model->Update(dt);
	}

	void En3rN::DX::Scene::Draw()
	{
		UIControls();
		auto view = m_registry.view<ModelRendererComponent, TransformComponent>();
		for(auto [entt, mrc, transform] : view.each()) {
			for(auto& meshIndex : mrc.MeshIndecies){
				auto& mesh = Meshes[meshIndex];
				auto& material = Materials[mesh.GetMaterialIndex()];
				auto& teqniques = mesh.GetTeqniques();
				for(auto& teq :teqniques) {
					if(teq.IsActive())
						for(auto& step : teq.GetSteps()) {
							m_renderer->GetPass(step.GetPassName()).AddJob(
								RenderJob(mesh, material,step));
						}
				}
			}
		}
		
		for(auto& model : models)
			model->CreateJobs(*m_renderer, *model->GetRootNode().get());
		m_renderer->Draw();
	}

	bool En3rN::DX::Scene::UIControls()
	{
		struct ComboParam
		{
			std::string items;
			uint32_t	count{};
		};
		//windows
		static bool ImGuiOn = true;
		static bool dockSpace = true;
		static bool show_demo_window = true;
		static bool sceneControlWindow = true;
		static bool modelControlWindow = true;
		static bool loggerWindow = true;

		//scene window vars
		static Model* selectedModel = nullptr;
		//static int selectedModel;
		static int selectedLight;
		static int selectedCamera;
		static Node* selectedNode = m_rootNode.get();
		static ComboParam modelCombo;
		static ComboParam cameraCombo;
		static ComboParam lightCombo;
		static entt::entity selectedEntity;
		//static entt::handle selectedEntity;
		

		auto modelSelector = [&](Node* node) {

			auto view = m_registry.view<Model>();
			for(auto [entity, model] : view.each())
			{
				if(model.GetRootNode()->FindNode(node)) {
					selectedModel = &model;
					return;
				}
			}
			selectedModel = nullptr;
			return;
		};
		auto entitySelector = [&](auto selected) {
			for(auto& entity : m_entities)
			{
				selected = entity.UISelector(selected);
			}
			return selected;
		};


		if(ImGuiOn) {
			if(dockSpace)
				ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
			if(show_demo_window)
				ImGui::ShowDemoWindow(&show_demo_window);
			if(loggerWindow) Logger::Draw("Logger", &loggerWindow);

			if(sceneControlWindow) {

				if(cameraCombo.count != cameras.size()) {
					cameraCombo = ComboParam{};
					std::for_each(cameras.begin(), cameras.end(), [&](const auto& camera) {
						cameraCombo.items += std::to_string(cameraCombo.count) + '\0'; //TODO cameraName
						++cameraCombo.count;
						}
					);
				}
				if(modelCombo.count != models.size()) {
					std::for_each(begin(models), end(models), [&](const auto& model) {
						modelCombo.items += model->GetName() + '\0';
						++modelCombo.count;
						}
					);
				}
				ImGui::Begin("Scene Controls:", &sceneControlWindow);


				if(ImGui::Combo("Cameras", &selectedCamera, cameraCombo.items.c_str())) {
					cameras.SetActiveCamera(selectedCamera);
				}

				if(!cameras.empty())
					cameras[selectedCamera].UIControls();
				if(ImGui::Combo("Lights", &selectedLight, cameraCombo.items.c_str())) {}
				
				selectedEntity= entitySelector(selectedEntity);

				//selectedNode = m_rootNode->UITree();
				
				ImGui::End();
			}
			if(modelControlWindow) {
				ImGui::Begin("Properties", &modelControlWindow);
				if(selectedNode) {					
					modelSelector(selectedNode);
					if(selectedModel)
						selectedModel->UIControls(selectedNode);
				}
				if(m_registry.valid(selectedEntity))
				{
					auto transform = m_registry.try_get<TransformComponent>(selectedEntity);
					if(transform)
						if(UIControls(*transform))
							m_registry.patch<TransformComponent>(selectedEntity, [](TransformComponent& tc) {});

					auto components = m_registry.try_get<
						AmbientLightComponent,
						DirectionalLightComponent,
						PointLightComponent, 
						SpotLightComponent,
						Transform,
						ModelRendererComponent>(selectedEntity);					
					unpackTuple(components);	
				}
				ImGui::End();
			}
		}
		return false;
	}

	/*Entity Scene::CreateEntities(aiNode* node, Vec3f scale)
	{
		uint32_t startSlotMeshes = m_meshes.size();
		
		auto e = Entity(m_registry, node->mName.C_Str());
		TransformComponent tc;
		tc.Scale = scale;

		if(node->mTransformation.IsIdentity())
			tc.Transform = Transform::GetMatrix(tc.Position, tc.Angles, tc.Scale);
		else
			tc.Transform = tc.Transform * Transform::GetMatrix(tc.Position, tc.Angles, tc.Scale);
		e.AddComponent<TransformComponent>(std::move(tc));

		ModelRendererComponent mrc;
		for(auto i = 0; i < node->mNumMeshes; ++i)
		{
			mrc.Meshes.push_back(node->mMeshes[i]+startSlotMeshes);		
		};
		e.AddComponent<ModelRendererComponent>(std::move(mrc));
		for(auto i = 0; i < node->mNumChildren; ++i)
			e.AddChild(CreateEntities(node->mChildren[i], scale));
		return e;
	}*/
	
}

/*if (ImGui::DragFloat3("Pos:x,y,z", model , 0.05f, -1000.0f,1000.0f)){}
		if (ImGui::DragFloat3("Rotation:x,y,z", ent->GetAngles(), 0.05f,-maxangle, maxangle)) {}
		if (ImGui::DragFloat3("Scale:x,y,z", ent->GetScale(), 0.05f, -1000.0f, 1000.0f)) {}
		if (ImGui::DragFloat("Length", &length,0.05f,-1000.0f,1000.0f)) {}*/

