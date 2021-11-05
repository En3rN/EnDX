#include "Model.h"
#include "Primitive.h"
#include "Teqnique.h"
#include "TeqLambert.h"
#include "Renderer.h"
#include "Shader.h"
#include "Config.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>



namespace En3rN::DX
{
	Model::Model(const aiScene* aiscene)
	{
		for (auto i = 0u; i < aiscene->mNumMeshes; ++i)
			m_meshes.push_back(aiscene->mMeshes[i]);
		for (auto i = 0u; i < aiscene->mNumMaterials; ++i)
			m_materials.push_back(aiscene->mMaterials[i]);
		m_rootNode = std::make_unique<Node>(aiscene->mRootNode);
		AddTeqnique(std::make_shared<TeqLambertian>());
	}
	Model::Model(Mesh&& mesh, const Material& material)
	{
		m_meshes.push_back(std::move(mesh));
		m_materials.push_back(material);
	}
	Model::Model(Mesh::Container&& meshes, Material::Container&& materials) :
		m_rootNode(nullptr), m_meshes(std::move(meshes)), m_materials(std::move(materials)) {}
	
	void Model::Update(float dt)
	{
		for (auto& mesh : m_meshes)
			mesh.Update(dt);
	}

	void Model::CreateJobs(Renderer& renderer, Node& node)
	{
		for (auto& meshIndex : node.GetMeshIndecies()) {
			auto& mesh = m_meshes[meshIndex];
			auto& material = m_materials[mesh.GetMaterialIndex()];
			for (auto& teq : mesh.GetTeqniques()){
				if(teq->IsActive())
					for (auto& step : teq->GetSteps()){
						renderer.GetPass(step.GetRenderPassSlot()).AddJob(
							Job(mesh, material, node.GetTransform(), step));
					}
			}
		}
		for (auto& childs : node.GetChilds())
			CreateJobs(renderer, *childs);

	}
	void Model::AddMesh(Mesh&& mesh)
	{
		m_meshes.push_back(std::move(mesh));
	}
	void Model::AddMaterial(Material material)
	{
		m_materials.push_back(material);
	}
	void Model::AddTeqnique(Teqnique::Base::handle teqnique)
	{	
		for (auto& mesh : m_meshes)
			mesh.AddTeqnique(teqnique);
	}
	void Model::ApplyTransform(Vec3f position, Vec3f rotationAngles, Vec3f scale)
	{
		using namespace DirectX;
		Transform::Matrix matrix = XMMatrixTranspose(
			XMMatrixRotationRollPitchYaw(rotationAngles.y, rotationAngles.x, rotationAngles.z) *
			XMMatrixScaling(scale.x, scale.y, scale.z) *
			XMMatrixTranslation(position.x, position.y, position.z));
		m_rootNode->ApplyTransform(matrix);

	}
	/*const std::vector<std::string> Model::GetSignatures()
	{
		std::vector<std::string> signatures;
		std::for_each(begin(m_teqniques), end(m_teqniques), [&](Teqnique::Base::handle& teq) {
			auto& steps = teq->GetSteps();
			std::for_each(begin(steps), end(steps), [&](Step& step) {
				auto shaderName = step.GetPassName();
				auto vs = BindableManager::Query<VertexShader>(shaderName);
				auto s = vs->GetSignatures();
				std::for_each(begin(s), end(s), [&](std::vector<std::string>::iterator it) {
					if (std::find(begin(signatures), end(signatures), *it) != end(signatures))
						signatures.insert(it, *it);
					});
				});
			});
		return signatures;
	}*/

	Model::handle Model::LoadModel(std::filesystem::path file)
	{
		file.make_preferred();
		auto& modelPath = Config::GetFolders().modelPath;
		modelPath = Config::GetFolders().assets;
		if (file.has_parent_path()) {
			modelPath += file.parent_path();
		}
		std::filesystem::path path = modelPath;
		path /= file.filename();
		
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path.string(),
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_GenNormals |
			aiProcess_ConvertToLeftHanded |
			aiProcess_CalcTangentSpace);
		// If the import failed, report it
		if (!scene) {
			std::string err = "Failed to load model: ";
			err += importer.GetErrorString();
			throw EnExcept(err, EnExParam);
		}
		return std::make_unique<Model>(scene);
	}
	Model::handle Model::LoadPrimitive()
	{
		Primitive factory;
		return factory();
	}
}
