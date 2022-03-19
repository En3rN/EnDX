#include "Node.h"
#include "Job.h"
#include "Renderer.h"
#include <assimp\scene.h>

namespace En3rN::DX
{
	Node* Node::ui_selectedNode = nullptr;

	Node::Node(const std::string& name) :
		m_name(name),
		ui_nodeOpen(false),
		m_TotalTransform(DirectX::XMMatrixIdentity()),
		m_parentTransform(DirectX::XMMatrixIdentity()),
		m_childs{},
		m_meshIndecies{}
	{
		
	}

	Node::Node(aiNode* ainode, Vec3f scale) :
		m_name(ainode->mName.C_Str()),
		ui_nodeOpen(false),
		m_TotalTransform(DirectX::XMMatrixIdentity()),
		m_parentTransform(DirectX::XMMatrixIdentity())	
	{
		for (auto i = 0; i < ainode->mNumMeshes; ++i)
			m_meshIndecies.push_back(ainode->mMeshes[i]);
		for (auto i = 0; i < ainode->mNumChildren; ++i)
			m_childs.push_back(std::make_unique<Node>(ainode->mChildren[i]));
		SetTransform(DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
			DirectX::XMLoadFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&ainode->mTransformation)));
	}
	void Node::SetTransform(const Transform::Matrix& transform)
	{		
		m_TotalTransform = transform * m_parentTransform;
		for(auto& child : m_childs)
			child->ApplyTransform(m_TotalTransform);
	}	
	void Node::ApplyTransform(const Transform::Matrix& transform)
	{
		m_parentTransform = transform;
		m_TotalTransform *= m_parentTransform;
		for (auto& child : m_childs)
			child->ApplyTransform(m_TotalTransform);
	}

	void Node::AddMeshIndex(Mesh::Index meshIndex)
	{
		m_meshIndecies.push_back(meshIndex);
	}

	Node* Node::UITree()
	{
		ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected;
		ImGuiTreeNodeFlags flags{};
		if(ui_selectedNode == this)
			flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected;
		if(m_childs.empty())
			flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Leaf;
		ui_nodeOpen = ImGui::TreeNodeEx(m_name.c_str(),flags); 
		if(ImGui::IsItemClicked()) {
			ui_selectedNode = this;
			std::string debug = ui_selectedNode->GetName() + "\n";
			Logger::Debug(debug.c_str());
		}
		if(ui_nodeOpen) {
			for(auto& node : m_childs)
				node->UITree();
			ImGui::TreePop();
		}
		return ui_selectedNode;
	}
}
