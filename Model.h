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
	public:
		using handle = std::shared_ptr<Model>;
		using Container = std::vector<handle>;
		
		template <typename ... ConstuctorArgs>
		static handle CreateHandle(ConstuctorArgs&& ... args)
		{
			return std::make_shared<Model>(std::forward<ConstuctorArgs>(args)...);
		}
		
		Model() = default;
		Model(const aiScene* aiscene);
		Model(Mesh&& mesh, const Material& material);
		Model(Mesh::Container&& meshes, Material::Container&& materials);
		//Model(const Model& other) = default; 
		Model(Model&& other) noexcept = default;
		//Model& operator = (const Model& other) = default;
		Model& operator = (Model && other) noexcept = default;
		~Model() = default;

		void Update(float dt);
		void CreateJobs(Renderer& renderer, Node& node);

		void AddMesh(Mesh&& mesh);
		void AddMaterial(Material material);
		void AddTeqnique(Teqnique::Base::handle teqnique);
		void ApplyTransform(Vec3f position, Vec3f rotationAngles, Vec3f scale);

		const Material& GetMaterial(Material::Index index) { return m_materials.at(index); }
		
		Node& GetRootNode() { return *m_rootNode; }
		//const std::vector<std::string> GetSignatures();
		
		static Model::handle LoadModel(std::filesystem::path filename);
		static Model::handle LoadPrimitive(); //random primitive
	
	protected:
		std::string						m_name;
		Transform						m_transform;
		Mesh::Container					m_meshes;
		Material::Container				m_materials;
		Node::handle					m_rootNode;
	};
}

