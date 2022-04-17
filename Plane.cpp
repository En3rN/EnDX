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
#include "Transform.h"
#include "Material.h"


namespace En3rN::DX
{
    Plane::Plane()
    {
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
        auto vs = BindableManager::Query<VertexShader>  ("VSPosTex.cso");
        auto ps = BindableManager::Query<PixelShader>   ("PSTexture.cso");
        auto signatures = vs->GetSignatures();
        enBuffer buf;
        buf.add_element(positions,std::size(positions));
        buf.add_element(texCoords,std::size(texCoords));
        buf.create_buffer(signatures);

        std::vector<uint16_t> indecies{ 0,1,2,2,3,0 };
        indexCount = (UINT)std::size(indecies);

        AddBindable(BindableManager::Query<VertexBuffer>          (buf, "plane"));
        AddBindable(BindableManager::Query<IndexBuffer>         (indecies, "plane"));
        AddBindable(BindableManager::Query<InputLayout>         (signatures, vs->GetBlob(),"plane"));
        AddBindable(BindableManager::Query<Texture>             (std::filesystem::path("gafi.jpg")));
        AddBindable(BindableManager::Query<Sampler>             (Sampler::State::Wrap));
        AddBindable(BindableManager::Query<Rasterizer>          (Rasterizer::State::None));
        AddBindable(BindableManager::Query<Material::ConstantBuffer>(1, 1));
        AddBindable(BindableManager::Query<Transform::ConstantBuffer > (1, 1));
        AddBindable(std::move(vs));
        AddBindable(std::move(ps));

        //behavior = Behaviors::Get(id);
    }
    Plane::Plane(Vec3f pos) : Plane()
    {
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
