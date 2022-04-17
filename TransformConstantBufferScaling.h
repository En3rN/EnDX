#pragma once
#include "Transform.h"
namespace En3rN::DX
{
	class TransformConstantBufferScaling : public Transform::ConstantBuffer
	{
	public:		
		TransformConstantBufferScaling(float scale) : 
			Transform::ConstantBuffer(1,1,"outlineScale"), m_scale(scale) {}
		~TransformConstantBufferScaling() = default;
		void Update(const Transform::Matrix& mat) override
		{
			auto data = mat * DirectX::XMMatrixScaling(m_scale, m_scale, m_scale);
			//data = DirectX::XMMatrixTranspose(data);

			D3D11_MAPPED_SUBRESOURCE msr{};
			errchk::hres(pContext->Map(pBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0u, &msr));
			memcpy(msr.pData, &data, sizeof(data) * count);
			pContext->Unmap(pBuffer.Get(), 0);
		}

	private:

		float m_scale;
	};
}
