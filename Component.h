#pragma once

#include "vec.h"
#include "Mesh.h"
#include "Teqnique.h"
#include <filesystem>
#include <memory>
#include <vector>
#include <string>
#include <map>

namespace En3rN::DX
{
	/*class Component
	{
	public:
		struct Base
		{
			using handle = std::shared_ptr<Component>;
			using Container = std::map<std::string, handle>;
		};
		virtual void OnAttach() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDetach() = 0;
		virtual ~Component() = default;
	};*/
	struct UIDComponent
	{
		size_t	uId{};
	};
	struct TagComponent
	{
		std::string tag;
		operator const std::string& () { return tag; }
		operator std::string& () { return tag; }
	};
	struct AmbientLightComponent
	{
		Vec4f Ambient = { .1f,.1f,.1f,.1f };
		Vec4f& operator ()() { return Ambient; }
	};
	struct DirectionalLightComponent
	{
		Vec4f Color = { 1,1,1,1 };
		uint32_t  IsDirectional = true;
	};
	struct PointLightComponent
	{
		Vec4f Color = { 1,1,1,1 };
		Vec3f Attenuation = { 1.f,0.025f,0.003f };
	};
	struct SpotLightComponent
	{
		Vec4f Color = { 1,1,1,1 };
		Vec3f Attenuation = { 1.f,0.025f,0.003f };
		float ConeInner = 0.8f;
		float ConeOuter = 0.8f;
	};
	struct TransformComponent
	{
		Transform::Matrix Transform = DirectX::XMMatrixIdentity();
		Transform::Matrix ParentTransform = DirectX::XMMatrixIdentity();
		Vec3f Position{};
		Vec3f Angles{};
		Vec3f Scale{1,1,1};
	};	
	struct ModelRendererComponent
	{		
		std::filesystem::path				Path;
		std::vector<Teqnique>				Teqniques;
		std::vector<Mesh::Index>			MeshIndecies;
	};
	struct CameraComponent;
	
}