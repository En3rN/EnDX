#pragma once
#include "iBindable.h"
#include "enexception.h"
#include "Entity.h"
#include "end3d11.h"
#include <stdint.h>
#include <vector>

namespace En3rN::DX
{
	class Buffer
	{
	public:
		Buffer(Buffer&& other) noexcept = default;
		virtual ~Buffer() = default;
	protected:
		Buffer(UINT count, UINT stride, UINT offset = 0) : count(count), stride(stride), offset(offset) {};
		ComPtr<ID3D11Buffer> pBuffer{};
		UINT count;
		UINT stride;
		UINT offset;
	};

	template <typename T>
	class VertexBuffer : public Buffer, public Bindable
	{
	public:
		VertexBuffer(std::vector<T>& data) : Buffer(data.size(),(sizeof(T)))
		{
			D3D11_BUFFER_DESC pdesc{};
			pdesc.ByteWidth = stride * count;
			pdesc.Usage = D3D11_USAGE_DEFAULT;
			pdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			pdesc.CPUAccessFlags = 0;
			pdesc.MiscFlags = 0;
			pdesc.StructureByteStride = stride;
			D3D11_SUBRESOURCE_DATA subRes{ data.data(),0,0 };
			errchk::hres(pDevice->CreateBuffer(&pdesc, &subRes, &pBuffer),EnExParam);
		}
		void Bind() override
		{
			pContext->IASetVertexBuffers(0, 1, pBuffer.GetAddressOf(), &stride, &offset);
		}

	private:
		
	};

	
	class IndexBuffer : public Buffer, public Bindable
	{
	public:
		IndexBuffer(std::vector<uint16_t>& data) :Buffer(data.size(), sizeof(uint16_t))
		{
			D3D11_BUFFER_DESC pdesc{};
			pdesc.ByteWidth = stride * count;
			pdesc.Usage = D3D11_USAGE_DEFAULT;
			pdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			pdesc.CPUAccessFlags = 0;
			pdesc.MiscFlags = 0;
			pdesc.StructureByteStride = stride;
			D3D11_SUBRESOURCE_DATA subRes{ data.data(),0,0 };
			errchk::hres(pDevice->CreateBuffer(&pdesc, &subRes, &pBuffer), EnExParam);
		}
		void Bind() override
		{
			pContext->IASetIndexBuffer(pBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);
		}
	protected:
		
	};
	
	template <typename T>
	class ConstantBuffer :  public Buffer, public DynamicBindable
	{
	public:
		ConstantBuffer(ConstantBuffer&& other) = default;
		virtual ~ConstantBuffer() = default;
	protected:		
		ConstantBuffer(std::vector<T>& data, Entity& parent, uint8_t slot) : parent(parent),  slot(slot),Buffer(data.size(), sizeof(T))
		{
			D3D11_BUFFER_DESC pdesc{};
			pdesc.ByteWidth = stride*count;
			pdesc.Usage = D3D11_USAGE_DYNAMIC;
			pdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			pdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			pdesc.MiscFlags = 0;
			pdesc.StructureByteStride = stride;
			D3D11_SUBRESOURCE_DATA subRes = { &data,0,0 };
			errchk::hres(pDevice->CreateBuffer(&pdesc, &subRes, &pBuffer), EnExParam);
		};
	protected:
		Entity& parent;
		UINT slot;
	};
	class Transform : public ConstantBuffer<DirectX::XMMATRIX>
	{
	public:
		using Matrix = std::vector<DirectX::XMMATRIX>;
		Transform(std::vector<DirectX::XMMATRIX> data, Entity& parent, uint8_t slot = 0) : ConstantBuffer<DirectX::XMMATRIX>(data, parent, slot) {};
		void Bind() override
		{
			Update();
			pContext->VSSetConstantBuffers(0, 1, pBuffer.GetAddressOf());
		}
		void Update() override;
	};
}
