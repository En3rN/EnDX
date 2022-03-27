#include "Resource.h"

namespace En3rN::DX
{
	D3D11_RESOURCE_DIMENSION Resource::GetDimension()
	{
		D3D11_RESOURCE_DIMENSION d{};
		m_res->GetType(&d);
		return d;
	}
}
