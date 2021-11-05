#pragma once

#include "iBindable.h"
namespace En3rN::DX
{

	class Topology : public Bindable
	{
	public:
		using handle = std::shared_ptr<Topology>;

		Topology(D3D11_PRIMITIVE_TOPOLOGY topology) : m_topology(topology) {};
		void Bind() override
		{
			D3D11_PRIMITIVE_TOPOLOGY current;
			pContext->IAGetPrimitiveTopology(&current);
			if(current!=m_topology)
				pContext->IASetPrimitiveTopology(m_topology);
			return;
		}
	private:
		D3D11_PRIMITIVE_TOPOLOGY m_topology;
	};
}
