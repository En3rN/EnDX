#include "Node.h"
#include "Job.h"
#include "Renderer.h"
#include <assimp\scene.h>

namespace En3rN::DX
{
	Node::Node(aiNode* ainode) : 
		m_name(ainode->mName.C_Str()), 
		m_transform(ainode->mTransformation)
	{
		for (auto i = 0; i < ainode->mNumMeshes; ++i)
			m_meshIndecies.push_back(ainode->mMeshes[i]);
		for (auto i = 0; i < ainode->mNumChildren; ++i)
			m_childs.push_back(std::make_unique<Node>(ainode->mChildren[i]));
	}
	void Node::ApplyTransform(const Transform::Matrix& transform)
	{
		m_transform.ApplyTransform(transform);
		for (auto& child : m_childs)
			child->ApplyTransform(m_transform.GetMatrix());
		
	}
	void Node::CreateJobs(Renderer& renderer)
	{
		
		return;
	}
}
