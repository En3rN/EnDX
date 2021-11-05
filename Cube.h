#pragma once
#include "Entity.h"
#include "Vertex.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Drawable.h"
#include "Shader.h"
#include "Inputlayout.h"
#include "Buffer.h"
#include "Window.h"
#include "logger.h"
#include "Rasterizer.h"
#include "Stencil.h"
#include "Texture.h"
#include "BindableManager.h"
#include "Sampler.h"
#include "enMath.h"
#include "Behavior.h"
#include "enBuffer.h"
#include <vector>
#include <DirectXMath.h>
#include "Transform.h"

namespace En3rN::DX
{
	class Cube : public Drawable
	{
	public:
		Cube() 
		{
			name = "Cube";
			float x = 1.0f / 4.0f;
			float y = 1.0f / 3.0f;
			float p = 0.5f;
			VertexElement::Pos positions[]
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
			VertexElement::Normal normals[]
			{
				{0.f,1.f,0.f},{0.f,1.f,0.f},{0.f,1.f,0.f},{0.f,1.f,0.f},
				{-1.f,0.f,0.f},{-1.f,0.f,0.f},{-1.f,0.f,0.f},{-1.f,0.f,0.f},
				{0.f,0.f,-1.f},{0.f,0.f,-1.f},{0.f,0.f,-1.f},{0.f,0.f,-1.f},
				{1.f,0.f,0.f},{1.f,0.f,0.f},{1.f,0.f,0.f},{1.f,0.f,0.f},
				{0.f,0.f,1.f},{0.f,0.f,1.f},{0.f,0.f,1.f},{0.f,0.f,1.f},
				{0.f,-1.f,0.f},{0.f,-1.f,0.f},{0.f,-1.f,0.f},{0.f,-1.f,0.f},
			};
			std::vector<uint16_t> indecies
			{
				0, 1, 2, 2, 3, 0,
				4, 5, 6, 6, 7, 4,
				8, 9,10,10,11, 8,
			   12,13,14,14,15,12,
			   16,17,18,18,19,16,
			   20,21,22,22,23,20
			};
			
			auto vs = BindableManager::Query<VertexShader>("VertexShader.cso");		//("VertexShader.cso");
			auto signatures = vs->GetSignatures();
			enBuffer buf;
			buf.add_element(positions,std::size(positions));
			buf.add_element(normals, std::size(normals));
			buf.create_buffer(signatures);
			//auto indecies = buf.indecies(4);
			indexCount = (UINT)std::size(indecies);
			AddBindable(BindableManager::Query<PixelShader>("Test.cso"));	//("PSSkybox.cso"));
			AddBindable(BindableManager::Query<InputLayout>			(signatures, vs->GetBlob(),"cube"));
			AddBindable(BindableManager::Query<VertexBuffer>			(buf,"cube"));
			AddBindable(BindableManager::Query<IndexBuffer>			(indecies,"cube"));
			AddBindable(BindableManager::Query<Texture>				("cube.png",0, Texture::Type::CubeMap));
			AddBindable(BindableManager::Query<Sampler>				(Sampler::State::Wrap));
			AddBindable(BindableManager::Query<Rasterizer>			(Rasterizer::State::Back));
			AddBindable(BindableManager::Query<Stencil>				(Stencil::State::DepthOnly));
			AddBindable(BindableManager::Query<Transform::ConstantBuffer>	(1,1));
			AddBindable(std::move(vs));
			behavior = Behaviors::Get(id);
		}
		Cube(Vec3f worldpos) : Cube(){
			SetPosition(worldpos);
		}
		Cube(Vec3f worldpos, Vec3f dirvec) :Cube(){
			SetPosition(worldpos);
			SetDirection(dirvec);
		}
		Cube(Cube&& other) = default;
		void Update(float dt) override 
		{
			//behavior(*this, dt);
		}
		void Draw() override
		{
			pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			pContext->DrawIndexed(indexCount, 0, 0);
		};
	private:
		Behaviors::Behavior behavior;
	};
}
