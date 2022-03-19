#pragma once
#include "Buffer.h"
#include "Job.h"
#include "Material.h"
#include "Transform.h"
#include "Node.h"


namespace En3rN::DX
{
	void PSConstantBuffer<Material::Data>::Update(const Job& job)
	{
		Update(job.GetMaterial().GetData());
		return;
	}
	
	void VSConstantBuffer<Transform::Matrix>::Update(const Job& job)
	{
		return Update(DirectX::XMMatrixTranspose(job.GetTransform()));
	}
	
}

