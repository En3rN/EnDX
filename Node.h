#pragma once

#include "Mesh.h"
#include "Transform.h"
#include <memory>
#include <vector>

class aiNode;
namespace En3rN::DX
{
	class Renderer;
	class Node
	{
	public:
		using handle = std::unique_ptr<Node>;
		using Container = std::vector<handle>;
		Node(aiNode* ainode);
		void ApplyTransform(const Transform::Matrix& transform);
		void AddChild(Node::handle&& child) { m_childs.push_back(std::move(child)); }

		const Transform::Matrix&	GetTransform()		{ return m_transform.GetMatrix(); }
		const std::vector<size_t>&	GetMeshIndecies()	{ return m_meshIndecies; }
		const Container&			GetChilds()			{ return m_childs; }
		
	private:
		std::string					m_name;
		std::vector<size_t>			m_meshIndecies;
		Transform					m_transform;
		Container					m_childs;
	};
}
