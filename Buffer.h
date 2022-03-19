#pragma once
#include "iBindable.h"
#include "BindableManager.h"
#include "enexception.h"
#include "end3d11.h"
#include "enBuffer.h"
#include <stdint.h>
#include <vector>
#include <string>

namespace En3rN::DX
{
	class Job;
	class Buffer
	{
	public:
		Buffer(const Buffer& other) = default;
		Buffer(Buffer&& other) noexcept = default;
		Buffer& operator = (const Buffer& other) = default;
		Buffer& operator = (Buffer&& other) noexcept = default;
		virtual ~Buffer() = default;
		
		UINT GetCount() { return count;}
	protected:
		Buffer(UINT count, UINT stride, UINT slot =0,UINT offset=0) : count(count), stride(stride), slot(slot),offset(offset) {};

		UINT count;
		UINT stride;
		UINT slot;
		UINT offset;
		ComPtr<ID3D11Buffer> pBuffer{};
	};

	template <typename T>
	class VertexBufferT : public Buffer , public Bindable
	{
	public:
		VertexBufferT(std::vector<T>& data) : Buffer(data.size(),sizeof(T))
		{
			D3D11_BUFFER_DESC pdesc{};
			pdesc.ByteWidth = stride * count;
			pdesc.Usage = D3D11_USAGE_DEFAULT;
			pdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			pdesc.CPUAccessFlags = 0;
			pdesc.MiscFlags = 0;
			pdesc.StructureByteStride = stride;
			D3D11_SUBRESOURCE_DATA subRes{ data.data(),0,0 };
			errchk::hres(pDevice->CreateBuffer(&pdesc, &subRes, &pBuffer));
		}
		void Bind() override
		{
			pContext->IASetVertexBuffers(0, 1, pBuffer.GetAddressOf(), &stride, &offset);
		}
	private:
	};

	class VertexBuffer : public Buffer , public Bindable
	{
	public:
		using handle = std::shared_ptr<VertexBuffer>;
		VertexBuffer(enBuffer enbuf, std::string modelName="") : Buffer(enbuf.count(), enbuf.layout_size())
		{
			D3D11_BUFFER_DESC pdesc{};
			pdesc.ByteWidth = stride * count;
			pdesc.Usage = D3D11_USAGE_DEFAULT;
			pdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			pdesc.CPUAccessFlags = 0;
			pdesc.MiscFlags = 0;
			pdesc.StructureByteStride = stride;
			D3D11_SUBRESOURCE_DATA subRes{ enbuf.data(),0,0 };
			errchk::hres(pDevice->CreateBuffer(&pdesc, &subRes, &pBuffer));
		}
		VertexBuffer(const VertexBuffer& other) = default;
		VertexBuffer(VertexBuffer&& other) = default;
		~VertexBuffer() = default;

		void Bind() override
		{
			pContext->IASetVertexBuffers(0, 1, pBuffer.GetAddressOf(), &stride, &offset);
		}
	};

	
	class IndexBuffer : public Buffer , public Bindable
	{
	public:
		using handle = std::shared_ptr<IndexBuffer>;
		IndexBuffer(std::vector<uint16_t> data,  std::string modelName="") :Buffer(data.size(), sizeof(uint16_t))
		{
			D3D11_BUFFER_DESC pdesc{};
			pdesc.ByteWidth = stride * count;
			pdesc.Usage = D3D11_USAGE_DEFAULT;
			pdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			pdesc.CPUAccessFlags = 0;
			pdesc.MiscFlags = 0;
			pdesc.StructureByteStride = stride;
			D3D11_SUBRESOURCE_DATA subRes{ data.data(),0,0 };
			errchk::hres(pDevice->CreateBuffer(&pdesc, &subRes, &pBuffer));
		}
		IndexBuffer(const IndexBuffer& other) = default;
		IndexBuffer(IndexBuffer&& other) = default;
		~IndexBuffer() = default;
		void Bind() override
		{
			pContext->IASetIndexBuffer(pBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);
		}
	protected:
	};
	
	template <typename T>
	class TConstantBuffer :  public Buffer, public Bindable
	{
	public:
		using Data = T;
		TConstantBuffer(TConstantBuffer&& other) = default;
		virtual ~TConstantBuffer() = default;
	protected:		
		TConstantBuffer(UINT count, uint8_t slot) : Buffer(count, sizeof(T), slot)
		{			
			D3D11_BUFFER_DESC pdesc{};
			pdesc.ByteWidth = stride*count;
			pdesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
			pdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			pdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			pdesc.MiscFlags = 0;
			pdesc.StructureByteStride = stride;
			errchk::hres(pDevice->CreateBuffer(&pdesc, nullptr, &pBuffer));
		};
	protected:
		void Update(const Data& data)
		{
			D3D11_MAPPED_SUBRESOURCE msr{&data,0,0};
			errchk::hres(pContext->Map(pBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0u, &msr));
			//memcpy(msr.pData, &data, sizeof(data));
			pContext->Unmap(pBuffer.Get(), 0);
		}
	};	
	
	template <typename T>
	class VSConstantBuffer  : public Buffer, public Bindable
	{
	public:
		VSConstantBuffer(UINT count, UINT slot) :
			Buffer(count, sizeof(T), slot)
		{
			D3D11_BUFFER_DESC pdesc{};
			pdesc.ByteWidth = stride * count;
			pdesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
			pdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			pdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			pdesc.MiscFlags = 0;
			pdesc.StructureByteStride = stride;
			errchk::hres(pDevice->CreateBuffer(&pdesc, NULL, &pBuffer));
		};	
		~VSConstantBuffer() = default;
		void Bind() override
		{
			pContext->VSSetConstantBuffers(slot, 1, pBuffer.GetAddressOf());
		}
		void Update(const T& data)
		{
			D3D11_MAPPED_SUBRESOURCE msr{};
			errchk::hres(pContext->Map(pBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0u, &msr));
			memcpy(msr.pData, &data, sizeof(data)*count);
			pContext->Unmap(pBuffer.Get(), 0);
		}
		void Update(const Job& e) override;
	private:
	};
	// template on datastructure
	template <typename T>
	class PSConstantBuffer : public Buffer, public Bindable
	{
	public:
		PSConstantBuffer(UINT count, UINT slot) :
			Buffer(count,sizeof(T),slot)
		{
			D3D11_BUFFER_DESC pdesc{};
			pdesc.ByteWidth = stride * count;
			pdesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
			pdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			pdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			pdesc.MiscFlags = 0;
			pdesc.StructureByteStride = stride;
			errchk::hres(pDevice->CreateBuffer(&pdesc, NULL, &pBuffer));
		};
		~PSConstantBuffer() = default;

		void Bind() override
		{
			pContext->PSSetConstantBuffers(slot, 1, pBuffer.GetAddressOf());
		}
		void Update(const T& data)
		{
			D3D11_MAPPED_SUBRESOURCE msr{};
			errchk::hres(pContext->Map(pBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0u, &msr));
			memcpy(msr.pData, &data, sizeof(data));
			pContext->Unmap(pBuffer.Get(), 0);
		}
		void Update(const Job& e) override;
	private:
	};
	
	

}
