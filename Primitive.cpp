#include "Primitive.h"
#include "SkyboxTeq.h"

namespace En3rN::DX
{

	Model::handle En3rN::DX::Primitive::LoadCube(Vec3f&& pos, Material&& mat)
	{
		auto model = std::make_unique<Model>();
		std::string name = "Cube";
		enBuffer buf;
		float x = 1.0f / 4.0f;
		float y = 1.0f / 3.0f;
		float p = 0.5f;
		VertexElement::Pos vertecies[]
		{
			{-p,+p,+p,}, //up    // 0.0f,1.0f
			{+p,+p,+p,},         // 1.0f,1.0f
			{+p,+p,-p,},         // 1.0f,0.0f
			{-p,+p,-p,}, 		 // 0.0f,0.0f
			{-p,+p,+p,}, //left  // 0.0f,1.0f         
			{-p,+p,-p,},		 // 1.0f,1.0f
			{-p,-p,-p,},		 // 1.0f,0.0f
			{-p,-p,+p,},		 // 0.0f,0.0f
			{-p,+p,-p,}, //front // 0.0f,1.0f
			{+p,+p,-p,},		 // 1.0f,1.0f
			{+p,-p,-p,},		 // 1.0f,0.0f
			{-p,-p,-p,},		 // 0.0f,0.0f
			{+p,+p,-p,}, //right // 0.0f,1.0f
			{+p,+p,+p,},		 // 1.0f,1.0f
			{+p,-p,+p,},		 // 1.0f,0.0f
			{+p,-p,-p,},		 // 0.0f,0.0f
			{-p,-p,+p,}, //back  // 0.0f,1.0f
			{+p,-p,+p,},		 // 1.0f,1.0f
			{+p,+p,+p,},		 // 1.0f,0.0f
			{-p,+p,+p,},		 // 0.0f,0.0f
			{-p,-p,-p,}, //down  // 0.0f,1.0f
			{+p,-p,-p,},		 // 1.0f,1.0f
			{+p,-p,+p,},		 // 1.0f,0.0f
			{-p,-p,+p,},		 // 0.0f,0.0f
		};
		/*VertexElement::Normal normals[]{};
		VertexElement::TexCoord texcoord[]{};*/
		buf.add_element(vertecies, std::size(vertecies));

		/*std::vector<uint16_t> indecies
		{
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9,10,10,11, 8,
		   12,13,14,14,15,12,
		   16,17,18,18,19,16,
		   20,21,22,22,23,20
		};*/
		auto material = std::make_shared<Material>(mat);
		
		auto vertexbuffer = BindableManager::Query<VertexBuffer>(buf, name);
		auto indexbuffer = BindableManager::Query<IndexBuffer>(buf.indecies(4), name);

		Mesh mesh(vertexbuffer, indexbuffer,0);
		mesh.SetPosition(pos);
		mesh.SetName(std::move(name));
		model->AddMesh(std::move(mesh));
		model->AddMaterial(mat);
		model->AddTeqnique(std::make_shared<TeqSkybox>());
		return std::move(model);
	}

	Model::handle Primitive::LoadPlane(Vec3f&& pos, Material&& mat)
	{
		return std::make_shared<Model>();
	}

	Model::handle Primitive::LoadSphere(Vec3f&& pos, Material&& mat)
	{
		return std::make_shared<Model>();
	}
}
