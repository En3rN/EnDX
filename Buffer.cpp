#pragma once
#include "Buffer.h"
#include "Window.h"
#include "Camera.h"


namespace En3rN::DX
{
	/*void VSConstantBuffer<DirectX::XMMATRIX>::Update(const Entity& e)
	{
		{
			auto& cam = Camera::GetActiveCamera();
			DirectX::XMMATRIX data[] = {
				{e.GetViewMatrix() },
				{cam.GetViewMatrix()},
				{cam.GetProjectionMatrix()}
			};
			
			D3D11_MAPPED_SUBRESOURCE msr{};
			errchk::hres(pContext->Map(pBuffer.Get(), slot, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0u, &msr), EnExParam);
			memcpy(msr.pData, &data, stride*count);
			pContext->Unmap(pBuffer.Get(), slot);
		}
	}*/
}

