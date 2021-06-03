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
#include <vector>
#include <DirectXMath.h>


namespace En3rN::DX
{
	class Cube : public DynamicDrawable
	{
	public:
		Cube() : speed{}, acc{}
		{
			//vertecies
		float x = 1.0f / 4.0f;
		float y = 1.0f / 3.0f;
		float p = 0.5f;

		std::vector<VertexPos> vertecies{

			{-p,+p,+p}, //up     
			{+p,+p,+p},          
			{+p,+p,-p},          
			{-p,+p,-p}, 
			{-p,+p,+p}, //left            
			{-p,+p,-p},
			{-p,-p,-p},
			{-p,-p,+p},
			{-p,+p,-p}, //front  
			{+p,+p,-p},
			{+p,-p,-p},
			{-p,-p,-p},
			{+p,+p,-p}, //right  
			{+p,+p,+p},
			{+p,-p,+p},
			{+p,-p,-p},
			{-p,-p,+p}, //back  
			{+p,-p,+p},
			{+p,+p,+p},
			{-p,+p,+p},
			{-p,-p,-p}, //down   
			{+p,-p,-p},
			{+p,-p,+p},
			{-p,-p,+p},
		};
		std::vector<uint16_t> indecies{ 0, 1, 2, 2, 3, 0,   //
										4, 5, 6, 6, 7, 4,   //
										8, 9,10,10,11, 8,   //
									   12,13,14,14,15,12,   //
									   16,17,18,18,19,16,   //
									   20,21,22,22,23,20};  //
		indexCount = (UINT)std::size(indecies);

			auto vs = std::make_unique<VertexShader>(L"VertexShader.cso");
			indexCount =(UINT)std::size(indecies);
			AddBindable(std::make_unique <VertexBuffer<VertexPos>>(vertecies));
			AddBindable(std::make_unique<IndexBuffer>(indecies));
			AddBindable(std::make_unique<PixelShader>(L"PixelShader.cso"));
			AddBindable(std::make_unique <InputLayout>(InputLayout::Position, vs->GetBlob()));
			AddBindable(std::move(vs));
			AddBindable(std::make_unique <Texture>(L"cube.png", Texture::Type::Cube, DirectX::TEX_FILTER_MIRROR));
			AddBindable(std::make_unique <Rasterizer>(CullMode::Back));
			AddBindable(std::make_unique <Stencil>(Stencil::State::DepthStencilEnabled));
			UpdateViewMatrix();
			AddBindable(std::make_unique<Transform>(Transform::Matrix{viewMatrix}, *this));
		}
		Cube(Vec3f& worldpos) : Cube(){
			SetPosition(worldpos);
		}
		Cube(Vec3f& worldpos, Vec3f& dirvec) :Cube(){
			SetPosition(worldpos);
			SetDirection(dirvec);
		}
		Cube(Cube&& other) = default;
		void Update(float dt) override {
			pos = pos.Rotate(0,0,dt);
			UpdateViewMatrix();
		}
		void Draw() override{
			pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			pContext->DrawIndexed(indexCount, 0, 0);
		};
	private:
		uint8_t speed;
		uint8_t acc;
		float angle = 0;
	};
}
