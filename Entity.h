#pragma once
#include "vec.h"
#include "Component.h"
#include <string>

namespace En3rN::DX
{
	class Entity
	{
	public:
		Entity();
		Entity(float posx, float posy, float posz, float dirx, float diry, float dirz, float scalex, float scaley, float scalez);
		Entity(Vec3f&& position);
		Entity(Entity&& other) noexcept = default;
		Entity& operator = (Entity&& other) noexcept = default;

		Vec3f& GetPosition()	{ return pos; }
		Vec3f& GetDirection()	{ return dir; }
		Vec3f& GetAngles()		{ return angles; }
		Vec3f& GetScale()		{ return scale; }

		const std::string& GetName() { return name; }

		void SetPosition(float x, float y, float z);
		void SetPosition(Vec3f& position);
		void SetDirection(float x, float y, float z);
		void SetDirection(Vec3f& direction);
		void SetDirectionFromAngles();
		void SetScale(float x, float y, float z);
		void SetScale(Vec3f& scale);
		void SetName(std::string&& name);

		void AddComponent(const std::string& key, Component::Base::handle && component);
		Component* GetComponent(const std::string& key);

		virtual DirectX::XMMATRIX GetViewMatrix() const;
		virtual ~Entity() = default;
	protected:

		Component::Base::Container	components;  // map<string, uniqe_ptr >
		static size_t	idCounter;
		std::string		name;
		size_t			id;
		Vec3f			pos;
		Vec3f			dir;
		Vec3f			angles;
		Vec3f			scale;
		uint8_t			speed;
		uint8_t			acceleration;

	private:
	};

}
