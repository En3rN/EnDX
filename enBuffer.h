#pragma once
#include<type_traits>
#include<typeindex>
#include<typeinfo>
#include<vector>
#include<assert.h>
#include "end3d11.h"
#include "vec.h"
#include "iHandle.h"
namespace En3rN::DX
{
	class enBuffer
	{
	public:
		using Data = std::vector<uint8_t>;
		class Element  
		{
		public:
			using Container = std::vector<Element>;	
			friend class enBuffer;
			Element() = default;
			Element(const Element & other) = default;
			Element(Element && other) = default;
			Element& operator = (const Element& other) = default;
			virtual ~Element() = default;
			size_t stride()			{ return m_stride; }
			size_t count()			{ return m_count; }
			size_t size()			{ return m_size; }
			size_t hash()			{ return m_hash; }
			enBuffer::Data& data()	{ return m_data; }
			virtual D3D11_INPUT_ELEMENT_DESC get_desc() { return {m_ied}; }
		protected:
			size_t			m_stride;	//size of stride
			size_t			m_count;	//stridecount
			size_t			m_size;		//total size
			size_t			m_hash;		//typeid.hash
			enBuffer::Data	m_data;		//data
			D3D11_INPUT_ELEMENT_DESC m_ied;
		};

		template<typename T,
		std::enable_if<!std::is_base_of<Element, T>::value>::type>
		void	add_element(const T& data, size_t stride = 0, size_t count = 1) // adds element to layout -- pass in stride and count if full buffer is passed 
		{
			Element e{};
			stride == 0 ? e.m_stride = sizeof(data) / count : e.m_stride = stride;
			e.m_count = count;
			e.m_size = stride * count;
			e.m_hash = typeid(T).hash_code();
			e.m_data.resize(e.m_size);
			memcpy(&e.m_data[0], &data, e.m_size);
			m_elements.push_back(e);
		}
		void	add_element(Element e[], size_t count = 1)
		{
			auto element = e[0];
			element.m_count = count;
			element.m_size = count * element.m_stride;
			element.m_data.resize(element.m_size);
			for (auto i = 1; i < count; ++i)
			{
				memcpy(element.m_data.data() + i * element.m_stride,
					e[i].m_data.data(), element.m_stride);
			}
			m_elements.push_back(element);
		}
		template <typename T>
		void	push_back(const T& data) // locks layout -- data size must match element_size
		{
			m_locked = true;
			size_t offset = m_buffer.size();
			m_buffer.resize(offset + sizeof(data));
			memcpy(&m_buffer[offset], &data, sizeof(data));
		}
		virtual void	create_buffer(); // creates buffer from elements -- locks the layout
		size_t	size(); // size of stored data
		size_t	count(); //number of stored manual structs
		size_t  element_count(); //number of elements in layout
		size_t  layout_size(); //size of layout in bytes
		Data&	data() { return m_buffer; }
		std::vector<D3D11_INPUT_ELEMENT_DESC> input_element_desc();
	protected:
		Data				m_buffer;
		Element::Container	m_elements;
		bool				m_locked = false;
	};
	struct VertexElement
	{
		struct Pos : public enBuffer::Element
		{
			Pos(float x, float y, float z)
			{
				Vec3f data(x, y, z);
				m_count = 1;
				m_hash = typeid(this).hash_code();
				m_stride = sizeof(data);
				m_size = m_stride * m_count;
				m_ied = get_desc();
				m_data.resize(m_size);
				memcpy(m_data.data(), &data, m_size);
			}
			D3D11_INPUT_ELEMENT_DESC get_desc() override
			{
				return { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			}
		};
		struct Color : public enBuffer::Element
		{
			D3D11_INPUT_ELEMENT_DESC get_desc() override
			{
				return  { "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			}
		};
		struct TexCoord : public enBuffer::Element
		{
			TexCoord(float u, float v)
			{
				Vec2f data(u,v);
				m_count = 1;
				m_hash = typeid(this).hash_code();
				m_stride = sizeof(data);
				m_size = m_stride * m_count;
				m_ied = get_desc();
				m_data.resize(m_size);
				memcpy(m_data.data(), &data, m_size);
			};
						
			D3D11_INPUT_ELEMENT_DESC get_desc() override
			{
				return  { "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			}
		};
		struct TexCoord3d : public enBuffer::Element
		{
			D3D11_INPUT_ELEMENT_DESC get_desc() override
			{
				return { "TexCoord3d", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			}
		};
		struct Normal : public enBuffer::Element
		{
			D3D11_INPUT_ELEMENT_DESC get_desc() override
			{
				return { "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			}
		};
		struct Tangent : public enBuffer::Element
		{
			D3D11_INPUT_ELEMENT_DESC get_desc() override
			{
				return { "Tangent", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			}
		};
		struct BiTangent : public enBuffer::Element
		{
			D3D11_INPUT_ELEMENT_DESC get_desc() override
			{
				return { "BiTangent", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			}
		};
	};
	
}


