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
	DynamicDrawable* dynamic = dynamic_cast<DynamicDrawable*>(drawable.get());
	if (dynamic) dynamicDrawableIndex.push_back((int)drawables.size());
	drawables.emplace_back(std::move(drawable));
}

void En3rN::DX::Scene::Bind()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile("assets/untitled.blend",
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	// If the import failed, report it
	if (!scene) {
		Logger::Err(importer.GetErrorString());
		return;
	}
	else
	{
		Logger::Err("loaded");
		
	}
	
}

void En3rN::DX::Scene::Update(float dt)
{	
	for (auto i : dynamicDrawableIndex)
	{
		DynamicDrawable* pUpdateable = static_cast<DynamicDrawable*>(drawables[i].get());
		pUpdateable->Update(dt);
	}
}

void En3rN::DX::Scene::Draw()
{
	Entity* ent = dynamic_cast<Entity*>(drawables[selected].get());
	Vec3f d = ent->GetDirection();
	float maxangle = 2 * PI;
	std::string items;
	int index = 0;
	auto length = ent->GetPosition().Length();
	
	for (auto& i : drawables)
	{
		items += std::to_string(index) + '\0';
		++index;
	}
	ImGui::Begin("Scene Controls:");
		Vec3f cam = camera.GetPosition();
		Vec3f camdir = camera.GetDirection();
		if(ImGui::DragFloat3("Camera", cam, 0.05f, -1000.0f, 1000.0f)) {
			camera.SetPosition(cam);
		}
		if (ImGui::DragFloat3("CameraDir", camdir, 0.05f, (-0.09*maxangle), (-0.09 * maxangle)))
			camera.SetDirection(camdir);
		if(ImGui::Combo("Object", &selected, items.c_str(), index))
			ent = dynamic_cast<Entity*>(drawables[selected].get());
		if (ImGui::DragFloat3("Pos:x,y,z", ent->GetPosition(), 0.05f, -1000.0f,1000.0f)){}
		if (ImGui::DragFloat3("Rotation:x,y,z", ent->GetAngles(), 0.05f, 0.0f, maxangle)) {}
		if (ImGui::DragFloat3("Scale:x,y,z", ent->GetScale(), 0.05f, -1000.0f, 1000.0f)) {}
		if (ImGui::DragFloat("Length", &length,0.05f,-1000.0f,1000.0f)) {}
	ImGui::End();

	for (auto& drawable : drawables)
	{
		Entity& e = dynamic_cast<Entity&>(*drawable.get());
		e.UpdateViewMatrix();
		//check pipline if need to bind
		for (auto &bindable : drawable->GetBindables())
		{
			bindable->Bind();
		}
		drawable->Draw();
	}
}

