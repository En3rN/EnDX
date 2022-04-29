#pragma once
#include "Graphics.h"
namespace En3rN::DX
{
	class Device
	{
	public:
		Device(const ComPtr<ID3D11Device>& device) : m_device(device) {};
		auto CreateBuffer() {
			D3D11_BUFFER_DESC desc{};
			D3D11_SUBRESOURCE_DATA data{};
			ComPtr<ID3D11Buffer> buffer;

			m_device->CreateBuffer(&desc, &data, &buffer);
			return buffer;
		}
		
	private:
		ComPtr<ID3D11Device> m_device;
	};
}

