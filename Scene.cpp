#include "Scene.h"
#include "Graphics.h"
#include "Entity.h"
#include "vec.h"
#include "logger.h"
#include "enMath.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

void En3rN::DX::Scene::AddDrawable(Drawable::handle drawable)
{
	drawables.emplace_back(std::move(drawable));
}

void En3rN::DX::Scene::AddModel(Model::handle model)
{
	models.push_back(model);
}

void En3rN::DX::Scene::Bind(Renderer& renderer)
{
}

void En3rN::DX::Scene::Update(float dt)
{	
	//camera.SetConstantBuffer(VSConstantBuffer::Matrix::View);
	camera.SetConstantBuffer();
	Light light;
	light.GetLightCB();

	
	for (auto& drawable : drawables)
		drawable->Update(dt);
	for (auto& model : models)
		model->Update(dt);
}

void En3rN::DX::Scene::Draw()
{
	//Entity* ent = static_cast<Entity*>(drawables[selected].get());
	std::string items;
	int index = 0;
	//auto length = ent->GetPosition().Length();
	
	for (auto& i : drawables)
	{
		items += i-> GetName()+'\0';
		++index;
	}
	ImGui::Begin("Scene Controls:");
		Vec3f cam = camera.GetPosition();
		Vec3f camdir = camera.GetDirection();		
		auto maxangle = PI;
		if(ImGui::DragFloat3("Camera", cam, 0.05f, -1000.0f, 1000.0f)) {
			camera.SetPosition(cam);
		}
		if (ImGui::DragFloat3("CameraDir", camdir, 0.05f, (-0.09* maxangle), (-0.09 * maxangle)))
			camera.SetDirection(camdir);
		if (ImGui::DragFloat4("Proj", &camera.w, .1f, .1f, 2000.f)) {};
		if (ImGui::Checkbox("FOV",&camera.bfov)){ }
		ImGui::SameLine();
		ImGui::DragFloat2("FOV", &camera.fov,.01f,.1f,2.f);
		/*if(ImGui::Combo("Object", &selected, items.c_str(), index))
			ent = static_cast<Entity*>(drawables[selected].get());
		if (ImGui::DragFloat3("Pos:x,y,z", ent->GetPosition(), 0.05f, -1000.0f,1000.0f)){}
		if (ImGui::DragFloat3("Rotation:x,y,z", ent->GetAngles(), 0.05f,-maxangle, maxangle)) {}
		if (ImGui::DragFloat3("Scale:x,y,z", ent->GetScale(), 0.05f, -1000.0f, 1000.0f)) {}
		if (ImGui::DragFloat("Length", &length,0.05f,-1000.0f,1000.0f)) {}*/
	ImGui::End();

	for (auto& drawable : drawables)
	{		
		//check pipline if need to bind
		for (auto &bindable : drawable->GetBindables())
		{
			bindable->Update(*drawable.get());
			bindable->Bind();
		}
		drawable->Draw();
	}
	for (auto& model : models)
		model->CreateJobs(*m_renderer, model->GetRootNode());
	m_renderer->Draw();

}

