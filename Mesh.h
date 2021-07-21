#pragma once
#include "end3d11.h"
#include "vec.h"
#include <vector>

namespace En3rN::DX
{	
	class Mesh
	{
	public:
		Mesh(Vec3f pos, Vec3f norm, Vec3f tang, Vec3f bitang, Vec4u8 color) {};
	
		using Container = std::vector<Mesh>;
		//void AddVertex(Vertex& vertex);
		void AddElement();

	private:
		std::vector<uint8_t>					m_vertexbuffer;
		std::vector<uint16_t>					m_indexBuffer;
		std::vector<D3D11_INPUT_ELEMENT_DESC>	m_inputLayout;
	};
}

