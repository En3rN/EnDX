#pragma once
#include "vec.h"

namespace En3rN::DX
{	

	/*template <class T>
	class Vertex 
	{
		virtual ~Vertex() = default;
		virtual std::vector<D3D11_INPUT_ELEMENT_DESC> GetDesc() = 0;
	};*/
	struct VertexPos 
	{
		float x, y, z;
	};
	struct VertexPosColor : VertexPos
	{
		uint8_t r,g,b,a;
	};
	struct VertexPosTextCoord : VertexPos
	{
		float u, v;
	};
	struct VertexPosTextCoord3d : VertexPos
	{
		float u, v, w;
	};
}

