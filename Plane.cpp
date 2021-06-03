#include "Plane.h"
#include "Buffer.h"
#include "..\DirectXTex\DirectXTex\DirectXTex.h"
#include "enString.h"
#include "logger.h"
#include "Texture.h"
#include "Shader.h"
#include "Sampler.h"
#include "Rasterizer.h"


namespace En3rN::DX
{
    Plane::Plane(std::wstring file)
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
        std::vector<uint16_t> indecies{ 0,1,2,2,3,0 };
        indexCount = (UINT)std::size(indecies);
        auto vs =   std::make_unique <VertexShader> (L"VSPosTex.cso");
        auto ps =   std::make_unique <PixelShader>  (L"PSTexture.cso");
        AddBindable(std::make_unique <VertexBuffer<VertexPosTextCoord>>(vertecies));
        AddBindable(std::make_unique <IndexBuffer>  (indecies));
        AddBindable(std::make_unique <InputLayout>  (InputLayout::PositionTexCoord,vs.get()->GetBlob()));
        AddBindable(std::move(vs));
        AddBindable(std::move(ps));
        AddBindable(std::make_unique <Texture>      (file));
        AddBindable(std::make_unique <Sampler>      (Sampler::State::Border));
        AddBindable(std::make_unique <Rasterizer>   (CullMode::Back));
        AddBindable(std::make_unique <Transform>    (Transform::Matrix{ viewMatrix }, *this));
    }
    void Plane::LoadTexture(std::wstring& file)
    {
    }
    void Plane::Draw()
    {
        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        pContext->DrawIndexed(indexCount, 0, 0);
    }
}
