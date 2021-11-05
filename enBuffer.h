#pragma once
//#include "end3d11.h"
//#include <d3d11shader.h>
#include "vec.h"
#include "iHandle.h"
#include"assimp\vector3.h"
#include<algorithm>
#include<type_traits>
#include<typeindex>
#include<typeinfo>
#include<vector>
#include<assert.h>
#include<string>
namespace En3rN::DX
{
	class enBuffer
	{
	public:
		enBuffer() = default;
		using Data = std::vector<uint8_t>;
		using Indecies = std::vector<uint16_t>;
		class Element
		{
		public:
			using Container = std::vector<Element>;

			friend class enBuffer;
			Element() = default;
			Element(const Element& other) = default;
			Element(Element&& other) noexcept= default;
			Element& operator = (const Element& other) = default;
			Element& operator = (Element&& other) noexcept = default;
			virtual ~Element() = default;

			const std::string&		semantic()		{ return m_semantic;}
			size_t					stride()		{ return m_stride; }
			size_t					count()			{ return m_count; }
			size_t					size()			{ return m_size; }
			size_t					hash()			{ return m_hash; }
			uint8_t*				data()			{ return m_data.data(); }
			const uint8_t*			data()	const	{ return m_data.data(); }
			
		protected:
			std::string		m_semantic;
			size_t			m_stride;	//size of stride
			size_t			m_count;	//stridecount
			size_t			m_size;		//total size
			size_t			m_hash;		//typeid.hash
			enBuffer::Data	m_data;		//data
		};
		template<typename T, std::enable_if_t<std::is_standard_layout_v<T>, bool> = true>
		enBuffer& operator << (const T& data) 
		{
			add_element(data);
			return *this;
		}
		template<typename T, std::enable_if_t<std::is_standard_layout_v<T>,bool> = true>
		void	add_element(const T& data, const std::string& semantic, size_t stride = 0, size_t count = 1) // adds element to layout -- pass in stride and count if full buffer is passed 
		{		
			
			m_elements.emplace_back(Element());
			auto& e = m_elements.back();
			e.m_semantic = semantic;
			stride == 0 ? e.m_stride = sizeof(T) : e.m_stride = stride;
			e.m_count = count;
			e.m_size = stride * count;
			e.m_hash = typeid(T).hash_code();
			e.m_data.resize(e.m_size);
			if constexpr (std::is_array_v<T[]>)
			{
				for (auto i = 0; i < count; ++i)
					memcpy(&e.m_data[i * stride], &data[i], stride);
			}
			else 
				memcpy(&e.m_data[0], &data, e.m_size);
		}
		void	add_element (const Element* e , size_t count)
		{
			auto element = e[0];
			element.m_count = count;
			element.m_size = count * element.m_stride;
			element.m_data.resize(element.m_size);
			for (auto i = 1; i < count; ++i)
			{
				memcpy(element.m_data.data() + i * element.m_stride,
					e[i].data(), element.m_stride);
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
		void				create_buffer(); // creates buffer from elements -- locks the layout
		void				create_buffer(std::vector<std::string>& semantics); // creates buffer from requested elements -- locks the layout
		size_t				size(); // size of stored data
		size_t				count(); //number of stored manual structs
		size_t				element_count(); //number of elements in layout
		size_t				layout_size(); //size of layout in bytes
		uint8_t*			data();
		Indecies			indecies(uint32_t verteciesPerFace); //
		
	protected:
		Data				m_buffer;
		Element::Container	m_elements;
		bool				m_locked = false;
	};
	class VertexElement // elements set up for vertexbuffer
	{
	public:
		struct Pos : public enBuffer::Element
		{
			Pos(aiVector3D vec) : Pos(vec.x,vec.y,vec.z){}
			Pos(float x, float y, float z)
			{
				Vec3f data(x, y, z);
				m_semantic = "Position";
				m_count = 1;
				m_hash = typeid(this).hash_code();
				m_stride = sizeof(data);
				m_size = m_stride * m_count;
				m_data.resize(m_size);
				memcpy(m_data.data(), &data, m_size);
			}
		};
		struct Color : public enBuffer::Element
		{
			Color(float r, float g, float b, float a)
			{
				Vec4u8 data(
					std::clamp<uint8_t>((255 * r), 0, 255),
					std::clamp<uint8_t>((255 * g), 0, 255),
					std::clamp<uint8_t>((255 * b), 0, 255),
					std::clamp<uint8_t>((255 * a), 0, 255));
				m_semantic = "Color";
				m_count = 1;
				m_hash = typeid(this).hash_code();
				m_stride = sizeof(data);
				m_size = m_stride * m_count;
				m_data.resize(m_size);
				memcpy(m_data.data(), &data, m_size);
			};
			Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
			{
				Vec4u8 data(r,g,b,a);
				m_semantic = "Color";
				m_count = 1;
				m_hash = typeid(this).hash_code();
				m_stride = sizeof(data);
				m_size = m_stride * m_count;
				m_data.resize(m_size);
				memcpy(m_data.data(), &data, m_size);
			};
			
		};
		struct TexCoord : public enBuffer::Element
		{
			TexCoord(float u, float v)
			{
				Vec2f data(u,v);
				m_semantic = "TexCoord";
				m_count = 1;
				m_hash = typeid(this).hash_code();
				m_stride = sizeof(data);
				m_size = m_stride * m_count;
				m_data.resize(m_size);
				memcpy(m_data.data(), &data, m_size);
			};
			
		};
		struct TexCoord3d : public enBuffer::Element
		{
			
		};
		struct Normal : public enBuffer::Element
		{
			Normal(float x, float y, float z)
			{
				Vec3f data(x, y, z);
				m_semantic = "Normal";
				m_count = 1;
				m_hash = typeid(this).hash_code();
				m_stride = sizeof(data);
				m_size = m_stride * m_count;
				m_data.resize(m_size);
				memcpy(m_data.data(), &data, m_size);
			}
			
		};
		struct Tangent : public enBuffer::Element
		{
			
		};
		struct BiTangent : public enBuffer::Element
		{
			
		};
	};
	
}


