#include "Transform.h"
#include <assimp\scene.h>
#include <assimp\quaternion.inl>
#include <assimp\matrix4x4.inl>
#include <assimp\vector3.h>
#include <assimp\vector3.inl>
#include <assimp\vector2.h>

namespace En3rN::DX
{
	using namespace DirectX;
	Transform::Transform() :
		m_matrix(DirectX::XMMatrixIdentity()),
		m_constantbuffer(BindableManager::Query<ConstantBuffer>(1, 1))
	{
	}
	Transform::Transform(aiMatrix4x4 aiMatrix) : 
		m_matrix(DirectX::XMLoadFloat4x4(reinterpret_cast<const DirectX::XMFLOAT4X4*>(&aiMatrix))),
		m_constantbuffer(BindableManager::Query<ConstantBuffer>(1, 1))
	{
	}
	//aiVector3D pos{};
		//aiVector3D scale{};
		//aiQuaternion q{};
		//aiMatrix.Decompose(scale, q, pos);
		///*Matrix m;
		//DirectX::XMQuaternionToAxisAngle()*/

		//m_position={pos.x, pos.y, pos.z};
		//m_scale = {scale.x,scale.y,scale.z};
		//m_rotationAngles.z = atan2(2.0 * (q.y * q.z + q.w * q.x), 1.0 - 2.0 * (q.x * q.x + q.z * q.z));
		//m_rotationAngles.x  = asin(2.0 * (q.z * q.w - q.y * q.x));
		//m_rotationAngles.y = atan2(2.0 * (q.y * q.w + q.x * q.z), -1.0 + 2.0 * (q.w * q.w + q.x * q.x));
	Transform::Transform(Vec3f position, Vec3f rotationAngles, Vec3f scale) :
		m_matrix(
			XMMatrixRotationRollPitchYaw(rotationAngles.y, rotationAngles.x, rotationAngles.z)*
			XMMatrixScaling(scale.x, scale.y, scale.z)*
			XMMatrixTranslation(position.x, position.y, position.z)),
		m_constantbuffer(BindableManager::Query<ConstantBuffer>(1,1))
	{
	}
	const Transform::Matrix& Transform::GetMatrix() const
	{
		using namespace DirectX;
		return m_matrix;
	}
	void Transform::SetTransform(Vec3f position, Vec3f rotationAngles, Vec3f scale)
	{
		m_matrix=
			XMMatrixRotationRollPitchYaw(rotationAngles.y, rotationAngles.x, rotationAngles.z) *
			XMMatrixScaling(scale.x, scale.y, scale.z) *
			XMMatrixTranslation(position.x, position.y, position.z);
	}
	void Transform::ApplyTransform(const Matrix& transform)
	{
		m_matrix *= transform;
	}
	void Transform::Rotate(float pitch, float yaw, float roll)
	{
		m_matrix *= XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	}
	void Transform::Translate(float x, float y, float z)
	{
		m_matrix *= XMMatrixTranslation(x, y, z);
	}
	void Transform::Scale(float x, float y, float z)
	{
		m_matrix *= XMMatrixScaling(x, y, z);
	}
	void Transform::Bind()
	{
		return m_constantbuffer->Update(GetMatrix());
	}
	void Transform::OnUpdate()
	{
	}
	/*void Transform::ConstantBuffer::Update(const Entity& e)
	{
		auto data = e.GetViewMatrix();
		D3D11_MAPPED_SUBRESOURCE msr{ &data,0,0 };
		errchk::hres(pContext->Map(pBuffer.Get(), slot, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0u, &msr), EnExParam);
		pContext->Unmap(pBuffer.Get(), slot);
	}*/
}
