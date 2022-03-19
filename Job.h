#pragma once

//#include "Mesh.h"
//#include "Material.h"
////#include "Step.h"

#include "Transform.h"
#include <vector>

namespace En3rN::DX
{
	class Mesh;
	class Material;
	class Step;
	class Node;
	class Job
	{
	public:
		using Container = std::vector<Job>;
		Job(const Mesh& mesh,const Material& material, const Transform::Matrix& transform, const Step& step) :  
			m_mesh(&mesh), m_material(&material),m_transform(&transform), m_step(&step) {}
		/*Job(Job&& other) noexcept = default;
		Job(Job& other) = default;
		Job& operator = (Job && other) noexcept = default;
		Job& operator = (Job & other) = default;
		~Job() = default;*/
		const Mesh&					GetMesh()		const { return *m_mesh; }
		const Material&				GetMaterial()	const { return *m_material;}
		const Transform::Matrix&	GetTransform()	const { return *m_transform; }
		const Step&					GetStep()		const { return *m_step; }
		
	private:
		const Mesh*					m_mesh;
		const Material*				m_material;
		const Transform::Matrix*	m_transform;
		const Step*					m_step;
	};
}
