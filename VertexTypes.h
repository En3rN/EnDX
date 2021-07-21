#pragma once
#include "vec.h"
#include "end3d11.h"
#include <type_traits>
#include <vector>
#include<memory>

namespace En3rN::DX
{	

	struct VertexTypes
	{
		struct Buffer
		{
			std::vector<uint8_t> vertexBuffer;
			std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc;
		};		
		struct Element
		{
			using Container = std::vector<std::unique_ptr<Element>>;
			virtual size_t Size() = 0;
			virtual void* GetData() =0; 
			virtual D3D11_INPUT_ELEMENT_DESC	GetDesc() = 0;
		};
		struct Pos : public VertexTypes::Element
		{
			Pos(float x, float y, float z) : x(x), y(y), z(z) {};
			using Container = std::vector<Pos>;
			float x, y, z;
			Container container;
			operator uint8_t* () { return *this; };
			virtual size_t Size() { return container.size(); }
			virtual void* GetData() { return container.data(); }
			D3D11_INPUT_ELEMENT_DESC GetDesc() { return { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }; }
		};
		struct Color :public VertexTypes::Element
		{
			Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {};
			using Container = std::vector<Color>;
			uint8_t r, g, b, a;
			Container container;
			operator uint8_t* () { return *this; };
			virtual size_t Size() { return container.size(); }
			virtual void* GetData() { return container.data(); }
			D3D11_INPUT_ELEMENT_DESC GetDesc() { return  { "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }; }
		};
		struct TexCord :public VertexTypes::Element
		{
			TexCord(float u, float v) : u(u), v(v){};
			using Container = std::vector<TexCord>;
			float u, v;
			Container container;
			operator uint8_t* () { return *this; };
			virtual size_t Size() { return container.size(); }
			virtual void* GetData() { return container.data(); }
			D3D11_INPUT_ELEMENT_DESC GetDesc() { return  { "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }; }
		};
		struct TexCord3d :public VertexTypes::Element
		{
			TexCord3d(float u, float v, float w) : u(u), v(v), w(w) {};
			using Container = std::vector<TexCord3d>;
			float u, v, w;
			Container container;
			operator uint8_t* () { return *this; };
			virtual size_t Size() { return container.size(); }
			virtual void* GetData() { return container.data(); }
			D3D11_INPUT_ELEMENT_DESC GetDesc() { return { "TexCord3d", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }; }
		};
		struct Normal : public VertexTypes::Element
		{
			Normal(float x, float y, float z) : x(x), y(y), z(z) {};
			struct Container
			{
				std::vector<Normal> container;
			};
			//using Container = std::vector<Normal>;			
			float x, y, z;
			//Container container;
			operator uint8_t* () { return *this; };
			/*virtual size_t Size() { return container.size(); }
			virtual void* GetData() { return container.data(); }*/
			D3D11_INPUT_ELEMENT_DESC GetDesc() { return { "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }; }
		};
		struct Tangent : VertexTypes::Element
		{
			using Container = std::vector<Tangent>;
			float x, y, z;
			Container container;
			operator uint8_t* () { return *this; };
			virtual size_t Size() { return container.size(); }
			virtual void* GetData() { return container.data(); }
			D3D11_INPUT_ELEMENT_DESC GetDesc() { return { "Tangent", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }; }
		};
		struct BiTangent :public VertexTypes::Element
		{
			using Container = std::vector<BiTangent>;
			float x, y, z;
			Container container;
			operator uint8_t* () { return *this; };
			virtual size_t Size() { return container.size(); }
			virtual void* GetData() { return container.data(); }
			D3D11_INPUT_ELEMENT_DESC GetDesc() { return { "BiTangent", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }; }
		};
		template <typename T>//, std::enable_if<std::is_base_of<Vertex::Element, T>::value ,T>>
		void AddBuffer(std::vector<T>&& buf)
		{
			
			elements.push_back(std::make_unique<T>(buf));

		};
		Buffer GetBuffer()
		{
			Buffer buffer;
			auto vertecies = elements.front().get()->Size();
			
			for (auto& element : elements)
			{
				assert(element->Size() == vertecies);
				buffer.inputElementDesc.push_back(element->GetDesc());
				
			}
			for (auto i = 0; i < vertecies; i++)
			{
				for (auto& element : elements)
					buffer.vertexBuffer.emplace_back(element->GetData());
			}

			return buffer;
		};
		Element::Container elements;
	};
	
	
}

