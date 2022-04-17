#include "Primitive.h"

namespace En3rN::DX
{

	Model::handle En3rN::DX::Primitive::LoadCube(Vec3f&& pos, Material&& mat)
	{
		auto model = std::make_unique<Model>(std::string("Cube"));
		enBuffer buf;
		float x = 1.0f / 4.0f;
		float y = 1.0f / 3.0f;
		float p = 0.5f;
		Vec3f vertecies[24]
		{
			{-p,+p,+p,}, //up    // 0.0f,1.0f
			{+p,+p,+p,},         // 1.0f,1.0f
			{+p,+p,-p,},         // 1.0f,0.0f
			{-p,+p,-p,},
			
			{-p,+p,+p,}, //left  // 0.0f,1.0f         
			{-p,+p,-p,},		 // 1.0f,1.0f
			{-p,-p,-p,},		 // 1.0f,0.0f
			{-p,-p,+p,},
			
			{-p,+p,-p,}, //front // 0.0f,1.0f
			{+p,+p,-p,},		 // 1.0f,1.0f
			{+p,-p,-p,},		 // 1.0f,0.0f
			{-p,-p,-p,},
			
			{+p,+p,-p,}, //right // 0.0f,1.0f
			{+p,+p,+p,},		 // 1.0f,1.0f
			{+p,-p,+p,},		 // 1.0f,0.0f
			{+p,-p,-p,},
			
			{-p,-p,+p,}, //back  // 0.0f,1.0f
			{+p,-p,+p,},		 // 1.0f,1.0f
			{+p,+p,+p,},		 // 1.0f,0.0f
			{-p,+p,+p,},
			
			{-p,-p,-p,}, //down  // 0.0f,1.0f
			{+p,-p,-p,},		 // 1.0f,1.0f
			{+p,-p,+p,},		 // 1.0f,0.0f
			{-p,-p,+p,},		 // 0.0f,0.0f
		};
		VertexElement::Normal normals[]
		{
			{0.f,1.f,0.f},
			{0.f,1.f,0.f},
			{0.f,1.f,0.f},
			{0.f,1.f,0.f},
				
			{-1.f,0.f,0.f},
			{-1.f,0.f,0.f},
			{-1.f,0.f,0.f},
			{-1.f,0.f,0.f},

			{0.f,0.f,-1.f},
			{0.f,0.f,-1.f},
			{0.f,0.f,-1.f},
			{0.f,0.f,-1.f},

			{1.f,0.f,0.f},
			{1.f,0.f,0.f},
			{1.f,0.f,0.f},
			{1.f,0.f,0.f},

			{0.f,0.f,1.f},
			{0.f,0.f,1.f},
			{0.f,0.f,1.f},
			{0.f,0.f,1.f},

			{0.f,-1.f,0.f},
			{0.f,-1.f,0.f},
			{0.f,-1.f,0.f},
			{0.f,-1.f,0.f},
		};
		Vec2f texCoord[24]
		{
			{0,0},{0,0}, {0,0},{0,0},
			{0,0},{0,0}, {0,0},{0,0},
			{0,0},{0,0}, {0,0},{0,0},
			{0,0},{0,0}, {0,0},{0,0},
			{0,0},{0,0}, {0,0},{0,0},
			{0,0},{0,0}, {0,0},{0,0}
		};
		Vec3f test(1, 1, 1);

		//VertexElement::TexCoord texcoord[]{};*/
		buf.add_element(vertecies,"Position",std::size(vertecies));
		buf.add_element(texCoord, "TexCoord",std::size(texCoord));
		buf.add_element(normals, std::size(normals));		

		std::vector<uint16_t> indecies
		{
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9,10,10,11, 8,
		   12,13,14,14,15,12,
		   16,17,18,18,19,16,
		   20,21,22,22,23,20
		};
		
		auto vertexbuffer = BindableManager::Query<VertexBuffer>(buf, model->GetName());
		auto indexbuffer = BindableManager::Query<IndexBuffer>(indecies, model->GetName());
		auto topology = BindableManager::Query <Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		auto vshader = BindableManager::Query <VertexShader>("vertexshader.cso");
		auto inputlayout = BindableManager::Query <InputLayout>(vshader->GetSignatures(), vshader->GetBlob(), model->GetName());


		Mesh mesh(vertexbuffer,indexbuffer,topology,vshader,inputlayout,0);
		mesh.SetName(model->GetName());
		model->AddMesh(std::move(mesh));
		model->AddMaterial(mat);
		model->GetRootNode()->AddMeshIndex(0);
		
		model->ApplyTransform(pos,Vec3f(0,0,0),Vec3f(1,1,1));
		//model->AddTeqnique(Teqnique("Phong"));
		return std::move(model);
	}

	Model::handle Primitive::LoadPlane(Vec3f&& pos, Material&& mat)
	{
		return std::make_shared<Model>("notimpl");
	}

	Model::handle Primitive::LoadSphere(Vec3f&& pos, Material&& mat)
	{
		return std::make_shared<Model>("notimpl");
	}
}
