#include "Entity.h"
#include "Component.h"
#include "logger.h"
#include "Window.h"

namespace En3rN::DX
{
	using namespace DirectX;

	size_t Entity::idCounter = -1;
	Entity::Entity() : pos{}, dir{ 0.0f,0.0f,0.0f }, angles{ .0f,.0f,0.f }, scale{ 1.0f ,1.0f ,1.0f }, speed(1), acceleration(0)
	{ 
		id = ++idCounter;
		name = std::to_string(idCounter);
	}
	
	Entity::Entity(float posx, float posy, float posz, float dirx, float diry, float dirz, float scalex, float scaley, float scalez)
	{
		id = ++idCounter;
		SetPosition(posx, posy, posz);
		SetDirection(dirx, diry, dirz);
		SetScale(scalex, scaley, scalez);
	}
	Entity::Entity(Vec3f&& position) : pos(std::move(position)), dir(0,0,0), angles(0,0,0), scale(1,1,1), speed(1), acceleration(0)
	{
		id = ++idCounter;
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
		dir = base.Rotate(angles.y, angles.x, angles.z);
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

	void Entity::SetName(std::string&& name)
	{
		this->name = std::move(name);
	}

	void Entity::AddComponent(const std::string& key, Component::Base::handle&& component)
	{
		components[key] = std::move(component);
	}

	Component* Entity::GetComponent(const std::string& key) 
	{
		return components.at(key).get();
	}
	
	DirectX::XMMATRIX Entity::GetViewMatrix() const
	{		
		return DirectX::XMMatrixTranspose(
			XMMatrixRotationRollPitchYaw(angles.y, angles.x, angles.z) *
			XMMatrixScaling(scale.x, scale.y, scale.z) *
			XMMatrixTranslation(pos.x, pos.y, pos.z)
		);
	}
};