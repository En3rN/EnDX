#include "Skybox.h"
#include "Shader.h"
#include "Sampler.h"
#include "Texture.h"
#include "Rasterizer.h"
#include "Camera.h"
#include "Blend.h"
#include "Stencil.h"
namespace En3rN::DX
{
	Skybox::Skybox(std::wstring file)
	{ 
        auto cam = Camera::GetActiveCamera();
        pos = cam.pos;
        scale = { 1,1,1};
        dir={};
        float x = 1.0f / 4.0f;
        float y = 1.0f / 3.0f;
        float p = 0.5f;

        float c = 0.01f;
        

        std::vector<VertexPosTextCoord> vertecies{

            {-p,+p,+p}, //up                // -c,  1 * x,1 * y
            {+p,+p,+p},                     // -c,  2 * x,1 * y
            {+p,+p,-p},             // +c        ,  2 * x,0 * y
            {-p,+p,-p},             // +c        ,  1 * x,0 * y
                     
            {-p,-p,-p}, //down        // -c      ,  1 * x,3 * y
            {+p,-p,-p},               // -c      ,  2 * x,3 * y
            {+p,-p,+p},                   // +c  ,  2 * x,2 * y
            {-p,-p,+p},                   // +c  ,  1 * x,2 * y
                     
            {-p,+p,+p},             //+c         ,  1 * x,1 * y
            {-p,+p,-p},             //+c         ,  0 * x,1 * y
            {-p,-p,-p},             //-c         ,  0 * x,2 * y
            {-p,-p,+p}, //left      //-c         ,  1 * x,2 * y
                     
            {-p,-p,+p}, //back      //-c         ,  1 * x,2 * y
            {+p,-p,+p},             //-c         ,  2 * x,2 * y
            {+p,+p,+p},             //+c         ,  2 * x,1 * y
            {-p,+p,+p},             //+c         ,  1 * x,1 * y
                     
            {+p,+p,-p}, //right    // +c         ,  3 * x,1 * y
            {+p,+p,+p},            // +c         ,  2 * x,1 * y
            {+p,-p,+p},            // -c         ,  2 * x,2 * y
            {+p,-p,-p},            // -c         ,  3 * x,2 * y
                     
            {-p,+p,-p}, //front           // +c  ,  4 * x,1 * y
            {+p,+p,-p},                   // +c  ,  3 * x,1 * y
            {+p,-p,-p},                   // -c  ,  3 * x,2 * y
            {-p,-p,-p},                   // -c  ,  4 * x,2 * y
                                                        //
        };
        std::vector<uint16_t> indecies{ 0, 1, 2, 2, 3, 0,   //
                                        4, 5, 6, 6, 7, 4,   //
                                        8, 9,10,10,11, 8,   //
                                       12,13,14,14,15,12,   //
                                       16,17,18,18,19,16,   //
                                       20,21,22,22,23,20};  //
        indexCount = (UINT)std::size(indecies);
        auto vs = std::make_unique <VertexShader>(L"VSSkybox.cso");
        auto ps = std::make_unique <PixelShader>(L"PSSkybox.cso");
        AddBindable(std::make_unique <VertexBuffer<VertexPosTextCoord>>(vertecies));
        AddBindable(std::make_unique <IndexBuffer>(indecies));
        AddBindable(std::make_unique <InputLayout>(InputLayout::Position, vs.get()->GetBlob()));
        AddBindable(std::move(vs));
        AddBindable(std::move(ps));
        AddBindable(std::make_unique <Texture>(file, Texture::Type::SkyBox));
        AddBindable(std::make_unique <Rasterizer>(CullMode::Front));
        AddBindable(std::make_unique <Stencil>(Stencil::State::DepthOnlyFuncLessEqualNoWrite));
        AddBindable(std::make_unique <Sampler>(Sampler::State::Clamp));
        AddBindable(std::make_unique <Transform>(Transform::Matrix{ viewMatrix }, *this));
	}
	void Skybox::Draw()
    {
        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        pContext->DrawIndexed(indexCount, 0, 0);
	}
    void Skybox::Update(float dt)
    {
        auto cam = Camera::GetActiveCamera();
        pos = cam.GetPosition();
    }
}