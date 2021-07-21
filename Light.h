#pragma once
#include "Entity.h"
#include "Buffer.h"
namespace En3rN::DX
{
	class Light : public Entity
	{
	public:
		struct Data
		{
			alignas(16)Vec3f color;
		};
	protected:
		
	};
	class DirecionalLight : public Light
	{
	public:
		struct Data
		{
			alignas(16)Vec3f color;
			alignas(16)Vec3f direction;
		};
		DirecionalLight();
		DirecionalLight(Vec3f dir);
	private:
	};
	class PointLight : public Light
	{
	public:
		struct Data
		{
			alignas(16)Vec3f color;
			alignas(16)Vec3f position;
		};
	};
	class LightCB : public ConstantBuffer<std::vector<LightCB>>
	{
	public:
		using Data = std::vector<LightCB>;
		
			
		
		
	private:
		Light::Data ambient;
		DirecionalLight::Data directional;
		PointLight::Data point;

	};
}

