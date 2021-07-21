#pragma once
#include "iBindable.h"
#include "enexception.h"
#include "Entity.h"
#include "end3d11.h"
#include "enBuffer.h"
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
		std::string GetKey(std::string modelname) 
		{
			return typeid(VertexBuffer<T>).name() + '#' + modelname;
		}
		void Bind() override
		{
			pContext->IASetVertexBuffers(0, 1, pBuffer.GetAddressOf(), &stride, &offset);
		}

	private:
		
	};

	struct VertexBuff: public Buffer, public Bindable
	{
		VertexBuff(enBuffer& enbuf) : Buffer(enbuf.count(), enbuf.layout_size())
		{
			D3D11_BUFFER_DESC pdesc{};
			pdesc.ByteWidth = stride * count;
			pdesc.Usage = D3D11_USAGE_DEFAULT;
			pdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			pdesc.CPUAccessFlags = 0;
			pdesc.MiscFlags = 0;
			pdesc.StructureByteStride = stride;
			D3D11_SUBRESOURCE_DATA subRes{ enbuf.data().data(),0,0 };
			errchk::hres(pDevice->CreateBuffer(&pdesc, &subRes, &pBuffer), EnExParam);
		}
		void Bind() override
		{
			pContext->IASetVertexBuffers(0, 1, pBuffer.GetAddressOf(), &stride, &offset);
		}

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
		std::string GetKey(std::string modelname)
		{
			return typeid(IndexBuffer).name() +'#' + modelname;
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
		using Data = std::vector<DirectX::XMMATRIX>;
		Transform(Data data, Entity& parent, uint8_t slot = 0) : ConstantBuffer<DirectX::XMMATRIX>(data, parent, slot) {};
		std::string GetKey()
		{
			return typeid(Transform).name();
		}
		void Bind() override
		{
			Update();
			pContext->VSSetConstantBuffers(0, 1, pBuffer.GetAddressOf());
		}
		void Update() override;
	};
}
