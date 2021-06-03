#pragma once
#include "vec.h"
namespace En3rN
{
	
	constexpr float PI = 3.14159265359f;

	class Entity
	{
	public:
		Entity();
		Entity(float posx, float posy, float posz, float dirx, float diry, float dirz, float scalex, float scaley, float scalez);
		Vec3f& GetPosition() { return pos; }
		Vec3f& GetDirection() { return dir; }
		Vec3f& GetScale() { return scale; }
		void Move(Vec3f& move);
		void SetPosition(float x, float y, float z);
		void SetPosition(Vec3f& position);
		void SetDirection(float x, float y, float z);
		void SetDirection(Vec3f& direction);
		void SetScale(float x, float y, float z);
		void SetScale(Vec3f& scale);
		virtual void UpdateViewMatrix();
		virtual ~Entity() = default;
		virtual const DirectX::XMMATRIX& GetViewMatrix();
	protected:
		Vec3f pos;
		Vec3f dir;
		Vec3f angles;
		Vec3f scale;
		uint8_t speed;
		DirectX::XMMATRIX viewMatrix;
	};

}
