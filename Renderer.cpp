#include "Renderer.h"
#include "Mesh.h"
#include "Step.h"
#include "Shader.h"
#include "Transform.h"
#include "InputLayout.h"
#include <algorithm>

namespace En3rN::DX
{

	Pass& En3rN::DX::Renderer::GetPass(Pass::Name name)
	{
		return m_passes[(int)name];
	}

	void En3rN::DX::Renderer::AddPass(Pass& pass)
	{
		m_passes.push_back(pass);
		std::sort(m_passes.begin(), m_passes.end(), std::less());
	}

	bool Renderer::Bound(const Bindable::Base::handle& bindable)
	{
		auto result = std::find(begin(m_currentBindings), end(m_currentBindings), bindable);
		if (result != end(m_currentBindings))
			return true;
		std::replace(begin(m_currentBindings), end(m_currentBindings), *result, bindable);
		bindable->Bind();

		return false;
	}

	void En3rN::DX::Renderer::Draw()
	{
		auto execute = [&](Pass& pass) {
			for (auto& job : pass.GetJobs()){
				auto& mesh = job.GetMesh();
				auto& material = job.GetMaterial();
				mesh.Bind();
				material.Bind();
				job.GetTransform();
				for (auto& step : job.GetStep().GetBindables()){
					step.get()->Update(mesh);
					step.get()->Bind();
				}
				m_context->DrawIndexed(mesh.GetIndexBuffer()->GetCount(), 0, 0);
			};
			pass.GetJobs().clear();
		};
		std::for_each(m_passes.begin(), m_passes.end(), execute);
	}
}