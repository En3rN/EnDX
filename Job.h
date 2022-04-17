#pragma once
#include "Transform.h"
#include <vector>

namespace En3rN::DX
{
	class Mesh;
	class Material;	
	class Node;
	class Step;
	class RenderJob
	{
	public:
		using Container = std::vector<RenderJob>;
		RenderJob(const Mesh& mesh,const Material& material, const Step& step) :  
			m_mesh(&mesh), m_material(&material), m_step(&step) {}
		const Mesh&					GetMesh()		const { return *m_mesh; }
		const Material&				GetMaterial()	const { return *m_material;}
		const Step&					GetStep()		const { return *m_step; }
		//const entt::handle&			GetEntity()		const { return m_entity; } //Usikker om dett er veiem å gå;
		bool operator == (RenderJob& job) {
			bool result = true;
			result *= m_mesh == job.m_mesh;
			result *= m_material == m_material;
			return result;
		}
		
	private:
		const Mesh*					m_mesh;
		const Material* m_material;
		const Step* m_step;
		//const entt::handle			m_entity;
	};
}
