#include "Pass.h"
#include "Mesh.h"

namespace En3rN::DX
{
	void Pass::AddJob(const Job& job)
	{
		auto sameVBuff = [](Job& j1, Job& j2) { return j1.GetMesh().GetVertexBuffer() == j2.GetMesh().GetVertexBuffer(); };
		m_jobs.push_back(job);
		//std::sort(begin(m_jobs), end(m_jobs), sameVBuff);
		return;
	}
	std::string Pass::GetPassName()
	{
		switch (m_name)
		{
		case En3rN::DX::Pass::Name::Lambertian:
			return std::string("Lambertian");
		case En3rN::DX::Pass::Name::Skybox:
			return std::string("Skybox");
		default:
			return std::string("Invalid Pass");
			break;
		}
		
	}
}