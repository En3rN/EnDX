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
		Buffer(UINT count, UINT stride, std::string tag, UINT slot =0,UINT offset=0 ) : count(count), stride(stride), tag(tag),slot(slot),offset(offset) {};
		std::string tag;
		UINT count;
		UINT stride;
		UINT slot;
		UINT offset;
		
		ComPtr<ID3D11Buffer> pBuffer{};
	};

	template <typename Element>
	class VertexBufferT : public Buffer , public Bindable
	{
	public:
		VertexBufferT(std::vector<Element>& data) : Buffer(data.size(),sizeof(Element))
		{
			D3D11_BUFFER_DESC pdesc{};
			pdesc.ByteWidth = stride * count;
			pdesc.Usage = D3D11_USAGE_DEFAULT;
			pdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			pdesc.CPUAccessFlags = 0;
			pdesc.MiscFlags = 0;
			pdesc.StructureByteStride = stride;
			D3D11_SUBRESOURCE_DATA subRes{ data.data(),0,0 };
			errchk::hres(GetDevice()->CreateBuffer(&pdesc, &subRes, &pBuffer));
		}
		void Bind() override
		{
			GetContext()->IASetVertexBuffers(0, 1, pBuffer.GetAddressOf(), &stride, &offset);
		}
	private:
	};

	class VertexBuffer : public Buffer , public Bindable
	{
	public:
		using handle = std::shared_ptr<VertexBuffer>;
		VertexBuffer(enBuffer enbuf, std::string modelName="") : Buffer(enbuf.count(), enbuf.layout_size(),modelName)
		{
			D3D11_BUFFER_DESC pdesc{};
			pdesc.ByteWidth = stride * count;
			pdesc.Usage = D3D11_USAGE_DEFAULT;
			pdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			pdesc.CPUAccessFlags = 0;
			pdesc.MiscFlags = 0;
			pdesc.StructureByteStride = stride;
			D3D11_SUBRESOURCE_DATA subRes{ enbuf.data(),0,0 };
			errchk::hres(GetDevice()->CreateBuffer(&pdesc, &subRes, &pBuffer));
		}
		VertexBuffer(const VertexBuffer& other) = default;
		VertexBuffer(VertexBuffer&& other) = default;
		~VertexBuffer() = default;

		void Bind() override
		{
			GetContext()->IASetVertexBuffers(0, 1, pBuffer.GetAddressOf(), &stride, &offset);
		}
	};

	
	class IndexBuffer : public Buffer , public Bindable
	{
	public:
		using handle = std::shared_ptr<IndexBuffer>;
		IndexBuffer(std::vector<uint16_t> data,  std::string modelName="") :Buffer((UINT)data.size(), sizeof(uint16_t),modelName)
		{
			D3D11_BUFFER_DESC pdesc{};
			pdesc.ByteWidth = stride * count;
			pdesc.Usage = D3D11_USAGE_DEFAULT;
			pdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			pdesc.CPUAccessFlags = 0;
			pdesc.MiscFlags = 0;
			pdesc.StructureByteStride = stride;
			D3D11_SUBRESOURCE_DATA subRes{ data.data(),0,0 };
			errchk::hres(GetDevice()->CreateBuffer(&pdesc, &subRes, &pBuffer));
		}
		IndexBuffer(const IndexBuffer& other) = default;
		IndexBuffer(IndexBuffer&& other) = default;
		~IndexBuffer() = default;
		void Bind() override
		{
			GetContext()->IASetIndexBuffer(pBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);
		}
	protected:
	};
	
	template <typename Element>
	class TConstantBuffer :  public Buffer, public Bindable
	{
	public:
		using Data = Element;
		TConstantBuffer(TConstantBuffer&& other) = default;
		virtual ~TConstantBuffer() = default;
	protected:		
		TConstantBuffer(UINT count, uint8_t slot) : Buffer(count, sizeof(Element), slot)
		{			
			D3D11_BUFFER_DESC pdesc{};
			pdesc.ByteWidth = stride*count;
			pdesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
			pdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			pdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			pdesc.MiscFlags = 0;
			pdesc.StructureByteStride = stride;
			errchk::hres(GetDevice()->CreateBuffer(&pdesc, nullptr, &pBuffer));
		};
	protected:
		void Update(const Data& data)
		{
			D3D11_MAPPED_SUBRESOURCE msr{&data,0,0};
			errchk::hres(GetContext()->Map(pBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0u, &msr));
			//memcpy(msr.pData, &data, sizeof(data));
			GetContext()->Unmap(pBuffer.Get(), 0);
		}
	};	
	
	template <typename Element>
	class VSConstantBuffer  : public Buffer, public Bindable
	{
	public:
		VSConstantBuffer() = delete;
		VSConstantBuffer(UINT count, UINT slot, std::string tag = "") :
			Buffer(count, sizeof(Element),tag,slot)
		{
			D3D11_BUFFER_DESC pdesc{};
			pdesc.ByteWidth = stride * count;
			pdesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
			pdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			pdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			pdesc.MiscFlags = 0;
			pdesc.StructureByteStride = stride;			
			errchk::hres(GetDevice()->CreateBuffer(&pdesc, NULL, &pBuffer));
		};
		VSConstantBuffer(Element data, UINT count, UINT slot, std::string tag =""):
			Buffer(count, sizeof(Element), tag, slot)
		{
			D3D11_BUFFER_DESC pdesc{};
			pdesc.ByteWidth = stride * count;
			pdesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
			pdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			pdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			pdesc.MiscFlags = 0;
			pdesc.StructureByteStride = stride;
			D3D11_SUBRESOURCE_DATA subres{ &data,0,0 };
			errchk::hres(GetDevice()->CreateBuffer(&pdesc, &subres, &pBuffer));
		}
		virtual ~VSConstantBuffer() = default;
		void Bind() override
		{
			GetContext()->VSSetConstantBuffers(slot, 1, pBuffer.GetAddressOf());
		}
		virtual void Update(const Element& data)
		{
			D3D11_MAPPED_SUBRESOURCE msr{};
			errchk::hres(GetContext()->Map(pBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0u, &msr));
			memcpy(msr.pData, &data, sizeof(data)*count);
			GetContext()->Unmap(pBuffer.Get(), 0);
		}
		//void Update(const RenderJob& e) override;
	private:
	};
	// template on datastructure
	template <typename Element>
	class PSConstantBuffer : public Buffer, public Bindable
	{
	public:
		PSConstantBuffer(UINT count, UINT slot, std::string tag= "") :
			Buffer(count,sizeof(Element),tag,slot)
		{
			D3D11_BUFFER_DESC pdesc{};
			pdesc.ByteWidth = stride * count;
			pdesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
			pdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			pdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			pdesc.MiscFlags = 0;
			pdesc.StructureByteStride = stride;
			errchk::hres(GetDevice()->CreateBuffer(&pdesc, NULL, &pBuffer));
		};
		PSConstantBuffer(Element data, UINT count, UINT slot, std::string tag = "") :
			Buffer(count, sizeof(Element), tag, slot)
		{
			D3D11_BUFFER_DESC pdesc{};
			pdesc.ByteWidth = stride * count;
			pdesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
			pdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			pdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			pdesc.MiscFlags = 0;
			pdesc.StructureByteStride = stride;
			D3D11_SUBRESOURCE_DATA subres{ &data,0,0 };

			errchk::hres(GetDevice()->CreateBuffer(&pdesc, &subres, &pBuffer));
		};
		~PSConstantBuffer() = default;

		void Bind() override
		{
			GetContext()->PSSetConstantBuffers(slot, 1, pBuffer.GetAddressOf());
		}
		virtual void Update(const Element& data)
		{
			D3D11_MAPPED_SUBRESOURCE msr{};
			errchk::hres(GetContext()->Map(pBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0u, &msr));
			memcpy(msr.pData, &data, sizeof(data));
			GetContext()->Unmap(pBuffer.Get(), 0);
		}
	private:
	};
	
	

}
