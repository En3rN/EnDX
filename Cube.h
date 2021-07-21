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
#include <vector>
#include <DirectXMath.h>


namespace En3rN::DX
{
	class Cube : public DynamicDrawable
	{
	public:
		Cube() 
		{
			//vertecies
			float x = 1.0f / 4.0f;
			float y = 1.0f / 3.0f;
			float p = 0.5f;
			UpdateViewMatrix();

			std::vector<VertexPos> vertecies
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
			std::vector<uint16_t> indecies
			{
				0, 1, 2, 2, 3, 0,
				4, 5, 6, 6, 7, 4,
				8, 9,10,10,11, 8,
			   12,13,14,14,15,12,
			   16,17,18,18,19,16,
			   20,21,22,22,23,20
			};

			indexCount = (UINT)std::size(indecies);
			auto vs = std::make_unique  <VertexShader>(L"VertexShader.cso");
			AddBindable(std::make_unique<VertexBuffer<VertexPos>>(vertecies));
			AddBindable(std::make_unique<InputLayout>(InputLayout::Position, vs->GetBlob()));
			AddBindable(std::make_unique<IndexBuffer>(indecies));
			AddBindable(std::make_unique<PixelShader>(L"PSSkybox.cso"));
			AddBindable(std::move(vs));
			AddBindable(BindableManager::Query<Texture>(L"cube.png", Texture::Type::CubeMap));
			AddBindable(std::make_unique <Sampler>(Sampler::State::Wrap));
			AddBindable(std::make_unique <Rasterizer>(State::Back));
			AddBindable(std::make_unique <Stencil>(Stencil::State::DepthStencilEnabled));
			AddBindable(std::make_unique<Transform>(Transform::Data{ viewMatrix }, * this));
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
			behavior(*this, dt);
		}
		void Draw() override{
			pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			pContext->DrawIndexed(indexCount, 0, 0);
		};
	private:
		Behaviors::Behavior behavior;
	};
}
