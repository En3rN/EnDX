#pragma once


#include "Component.h"
#include "Buffer.h"

namespace En3rN::DX
{
	class LightCB;
	class Light : public Component
	{
	public:
		struct Data
		{
			alignas(16)Vec3f color;
		};
		std::shared_ptr<LightCB> GetLightCB() { return m_cBuff; }
		//components funcs
		virtual void OnAttach() override;
		virtual void Bind() override;
		virtual void OnUpdate() override;
		virtual void OnDetach() override;

	protected:
		std::shared_ptr<LightCB> m_cBuff;
	};
	class DirecionalLight : public Light
	{
	public:
		struct Data
		{
			alignas(16)Vec3f color;
			alignas(16)Vec3f direction;
		};
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
	class LightCB : public TConstantBuffer<LightCB>
	{

	private:
	};
}

