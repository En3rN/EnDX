#pragma once
#include "Buffer.h"
#include "Window.h"
#include "Camera.h"


namespace En3rN::DX
{
	void Transform::Update()
	{
		auto cam = Camera::GetActiveCamera();
		DirectX::XMMATRIX data =
			DirectX::XMMatrixTranspose(
				parent.GetViewMatrix() *
				cam.GetViewMatrix() *
				cam.GetProjectionMatrix()
			);
		D3D11_MAPPED_SUBRESOURCE msr{};
		errchk::hres(pContext->Map(pBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr), EnExParam);
		memcpy(msr.pData, &data, sizeof(data));
		pContext->Unmap(pBuffer.Get(), 0u);
	}
	
}
