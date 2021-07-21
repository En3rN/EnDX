#include "Plane.h"
#include "Buffer.h"
#include "enBuffer.h"
#include "..\DirectXTex\DirectXTex\DirectXTex.h"
#include "enString.h"
#include "logger.h"
#include "Texture.h"
#include "Shader.h"
#include "Sampler.h"
#include "Rasterizer.h"
#include "InputLayout.h"
#include "BindableManager.h"


namespace En3rN::DX
{
    Plane::Plane()
    {
        pos = { 0.0f,0.0f,0.0f };
        scale= { 1.0f,1.0f,1.0f };
        dir = {};
        std::vector<VertexPosTextCoord> vertecies{
            {-0.5f,+0.5f,0.0f,  -0.01f,-0.01f},
            {+0.5f,+0.5f,0.0f,   1.01f,-0.01f},
            {+0.5f,-0.5f,0.0f,   1.01f, 1.01f},
            {-0.5f,-0.5f,0.0f,  -0.01f, 1.01f}
        };
        VertexElement::Pos positions[]
        {
            {-0.5f,+0.5f,0.0f},
            {+0.5f,+0.5f,0.0f},
            {+0.5f,-0.5f,0.0f},
            {-0.5f,-0.5f,0.0f}
        };
        VertexElement::TexCoord texCoords[]
        {
            { 0.0f, 0.0f},
            { 1.0f, 0.0f},
            { 1.0f, 1.0f},
            { 0.0f, 1.0f}
        };
        enBuffer buf;
        buf.add_element(positions,std::size(positions));
        buf.add_element(texCoords,std::size(texCoords));
        buf.create_buffer();
        
        std::vector<uint16_t> indecies{ 0,1,2,2,3,0 };
        indexCount = (UINT)std::size(indecies);
        auto vs =   std::make_unique <VertexShader> (L"VSPosTex.cso");
        auto ps =   std::make_unique <PixelShader>  (L"PSTexture.cso");
        AddBindable(std::make_unique <VertexBuff>   (buf));
        AddBindable(std::make_unique <IndexBuffer>  (indecies));
        AddBindable(std::make_unique <InputLayout>  (buf.input_element_desc(),vs.get()->GetBlob()));
        AddBindable(std::move(vs));
        AddBindable(std::move(ps));
        AddBindable(BindableManager::Query<Texture>      (L"gafi.jpg"));
        AddBindable(std::make_unique <Sampler>      (Sampler::State::Border));
        AddBindable(std::make_unique <Rasterizer>   (State::None));
        AddBindable(std::make_unique <Transform>    (Transform::Data{ viewMatrix }, *this));
        behavior = Behaviors::Get(id);
    }
    Plane::Plane(Vec3f pos) : Plane()
    {
        SetPosition(pos);
    }
    void Plane::LoadTexture(std::wstring& file)
    {
    }
    void Plane::Update(float dt)
    {
        behavior(*this, dt);        
    }
    void Plane::Draw()
    {
        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        pContext->DrawIndexed(indexCount, 0, 0);
    }
}
