#pragma once
#include "Material.h"
#include "Mesh.h"
#include "Node.h"
#include "Teqnique.h"
#include "Entity.h"
#include "Transform.h"
#include <memory>
#include <vector>

struct aiScene;

namespace En3rN::DX
{
	class Renderer;
	
	class Model
	{
		friend class Scene;
	public:
		using handle = std::shared_ptr<Model>;
		using Container = std::vector<handle>;
		
		template <typename ... ConstuctorArgs>
		static handle CreateHandle(ConstuctorArgs&& ... args)
		{
			return std::make_shared<Model>(std::forward<ConstuctorArgs>(args)...);
		}
		
		Model(std::filesystem::path filename, Vec3f scale = { 1.f,1.f,1.f });
		Model(const aiScene* aiscene, const std::string& name, Vec3f scale = { 1.f,1.f,1.f });
		//Model(const Model& other) = default; 
		Model(Model&& other) noexcept = default;
		//Model& operator = (const Model& other) = default;
		Model& operator = (Model && other) noexcept = default;
		~Model() = default;

		void Update(float dt);
		void CreateJobs(Renderer& renderer, Node& node);

		void AddMesh(Mesh&& mesh);
		void AddMaterial(Material material);
		void AddTeqnique(Teqnique teqnique);
		void ApplyTransform(Vec3f position, Vec3f rotationAngles, Vec3f scale);

		const Material& GetMaterial(Material::Index index) const { return m_materials[index]; }
		const std::string& GetName() const { return m_name; }
		
		Node::handle GetRootNode() { return m_rootNode; }
		
		//const std::vector<std::string> GetSignatures();
		
		static Model::handle LoadModel(std::filesystem::path filename, Vec3f scale = { 1.f,1.f,1.f });
		static Model::handle LoadPrimitive(); //random primitive

		bool UIControls(Node* node);
	
	protected:
		std::string						m_name;
		Transform						m_transform;
		std::filesystem::path			m_path;
		Mesh::Container					m_meshes;
		Material::Container				m_materials;
		Node::handle					m_rootNode;
		bool							m_controlWindow = true;
	};
}

