#pragma once

#include "Mesh.h"
#include "Transform.h"
#include <memory>
#include <vector>

struct aiNode;
namespace En3rN::DX
{
	class Renderer;
	class Node
	{
	public:
		using handle = std::shared_ptr<Node>;
		using Container = std::vector<handle>;
		Node() = default;
		Node(const std::string& name);
		Node(aiNode* ainode, Vec3f scale = { 1.f,1.f,1.f });
		~Node() = default;

		
		void SetTransform(const Transform::Matrix& transform);
		void ApplyTransform(const Transform::Matrix& transform);
		void AddMeshIndex(Mesh::Index meshIndex);
		void AddChild(Node::handle&& child) { m_childs.push_back(std::move(child)); }
		
		auto FindNode(Node* node)
		{
			if(this == node)
				return true;
			for(auto& child : m_childs)
				if(child->FindNode(node))
					return true;
			return false;
		};
		Node* UITree();

		const Transform::Matrix&		GetTransform() const		{ return m_TotalTransform; }
		const std::vector<Mesh::Index>&	GetMeshIndecies() const		{ return m_meshIndecies; }
		const Container&				GetChilds()	const			{ return m_childs; }
		const std::string&				GetName() const				{ return m_name; }
		
	private:
		Transform::Matrix			m_parentTransform;
		Transform::Matrix			m_TotalTransform;		
		std::string					m_name;
		std::vector<Mesh::Index>	m_meshIndecies;
		Container					m_childs;

		bool		 ui_nodeOpen;
		static Node* ui_selectedNode;
	};
}
