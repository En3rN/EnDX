#include "Entity.h"
#include "logger.h"
#include "Window.h"

namespace En3rN
{
	using namespace DirectX;
	using namespace DX;

	size_t Entity::idCounter = -1;
	Entity::Entity() : pos{}, dir{ 0.0f,0.0f,0.0f }, angles{ .0f,.0f,0.f }, scale{ 1.0f ,1.0f ,1.0f }, speed(1), acceleration(0)
	{ 
		idCounter++;
		id = idCounter;
		name = std::to_string(idCounter);
		UpdateViewMatrix(); 
	}
	
	Entity::Entity(float posx, float posy, float posz, float dirx, float diry, float dirz, float scalex, float scaley, float scalez)
	{
		idCounter++;
		id = idCounter;
		SetPosition(posx, posy, posz);
		SetDirection(dirx, diry, dirz);
		SetScale(scalex, scaley, scalez);
		UpdateViewMatrix();
	}
	void Entity::Move(Vec3f& move)
	{
		pos += move;
	}
	void Entity::SetPosition(float x, float y, float z)
	{
		Vec3f p = { x,y,z };
		SetPosition(p);
	}
	void Entity::SetPosition(Vec3f& position)
	{
		pos = position;
	}
	void Entity::SetDirection(float x, float y, float z)
	{
		Vec3f newDir(x, y, z);
		SetDirection(newDir);
	}
	void Entity::SetDirectionFromAngles() {
		Vec3 base(.0f, .0f, 1.f);
		dir = base.Rotate(angles.x, angles.y, angles.z);
	}
	void Entity::SetDirection(Vec3f& direction) 
	{
		dir=XMVector3Normalize(direction);
	}
	void Entity::SetScale(float x, float y, float z)
	{
		Vec3f s = { x,y,z };
		SetScale(s);
	}
	void Entity::SetScale(Vec3f& scale)
	{
		this->scale = scale;
	}
	void Entity::UpdateViewMatrix()
	{
		viewMatrix =
			XMMatrixRotationX(angles.x) *
			XMMatrixRotationY(angles.y) *
			XMMatrixRotationZ(angles.z) *
			XMMatrixScaling(scale.x, scale.y, scale.z) *
			XMMatrixTranslation(pos.x, pos.y, pos.z);
	}
	const DirectX::XMMATRIX& Entity::GetViewMatrix() 
	{ 
		UpdateViewMatrix();
		return viewMatrix; 
	}
};