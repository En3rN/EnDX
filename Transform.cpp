#include "Transform.h"
#include <functional>
#include <assimp\scene.h>
#include <assimp\quaternion.inl>
#include <assimp\matrix4x4.inl>
#include <assimp\vector3.h>
#include <assimp\vector3.inl>
#include <assimp\vector2.h>

namespace En3rN::DX
{
	using namespace DirectX;
	
	
	Transform::Transform(const Matrix& transform)
	{
		XMVECTOR position{};
		XMVECTOR rotation{};
		XMVECTOR scale{};
		XMVECTOR axis{};
		float angle{};
		float pitch{}, yaw{}, roll{};
		
		XMFLOAT3X3 mat{};
		XMStoreFloat3x3(&mat, transform);

		Vec3f x = { mat._11,mat._12,mat._13 };
		Vec3f y = { mat._21,mat._22,mat._23 };
		Vec3f z = { mat._31,mat._32,mat._33 };

		Vec2f rightVec = Vec2f(mat._11, mat._12).Normalized();

		Vec2f upVec = Vec2f(mat._23, mat._22).Normalized();
		Vec2f fwdVec = Vec2f( mat._31, -mat._33).Normalized();

		float pitchMod{}, yawMod{}, rollMod{};
		
		/*dotUP    = upVec.Dot();
		dotFwd   = fwdVec.Dot({ 0,-1 });
		dotRight = rightVec.Dot({ 1, 0});*/

		auto rad = [](Vec2f vec, Vec2f dotvec)
		{
			if(vec.y > 0)
			{
				if(vec.x < 0)
					return -acos(vec.Dot(dotvec));
				else if(vec.x > 0)
					return acos(vec.Dot(dotvec));
			}
			else
			{
				if(vec.x < 0)
					return -acos(vec.Dot(dotvec));
				else if(vec.x > 0)
					return acos(vec.Dot(dotvec));
			}
		};
		pitch = rad(upVec,{0,1});
		yaw = rad(fwdVec, { 0,-1 });
		if(yaw < -halfPI || yaw> halfPI)
		{
			roll += PI;
		}
		roll += rad(rightVec, { 1,0 });
		if(pitch < -halfPI || pitch> halfPI)
		{
			yaw -= PI;
		}
		if(roll<-halfPI || roll> halfPI)
		{
			pitch -= PI;
		}
		
		


		XMMatrixDecompose(&scale, &rotation, &position, transform);
		m_position = position;
		m_scale = scale;
		XMFLOAT4 axisf4{};
		XMQuaternionToAxisAngle(&axis, &angle, rotation);
		XMStoreFloat4(&axisf4,axis);
		
		//toEuler(axisf4.x, axisf4.y, axisf4.z, angle);
		
		//TODO finne ut av cornercases og alt failer ved yaw
		//float p = asinf(-mat._32); // Pitch
		//
		//pitch = atan2f(-mat._32, mat._33);
		//float cosP = cos(p);

		//if(cosP > 0.001 )			// Not at poles
		//{
		//	yaw = atan2f(-mat._31, mat._33);      // Yaw
		//	roll = atan2f(mat._12, mat._22);      // Roll
		//}
		//else
		//{
		//	yaw = 0.0f;                           // Yaw
		//	roll = atan2f(-mat._21, mat._11);     // Roll
		//}
		//if(pitch > halfPI || pitch < -halfPI)
		//{
		//	yaw -= PI;
		//	roll -= PI;
		//}

		m_angles.x = pitch + pitchMod;
		m_angles.y = yaw + yawMod;		
		m_angles.z = roll + rollMod;
	}
	 void Transform::toEuler(double x, double y, double z, double angle) {
		double s = sin(angle);
		double c = cos(angle);
		double t = 1 - c;
		//  if axis is not already normalised then uncomment this
		// double magnitude = Math.sqrt(x*x + y*y + z*z);
		// if (magnitude==0) throw error;
		// x /= magnitude;
		// y /= magnitude;
		// z /= magnitude;
		if((x * y * t + z * s) > 0.998) { // north pole singularity detected
			m_angles.y = 2 * atan2(x * sin(angle / 2), cos(angle / 2));
			m_angles.x = PI / 2;
			m_angles.z = 0;
			return;
		}
		if((x * y * t + z * s) < -0.998) { // south pole singularity detected
			m_angles.y	 = -2 * atan2(x * sin(angle / 2), cos(angle / 2));
			m_angles.x	 = -PI / 2;
			m_angles.z	= 0;
			return;
		}
		m_angles.y	= atan2(y * s - x * z * t, 1 - (y * y + z * z) * t);
		m_angles.x	= asin(x * y * t + z * s);
		m_angles.z	= atan2(x * s - y * z * t, 1 - (x * x + z * z) * t);
	}
	
	/*pitch = atan2f(-z.y, z.z);
	yaw = atan2f(-x.z, x.x);
	roll = atan2f(x.y, x.x);

	if(yaw > halfPI)
	{
		pitch += PI;
		roll += -PI;
	}
	if(roll > halfPI)
	{
		yaw += PI;
	*/
	
	Transform::Transform(Vec3f position, Vec3f angles, Vec3f scale) :
		m_position(position),m_angles(angles), m_scale(scale){}

	const Transform::Matrix Transform::GetMatrix() const
	{
		return XMMATRIX(XMMatrixIdentity()*
			XMMatrixRotationRollPitchYaw(m_angles.x,m_angles.y,m_angles.z) *
			XMMatrixScaling(m_scale.x,m_scale.y,m_scale.z) *
			XMMatrixTranslation(m_position.x, m_position.y, m_position.z));
	}
	Vec3f& Transform::Position()
	{
		return m_position;
	}
	Vec3f& Transform::Angles()
	{
		return m_angles;
	}
	Vec3f& Transform::Scale()
	{
		return m_scale;
	}
	const Vec3f& Transform::GetPosition() const
	{
		return m_position;
	}
	Vec3f Transform::GetDirection() const
	{
		XMFLOAT4X4 mat;
		XMStoreFloat4x4(&mat, GetMatrix());
		return Vec3f(mat._31, mat._32, mat._33);
	}

	Vec3f Transform::GetXAxis() const
	{
		XMFLOAT4X4 mat;
		DirectX::XMStoreFloat4x4(&mat, GetMatrix());
		return Vec3f(mat._11, mat._12, mat._13);
	}

	Vec3f Transform::GetUpAxis() const
	{
		XMFLOAT4X4 mat;
		DirectX::XMStoreFloat4x4(&mat, GetMatrix());
		return Vec3f(mat._21, mat._22, mat._23);
	}

	void Transform::Listener(void(int i, char c))
	{
		int dummy = 1;
	}

	Transform::Matrix Transform::GetMatrix(Vec3f& pos, Vec3f& angles, Vec3f& scale)
	{
		return XMMATRIX(XMMatrixIdentity() *
			XMMatrixScaling(scale.x, scale.y, scale.z) *
			XMMatrixRotationRollPitchYaw(angles.x, angles.y, angles.z) *
			XMMatrixTranslation(pos.x, pos.y, pos.z));
	}

	Vec3f Transform::GetDirection(Vec3f& angles)
	{
		XMFLOAT3X3 mat;
		XMStoreFloat3x3(&mat,XMMatrixRotationRollPitchYaw(angles.x, angles.y, angles.z));
		return Vec3f(mat._31, mat._32, mat._33);
	}

	bool Transform::UIControls()
	{
		bool change = false;
		//ImGui::BeginChild("Transform", { 0,81 }, true, ImGuiWindowFlags_AlwaysAutoResize);
		if (ImGui::DragFloat3("Position", m_position,.1f ))		{ change = true; }
		if (ImGui::SliderFloat3("Angles", m_angles,	-PI,PI))	{ change = true; }
		if (ImGui::DragFloat3("Scale", m_scale,.1f ))			{ change = true; }
		//ImGui::EndChild();
		return change;
	}
	//void Transform::OnAttach()
	//{
	//	//Data data;
	//	//init data;
	//	//system<Transform>.register({entt, data}) --> index
	//}
	//void Transform::OnUpdate()
	//{
	//}
	//void Transform::OnDetach()
	//{
	//	//system<Transform> [entt][index].erase()
	//}
}
