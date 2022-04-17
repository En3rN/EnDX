#include "Model.h"
#include "Primitive.h"
#include "Teqnique.h"
#include "Renderer.h"
#include "Shader.h"
#include "Config.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <assimp\version.h>



namespace En3rN::DX
{
	Model::Model(std::filesystem::path file, Vec3f scale)
	{
		std::filesystem::path path;
		if(file.has_parent_path())
			path = file.parent_path();
		else
			path = file;
		std::stringstream version;
		version << "Assimp version: " << "Major:" << aiGetVersionMajor() << " Minor:" << aiGetVersionMinor() << " Revision:" << aiGetVersionRevision() << std::endl;
		OutputDebugString(version.str().c_str());

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(file.string(),
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_ConvertToLeftHanded |
			aiProcess_CalcTangentSpace |
			aiProcess_GenNormals |
			aiProcess_FixInfacingNormals
		);
		// If the import failed, report it
		if(!scene) {
			std::string err = "Failed to load model: ";
			err += importer.GetErrorString();
			throw EnExcept(err);
		}
		Logger::Debug(importer.GetErrorString());

		for(auto i = 0u; i < scene->mNumMeshes; ++i)
			Meshes.push_back(scene->mMeshes[i]);
		for(auto i = 0u; i < scene->mNumMaterials; ++i)
			Materials.push_back(scene->mMaterials[i]);
		m_rootNode = std::make_unique<Node>(scene->mRootNode, scale);
	}
	Model::Model(const aiScene* aiscene, const std::string& name, Vec3f scale) : m_name(name), m_controlWindow(false)
	{
		for (auto i = 0u; i < aiscene->mNumMeshes; ++i)
			Meshes.push_back(aiscene->mMeshes[i]);
		for (auto i = 0u; i < aiscene->mNumMaterials; ++i)
			Materials.push_back(aiscene->mMaterials[i]);
		m_rootNode = std::make_unique<Node>(aiscene->mRootNode, scale);
		AddTeqnique(Teqnique("Phong",true));
	}
	void Model::Update(float dt)
	{
		for (auto& mesh : Meshes)
			mesh.Update(dt);
	}

	void Model::CreateJobs(Renderer& renderer, Node& node)
	{
		for (auto& meshIndex : node.GetMeshIndecies()) {
			auto& mesh = Meshes[meshIndex];
			auto& material = Materials[mesh.GetMaterialIndex()];
			for (auto& teq : mesh.GetTeqniques()){
				if(teq.IsActive())
					for (auto& step : teq.GetSteps()){
						//renderer.GetPass(step.GetPassName()).AddJob(
							//RenderJob(mesh, material, node.GetTransform(), step));
					}
			}
		}
		for (auto& child : node.GetChilds())
			CreateJobs(renderer, *child);

	}
	void Model::AddMesh(Mesh&& mesh)
	{
		Meshes.push_back(std::move(mesh));
	}
	void Model::AddMaterial(Material material)
	{
		Materials.push_back(material);
	}
	void Model::AddTeqnique(Teqnique teqnique)
	{	
		for (auto& mesh : Meshes)
			mesh.AddTeqnique(teqnique, Materials);
	}
	void Model::ApplyTransform(Vec3f position, Vec3f rotationAngles, Vec3f scale)
	{
		using namespace DirectX;
		Transform::Matrix matrix = 
			XMMatrixRotationRollPitchYaw(rotationAngles.y, rotationAngles.x, rotationAngles.z) *
			XMMatrixScaling(scale.x, scale.y, scale.z) *
			XMMatrixTranslation(position.x, position.y, position.z);
		m_rootNode->ApplyTransform(matrix);

	}

	Model::handle Model::LoadModel(std::filesystem::path file, Vec3f scale)
	{
		file.make_preferred();
		auto& modelPath = Config::GetFolders().modelPath;
		modelPath = Config::GetFolders().assets;
		if (file.has_parent_path()) {
			modelPath += file.parent_path();
		}
		std::filesystem::path path = modelPath;
		path /= file.filename();

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
			aiProcess_GenNormals |
			aiProcess_FixInfacingNormals
		);
		// If the import failed, report it
		if(!scene) {
			std::string err = "Failed to load model: ";
			err += importer.GetErrorString();
			throw EnExcept(err);
		}
		Logger::Debug(importer.GetErrorString());
		return std::make_unique<Model>(scene, path.filename().string(), scale);
	}
	Model::handle Model::LoadPrimitive()
	{
		Primitive factory;
		return factory();
	}

	bool Model::UIControls(Node* node)
	{
		for (auto index : node->GetMeshIndecies()) {
			Materials[Meshes[index].GetMaterialIndex()].UIControls();
		}

		return false;
	}

	
	
	
	
		


	
}
